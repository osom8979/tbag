/**
 * @file   WsFrame.cpp
 * @brief  WsFrame class implementation.
 * @author zer0
 * @date   2017-10-01
 * @date   2018-12-25 (Change namespace: libtbag::network::http::ws -> libtbag::http)
 */

#include <libtbag/http/WsFrame.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/bitwise/Endian.hpp>

#include <cassert>
#include <cstring>
#include <sstream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace http {

WsFrame::WsFrame() : fin (false), rsv1(false), rsv2(false), rsv3(false),
                     opcode(WsOpCode::WSOC_CONTINUATION_FRAME), mask(false),
                     masking_key(0)
{
    // EMPTY.
}

WsFrame::~WsFrame()
{
    // EMPTY.
}

void WsFrame::swap(WsFrame & obj)
{
    std::swap(fin, obj.fin);
    std::swap(rsv1, obj.rsv1);
    std::swap(rsv2, obj.rsv2);
    std::swap(rsv3, obj.rsv3);
    libtbag::http::swap(opcode, obj.opcode);
    std::swap(mask, obj.mask);
    std::swap(masking_key, obj.masking_key);
    payload.swap(obj.payload);
}

void WsFrame::clear()
{
    fin  = false;
    rsv1 = false;
    rsv2 = false;
    rsv3 = false;
    opcode = WsOpCode::WSOC_CONTINUATION_FRAME;
    mask = false;
    masking_key = 0;
    payload.clear();
}

Err WsFrame::execute(char const * data, std::size_t size, std::size_t * read_size)
{
    if (size < WsFrame::MINIMUM_BUFFER_SIZE) {
        return E_SMALLBUF; // Check minimum size.
    }

    auto temp_opcode = data[0] & 0x0F; // 00001111 00000000 // Forces the first checker.
    auto temp_fin    = data[0] & 0x80; // 10000000 00000000
    auto temp_rsv1   = data[0] & 0x40; // 01000000 00000000
    auto temp_rsv2   = data[0] & 0x20; // 00100000 00000000
    auto temp_rsv3   = data[0] & 0x10; // 00010000 00000000
    auto temp_mask   = data[1] & 0x80; // 00000000 10000000
    auto temp_pl7bit = data[1] & 0x7F; // 00000000 01111111

    auto const PAYLOAD_LENGTH_7BIT = static_cast<uint8_t>(temp_pl7bit);
    auto const PAYLOAD_BIT         = getWsPayloadBit(PAYLOAD_LENGTH_7BIT);
    auto const MASK_KEY_INDEX      = getMaskingKeyByteIndex(PAYLOAD_BIT);
    auto const DATA_INDEX          = getPayloadDataByteIndex(PAYLOAD_BIT, static_cast<bool>(temp_mask));
    auto const PAYLOAD_LENGTH      = getPayloadLength(data);

    if (size < MASK_KEY_INDEX) {
        return E_SMALLBUF; // Check header data.
    }
    if (temp_mask && size < DATA_INDEX) {
        return E_SMALLBUF; // Check masking key.
    }
    if (PAYLOAD_LENGTH > 0 && size < DATA_INDEX + PAYLOAD_LENGTH) {
        return E_SMALLBUF; // Check payload data.
    }

    // Parsing OK !! You are ready to assign.

    opcode = static_cast<WsOpCode>(temp_opcode);
    fin    = static_cast<bool>(temp_fin);
    rsv1   = static_cast<bool>(temp_rsv1);
    rsv2   = static_cast<bool>(temp_rsv2);
    rsv3   = static_cast<bool>(temp_rsv3);
    mask   = static_cast<bool>(temp_mask);
    payload_length = PAYLOAD_LENGTH;

    if (mask) {
        // Update masking key.
        masking_key = copyMaskingKeyFromBuffer(&data[MASK_KEY_INDEX]);
    }

    if (payload_length > 0) {
        // Update payload data.
        payload.assign(&data[DATA_INDEX], &data[DATA_INDEX] + payload_length);
        if (mask) {
            updatePayloadData(masking_key, payload.data(), payload_length);
        }
    }

    if (read_size != nullptr) {
        *read_size = calculateBufferSize(payload_length, mask);
    }
    return E_SUCCESS;
}

std::size_t WsFrame::copyTo(char * buffer, std::size_t size) const
{
    if (size < calculateBufferSize(payload_length, mask)) {
        return 0;
    }

#if defined(max)
    TBAG_PUSH_MACRO(max);
#undef max
#define __RESTORE_MAX__
#endif

    TBAG_CONSTEXPR std::size_t const MAX_UINT16_BYTE_SIZE = std::numeric_limits<uint16_t>::max();
    TBAG_CONSTEXPR std::size_t const MAX_UINT64_BYTE_SIZE = std::numeric_limits<uint64_t>::max();

#if defined(__RESTORE_MAX__)
    TBAG_POP_MACRO(max);
#undef __RESTORE_MAX__
#endif

    ::memset(buffer, 0x00, size);

    *(buffer + 0) |= ( fin ? 0x80 : 0);
    *(buffer + 0) |= (rsv1 ? 0x40 : 0);
    *(buffer + 0) |= (rsv2 ? 0x20 : 0);
    *(buffer + 0) |= (rsv3 ? 0x10 : 0);
    *(buffer + 0) |= static_cast<char>(opcode);
    *(buffer + 1) |= (mask ? 0x80 : 0);

    // Next index (Written size)
    std::size_t index = WEBSOCKET_MINIMUM_HEADER_BYTE_SIZE;

    // Update payload length.
    if (payload_length <= WS_PAYLOAD_7BIT_TYPE_SIZE) {
        *(buffer + 1) |= payload_length;
    } else if (payload_length <= MAX_UINT16_BYTE_SIZE) {
        *(buffer + 1) |= 0x7E;
        uint16_t temp = bitwise::toNetwork(static_cast<uint16_t>(payload_length));
        ::memcpy(buffer + index, &temp, sizeof(temp));
        index += sizeof(uint16_t);
    } else {
        assert(MAX_UINT16_BYTE_SIZE < COMPARE_AND(payload_length) <= MAX_UINT64_BYTE_SIZE);
        *(buffer + 1) |= 0x7F;
        uint64_t temp = bitwise::toNetwork(static_cast<uint64_t>(payload_length));
        ::memcpy(buffer + index, &temp, sizeof(temp));
        index += sizeof(uint64_t);
    }

    // Update masking key.
    if (mask) {
        ::memcpy(buffer + index, &masking_key, sizeof(uint32_t));
        index += sizeof(uint32_t);
    }

    // Update payload data.
    if (payload_length > 0) {
        ::memcpy(buffer + index, payload.data(), payload_length);
        if (mask) {
            updatePayloadData(masking_key, buffer + index, payload_length);
        }
        index += payload_length;
    }

    return index;
}

std::size_t WsFrame::copyTo(util::Buffer & buffer) const
{
    buffer.resize(calculateBufferSize(payload_length, mask));
    return copyTo(buffer.data(), buffer.size());
}

void WsFrame::setHeader(bool f, bool r1, bool r2, bool r3, WsOpCode op, uint32_t key) TBAG_NOEXCEPT
{
    fin = f;
    rsv1 = r1;
    rsv2 = r2;
    rsv3 = r3;
    opcode = op;
    mask = (key != 0);
    masking_key = key;
}

void WsFrame::setData(char const * data, std::size_t size)
{
    if (data == nullptr || size == 0) {
        payload.clear();
        payload_length = 0;
    } else {
        payload.assign(data, data + size);
        payload_length = size;
    }
}

void WsFrame::set(bool f, bool r1, bool r2, bool r3, WsOpCode op, char const * data, std::size_t size, uint32_t key)
{
    setHeader(f, r1, r2, r3, op, key);
    setData(data, size);
}

void WsFrame::text(char const * buffer, std::size_t size, uint32_t key, bool finish)
{
    set(finish, false, false, false, WsOpCode::WSOC_TEXT_FRAME, buffer, size, key);
}

void WsFrame::text(char const * buffer, std::size_t size, bool finish)
{
    text(buffer, size, 0, finish);
}

void WsFrame::text(std::string const & str, uint32_t key, bool finish)
{
    text(str.c_str(), str.size(), key, finish);
}

void WsFrame::text(std::string const & str, bool finish)
{
    text(str, 0, finish);
}

void WsFrame::binary(char const * buffer, std::size_t size, uint32_t key, bool finish)
{
    set(finish, false, false, false, WsOpCode::WSOC_BINARY_FRAME, buffer, size, key);
}

void WsFrame::binary(char const * buffer, std::size_t size, bool finish)
{
    binary(buffer, size, 0, finish);
}

void WsFrame::binary(util::Buffer const & buffer, uint32_t key, bool finish)
{
    binary(buffer.data(), buffer.size(), key, finish);
}

void WsFrame::binary(util::Buffer const & buffer, bool finish)
{
    binary(buffer, 0, finish);
}

void WsFrame::close(uint32_t key)
{
    set(true, false, false, false, WsOpCode::WSOC_CONNECTION_CLOSE, nullptr, 0, key);
}

void WsFrame::close(uint16_t code, std::string const & reason)
{
    code = bitwise::toNetwork(code);
    util::Buffer buffer(sizeof(uint16_t) + reason.size());
    memcpy(&buffer[0], &code, sizeof(uint16_t));
    memcpy(&buffer[sizeof(uint16_t)], &reason[0], reason.size());
    set(true, false, false, false, WsOpCode::WSOC_CONNECTION_CLOSE, buffer.data(), buffer.size());
}

void WsFrame::close(WsStatusCode code)
{
    return close(getWsStatusCodeNumber(code), getWsStatusCodeName(code));
}

void WsFrame::close(WsStatus const & status)
{
    close(status.code, status.reason);
}

WsStatus WsFrame::getWsStatus() const
{
    return WsStatus(payload);
}

void WsFrame::ping(char const * data, std::size_t size, uint32_t key)
{
    assert(size <= WS_PAYLOAD_7BIT_TYPE_SIZE);
    set(true, false, false, false, WsOpCode::WSOC_DENOTES_PING, data, size, key);
}

void WsFrame::ping(std::string const & str, uint32_t key)
{
    ping(str.data(), str.size(), key);
}

void WsFrame::pong(char const * data, std::size_t size, uint32_t key)
{
    assert(size <= WS_PAYLOAD_7BIT_TYPE_SIZE);
    set(true, false, false, false, WsOpCode::WSOC_DENOTES_PONG, data, size, key);
}

void WsFrame::pong(std::string const & str, uint32_t key)
{
    pong(str.data(), str.size(), key);
}

std::string WsFrame::toDebugString(bool print_payload) const
{
    std::stringstream ss;
    ss << "WS[" << (fin?'1':'0') << (rsv1?'1':'0') << (rsv2?'1':'0') << (rsv3?'1':'0')
       << '/' << getWsOpCodeName(opcode) << ']';
    if (mask) {
        ss << " M(" << masking_key << ")";
    }
    if (print_payload && payload_length > 0) {
        if (opcode == WsOpCode::WSOC_TEXT_FRAME) {
            ss << std::string(payload.data(), payload.data() + payload_length);
        } else {
            auto data = std::vector<uint8_t>((uint8_t*)payload.data(),
                                             (uint8_t*)payload.data() + payload_length);
            ss << string::convertByteVectorToHexString(data);
        }
    }
    return ss.str();
}

} // namespace http

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

