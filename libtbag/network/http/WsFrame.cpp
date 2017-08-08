/**
 * @file   WsFrame.cpp
 * @brief  WsFrame class implementation.
 * @author zer0
 * @date   2017-06-11
 * @date   2017-08-07 (Rename: WebSocketFrame -> WsFrame)
 */

#include <libtbag/network/http/WsFrame.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/bitwise/Endian.hpp>
#include <libtbag/encrypt/Base64.hpp>
#include <libtbag/encrypt/Sha1.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/random/MaskingDevice.hpp>
#include <libtbag/id/Uuid.hpp>

#include <cassert>
#include <cstring>

#include <algorithm>
#include <sstream>
#include <iostream>
#include <utility>
#include <limits>
#include <set>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {

#if defined(max)
TBAG_PUSH_MACRO(max);
#undef max
#define __RESTORE_MAX__
#endif

TBAG_CONSTEXPR static std::size_t const MAX_UINT16_BYTE_SIZE = std::numeric_limits<uint16_t>::max();
TBAG_CONSTEXPR static std::size_t const MAX_UINT64_BYTE_SIZE = std::numeric_limits<uint64_t>::max();

#if defined(__RESTORE_MAX__)
TBAG_POP_MACRO(max);
#undef __RESTORE_MAX__
#endif

TBAG_CONSTEXPR static uint8_t const PAYLOAD_7BIT_TYPE_SIZE  = 125;
TBAG_CONSTEXPR static uint8_t const PAYLOAD_16BIT_TYPE_SIZE = 126;
TBAG_CONSTEXPR static uint8_t const PAYLOAD_64BIT_TYPE_SIZE = 127;

WsStatusCode getWsStatusCode(uint16_t code) TBAG_NOEXCEPT
{
    switch (code) {
#define _TBAG_XX(num, name, str) case num: return WsStatusCode::WSSC_##name;
    TBAG_WEB_SOCKET_STATUS_CODE_MAP(_TBAG_XX)
#undef _TBAG_XX
    default: return WsStatusCode::WSSC_UNKNOWN;
    }
}

char const * getWsStatusCodeName(WsStatusCode code) TBAG_NOEXCEPT
{
    switch (code) {
#define _TBAG_XX(num, name, str) case WsStatusCode::WSSC_##name: return #name;
    TBAG_WEB_SOCKET_STATUS_CODE_MAP(_TBAG_XX)
#undef _TBAG_XX
    default: return "WSSC_UNKNOWN";
    }
}

char const * getWsStatusCodeReason(WsStatusCode code) TBAG_NOEXCEPT
{
    switch (code) {
#define _TBAG_XX(num, name, str) case WsStatusCode::WSSC_##name: return str;
    TBAG_WEB_SOCKET_STATUS_CODE_MAP(_TBAG_XX)
#undef _TBAG_XX
    default: return "Unknown reason";
    }
}

uint16_t getWsStatusCodeNumber(WsStatusCode code) TBAG_NOEXCEPT
{
    switch (code) {
#define _TBAG_XX(num, name, str) case WsStatusCode::WSSC_##name: return num;
    TBAG_WEB_SOCKET_STATUS_CODE_MAP(_TBAG_XX)
#undef _TBAG_XX
    default: return TBAG_UNKNOWN_WEBSOCKET_STATUS_CODE;
    }
}

// ------------------------------
// WsFrame implementation.
// ------------------------------

WsFrame::WsFrame()
        : fin (false), rsv1(false), rsv2(false), rsv3(false),
          opcode(OpCode::OC_CONTINUATION_FRAME), mask(false),
          payload_length(0), masking_key(0)
{
    // EMPTY.
}

WsFrame::WsFrame(WsFrame const & obj)
{
    (*this) = obj;
}

WsFrame::WsFrame(WsFrame && obj)
{
    (*this) = std::move(obj);
}

WsFrame::~WsFrame()
{
    // EMPTY.
}

WsFrame & WsFrame::operator =(WsFrame const & obj)
{
    if (this != &obj) {
        fin = obj.fin;
        rsv1 = obj.rsv1;
        rsv2 = obj.rsv2;
        rsv3 = obj.rsv3;
        opcode = obj.opcode;
        mask = obj.mask;
        payload_length = obj.payload_length;
        masking_key = obj.masking_key;
        payload = obj.payload;
    }
    return *this;
}

WsFrame & WsFrame::operator =(WsFrame && obj)
{
    if (this != &obj) {
        std::swap(fin, obj.fin);
        std::swap(rsv1, obj.rsv1);
        std::swap(rsv2, obj.rsv2);
        std::swap(rsv3, obj.rsv3);
        std::swap(opcode, obj.opcode);
        std::swap(mask, obj.mask);
        std::swap(payload_length, obj.payload_length);
        std::swap(masking_key, obj.masking_key);
        payload.swap(obj.payload);
    }
    return *this;
}

void WsFrame::clear()
{
    fin  = false;
    rsv1 = false;
    rsv2 = false;
    rsv3 = false;
    opcode = OpCode::OC_CONTINUATION_FRAME;
    mask = false;
    payload_length = 0;
    masking_key = 0;
    payload.clear();
}

Err WsFrame::execute(uint8_t const * data, std::size_t size, std::size_t * read_size)
{
    if (size < WsFrame::MINIMUM_BUFFER_SIZE) {
        return Err::E_SMALLBUF;
    }

    std::size_t calculated_byte = 0;

    opcode = static_cast<OpCode>(data[0] & 0x0F /* 0b00001111 */); // Forces the first checker.
    fin    = static_cast<  bool>(data[0] & 0x80 /* 0b10000000 */);
    rsv1   = static_cast<  bool>(data[0] & 0x40 /* 0b01000000 */);
    rsv2   = static_cast<  bool>(data[0] & 0x20 /* 0b00100000 */);
    rsv3   = static_cast<  bool>(data[0] & 0x10 /* 0b00010000 */);
    mask   = static_cast<  bool>(data[1] & 0x80 /* 0b10000000 */);

    auto const PAYLOAD_LENGTH_7BIT = static_cast<uint8_t>(data[1] & 0x7F /* 0b01111111 */);
    auto const PAYLOAD_BIT    = getPayloadBit(PAYLOAD_LENGTH_7BIT);
    auto const MASK_KEY_INDEX = getMaskingKeyByteIndex(PAYLOAD_BIT);
    auto const DATA_INDEX     = getPayloadDataByteIndex(PAYLOAD_BIT, mask);
    calculated_byte = WsFrame::MINIMUM_BUFFER_SIZE;

    if (size < MASK_KEY_INDEX) {
        return Err::E_SMALLBUF;
    }

    uint64_t const PREV_PAYLOAD_LENGTH = payload_length;

    // Update payload length.
    if (PAYLOAD_BIT == PayloadBit::PL_BIT_7) {
        payload_length = PAYLOAD_LENGTH_7BIT;
    } else if (PAYLOAD_BIT == PayloadBit::PL_BIT_16) {
        uint16_t temp = 0;
        ::memcpy(&temp, &data[2], sizeof(uint16_t));
        payload_length = bitwise::toHost(temp);
    } else if (PAYLOAD_BIT == PayloadBit::PL_BIT_64) {
        uint64_t temp = 0;
        ::memcpy(&temp, &data[2], sizeof(uint64_t));
        payload_length = bitwise::toHost(temp);
    } else {
        TBAG_INACCESSIBLE_BLOCK_ASSERT();
    }

    // Update masking key.
    if (mask) {
        if (size < DATA_INDEX) {
            return Err::E_SMALLBUF;
        }
        masking_key = getMaskingKey(&data[MASK_KEY_INDEX]);
    } else {
        masking_key = 0;
    }

    // Update payload data.
    if (payload_length > 0) {
        if (size < DATA_INDEX + payload_length) {
            return Err::E_SMALLBUF;
        }

        if (opcode == OpCode::OC_CONTINUATION_FRAME) {
            if (payload.size() < payload_length + PREV_PAYLOAD_LENGTH) {
                payload.resize(payload_length + PREV_PAYLOAD_LENGTH);
            }
            payload.insert(payload.begin() + PREV_PAYLOAD_LENGTH, &data[DATA_INDEX], &data[DATA_INDEX] + payload_length);
            if (mask) {
                updatePayloadData(masking_key, payload.data() + PREV_PAYLOAD_LENGTH, payload_length);
            }
        } else {
            if (payload.size() < payload_length) {
                payload.resize(payload_length);
            }
            payload.assign(&data[DATA_INDEX], &data[DATA_INDEX] + payload_length);
            if (mask) {
                updatePayloadData(masking_key, payload.data(), payload_length);
            }
        }
    }

    if (opcode == OpCode::OC_CONTINUATION_FRAME) {
        payload_length += PREV_PAYLOAD_LENGTH;
    }

    if (read_size != nullptr) {
        *read_size = calculateWriteBufferSize();
    }
    return Err::E_SUCCESS;
}

std::size_t WsFrame::calculateWriteBufferSize() const
{
    std::size_t default_size = 2/*HEADER*/ + payload_length + (mask ? sizeof(uint32_t) : 0);
    switch (getPayloadBitWithPayloadLength(payload_length)) {
    case PayloadBit::PL_BIT_7:  return default_size;
    case PayloadBit::PL_BIT_16: return default_size + sizeof(uint16_t);
    case PayloadBit::PL_BIT_64: return default_size + sizeof(uint64_t);
    default:                    return default_size;
    }
}

std::size_t WsFrame::copyTo(uint8_t * data, std::size_t size) const
{
    ::memset(data, 0x00, size);

    if (size < 2) {
        return 0;
    }

    *(data + 0) |= ( fin ? 0x80 : 0);
    *(data + 0) |= (rsv1 ? 0x40 : 0);
    *(data + 0) |= (rsv2 ? 0x20 : 0);
    *(data + 0) |= (rsv3 ? 0x10 : 0);
    *(data + 0) |= static_cast<uint8_t>(opcode);
    *(data + 1) |= (mask ? 0x80 : 0);

    // Next index (Written size)
    std::size_t index = 2;

    // Update payload length.
    if (payload_length <= PAYLOAD_7BIT_TYPE_SIZE) {
        if (size < index) {
            return 0;
        }
        *(data + 1) |= payload_length;

    } else if (payload_length <= MAX_UINT16_BYTE_SIZE) {
        if (size < index + sizeof(uint16_t)) {
            return 0;
        }
        *(data + 1) |= 0x7E;
        uint16_t temp = bitwise::toNetwork(static_cast<uint16_t>(payload_length));
        ::memcpy(data + index, &temp, sizeof(uint16_t));
        index += sizeof(uint16_t);

    } else {
        assert(MAX_UINT16_BYTE_SIZE < COMPARE_AND(payload_length) <= MAX_UINT64_BYTE_SIZE);
        if (size < index + sizeof(uint64_t)) {
            return 0;
        }

        *(data + 1) |= 0x7F;
        uint64_t temp = bitwise::toNetwork(static_cast<uint64_t>(payload_length));
        ::memcpy(data + index, &temp, sizeof(uint64_t));
        index += sizeof(uint64_t);
    }

    // Update masking key.
    if (mask) {
        if (size < index + sizeof(uint32_t)) {
            return 0;
        }
        ::memcpy(data + index, &masking_key, sizeof(uint32_t));
        index += sizeof(uint32_t);
    }

    // Update payload data.
    if (payload_length > 0) {
        if (size < index + payload_length) {
            return 0;
        }
        ::memcpy(data + index, payload.data(), payload_length);
        if (mask) {
            updatePayloadData(masking_key, data + index, payload_length);
        }
        index += payload_length;
    }

    return index;
}

std::size_t WsFrame::copyTo(WsBuffer & buffer) const
{
    std::size_t const RESERVE_SIZE = calculateWriteBufferSize();
    if (buffer.size() < RESERVE_SIZE) {
        buffer.resize(RESERVE_SIZE);
    }
    return copyTo(buffer.data(), buffer.size());
}

void WsFrame::setHeader(bool f, bool r1, bool r2, bool r3, OpCode op, uint32_t key) TBAG_NOEXCEPT
{
    fin = f;
    rsv1 = r1;
    rsv2 = r2;
    rsv3 = r3;
    opcode = op;
    mask = (key != 0);
    masking_key = key;
}

void WsFrame::setData(uint8_t const * data, std::size_t size) TBAG_NOEXCEPT
{
    if (data != nullptr && size > 0) {
        if (payload.size() < size) {
            payload.resize(size);
        }
        ::memcpy(&payload[0], data, size);
        payload_length = size;
    } else {
        payload_length = 0;
    }
}

Err WsFrame::build(bool f, bool r1, bool r2, bool r3, OpCode op,
                          uint8_t const * data, std::size_t size,
                          uint32_t key)
{
    setHeader(f, r1, r2, r3, op, key);
    setData(data, size);
    return Err::E_SUCCESS;
}

Err WsFrame::text(std::string const & str, uint32_t key, bool continuation, bool finish)
{
    return build(finish, false, false, false,
                 (continuation ? OpCode::OC_CONTINUATION_FRAME : OpCode::OC_TEXT_FRAME),
                 (uint8_t const *)str.data(), str.size(), key);
}

Err WsFrame::text(std::string const & str, bool continuation, bool finish)
{
    return text(str, 0, continuation, finish);
}

Err WsFrame::binary(WsBuffer const & buffer, uint32_t key, bool continuation, bool finish)
{
    return build(finish, false, false, false,
                 (continuation ? OpCode::OC_CONTINUATION_FRAME : OpCode::OC_BINARY_FRAME),
                 buffer.data(), buffer.size(), key);
}

Err WsFrame::binary(WsBuffer const & buffer, bool continuation, bool finish)
{
    return binary(buffer, 0, continuation, finish);
}

Err WsFrame::close(uint32_t key)
{
    return build(true, false, false, false, OpCode::OC_CONNECTION_CLOSE, nullptr, 0, key);
}

Err WsFrame::close(uint16_t code, std::string const & reason)
{
    code = bitwise::toNetwork(code);
    WsBuffer buffer(sizeof(uint16_t) + reason.size());
    memcpy(&buffer[0], &code, sizeof(uint16_t));
    memcpy(&buffer[sizeof(uint16_t)], &reason[0], reason.size());
    return build(true, false, false, false, OpCode::OC_CONNECTION_CLOSE, buffer.data(), buffer.size());
}

Err WsFrame::close(WsStatusCode code)
{
    return close(getWsStatusCodeNumber(code), std::string(getWsStatusCodeName(code)));
}

uint16_t WsFrame::getStatusCode() const
{
    if (opcode == OpCode::OC_CONNECTION_CLOSE && payload_length >= sizeof(uint16_t)) {
        uint16_t temp = 0;
        ::memcpy(&temp, &payload[0], sizeof(uint16_t));
        return bitwise::toHost(temp);
    }
    return 0U;
}

std::string WsFrame::getReason() const
{
    if (opcode == OpCode::OC_CONNECTION_CLOSE && payload_length > sizeof(uint16_t)) {
        return std::string(&payload[sizeof(uint16_t)], &payload[sizeof(uint16_t)] + payload_length - sizeof(uint16_t));
    }
    return std::string();
}

WsCloseResult WsFrame::getCloseResult() const
{
    return WsCloseResult(getStatusCode(), getReason());
}

Err WsFrame::ping(uint8_t const * data, std::size_t size, uint32_t key)
{
    if (size > PAYLOAD_7BIT_TYPE_SIZE) {
        return Err::E_ILLARGS;
    }
    return build(true, false, false, false, OpCode::OC_DENOTES_PING, data, size, key);
}

Err WsFrame::ping(std::string const & str, uint32_t key)
{
    return ping((uint8_t const *)str.data(), str.size(), key);
}

Err WsFrame::pong(uint8_t const * data, std::size_t size, uint32_t key)
{
    if (size > PAYLOAD_7BIT_TYPE_SIZE) {
        return Err::E_ILLARGS;
    }
    return build(true, false, false, false, OpCode::OC_DENOTES_PONG, data, size, key);
}

Err WsFrame::pong(std::string const & str, uint32_t key)
{
    return pong((uint8_t const *)str.data(), str.size(), key);
}

std::string WsFrame::toDebugString() const
{
    std::stringstream ss;
    ss << "WS[" << (fin?'1':'0') << (rsv1?'1':'0') << (rsv2?'1':'0') << (rsv3?'1':'0')
       << '/' << http::getOpCodeName(opcode) << ']';
    if (mask) {
        ss << " M(" << masking_key << ")";
    }
    if (payload_length > 0) {
        if (opcode == OpCode::OC_TEXT_FRAME) {
            ss << std::string(payload.data(), payload.data() + payload_length);
        } else {
            auto data = std::vector<uint8_t>(payload.data(), payload.data() + payload_length);
            ss << string::convertByteArrayToHexString(data);
        }
    }
    return ss.str();
}

// ---------------
// Static methods.
// ---------------

PayloadBit WsFrame::getPayloadBit(uint8_t payload_length_7bit) TBAG_NOEXCEPT
{
    if (payload_length_7bit <= PAYLOAD_7BIT_TYPE_SIZE) {
        return PayloadBit::PL_BIT_7;
    } else if (payload_length_7bit == PAYLOAD_16BIT_TYPE_SIZE) {
        return PayloadBit::PL_BIT_16;
    } else if (payload_length_7bit == PAYLOAD_64BIT_TYPE_SIZE) {
        return PayloadBit::PL_BIT_64;
    } else {
        TBAG_INACCESSIBLE_BLOCK_ASSERT();
    }
    return PayloadBit::PL_BIT_7;
}

PayloadBit WsFrame::getPayloadBitWithPayloadLength(uint64_t payload_length) TBAG_NOEXCEPT
{
    if (payload_length <= PAYLOAD_7BIT_TYPE_SIZE) {
        return PayloadBit::PL_BIT_7;
    } else if (payload_length <= MAX_UINT16_BYTE_SIZE) {
        return PayloadBit::PL_BIT_16;
    }

    assert(MAX_UINT16_BYTE_SIZE < COMPARE_AND(payload_length) <= MAX_UINT64_BYTE_SIZE);
    return PayloadBit::PL_BIT_64;
}

uint8_t WsFrame::getPayloadDataByteIndex(PayloadBit payload_bit, bool is_mask) TBAG_NOEXCEPT
{
    return getMaskingKeyByteIndex(payload_bit) + (is_mask ? sizeof(uint32_t) : 0);
}

uint8_t WsFrame::getMaskingKeyByteIndex(PayloadBit payload_bit) TBAG_NOEXCEPT
{
    switch (payload_bit) {
    case PayloadBit::PL_BIT_7:  return 2;
    case PayloadBit::PL_BIT_16: return 2 + sizeof(uint16_t);
    case PayloadBit::PL_BIT_64: return 2 + sizeof(uint64_t);
    }
    TBAG_INACCESSIBLE_BLOCK_ASSERT();
    return 0;
}

uint32_t WsFrame::getMaskingKey(uint8_t const * data) TBAG_NOEXCEPT
{
    uint32_t network_32byte_size = 0;
    ::memcpy(&network_32byte_size, data, sizeof(uint32_t));
    return network_32byte_size;
}

std::string WsFrame::getPayloadData(uint32_t mask, std::string const & data)
{
    WsBuffer const INPUT(data.begin(), data.end());
    WsBuffer const OUTPUT = getPayloadData(mask, INPUT);
    return std::string(OUTPUT.begin(), OUTPUT.end());
}

WsFrame::WsBuffer WsFrame::getPayloadData(uint32_t mask, WsBuffer const & data)
{
    return getPayloadData(mask, data.data(), data.size());
}

WsFrame::WsBuffer WsFrame::getPayloadData(uint32_t mask, uint8_t const * data, std::size_t size)
{
    WsBuffer result(data, data + size);
    updatePayloadData(mask, result.data(), result.size());
    return result;
}

void WsFrame::updatePayloadData(uint32_t mask, uint8_t * result, std::size_t size)
{
    static_assert(sizeof(uint32_t) == 4, "Why not?");
    uint8_t const * mask_ptr = reinterpret_cast<uint8_t const *>(&mask);
    for (std::size_t i = 0; i < size; ++i) {
        *(result + i) ^= mask_ptr[i % sizeof(uint32_t)];
    }
}

// ------------------------
// Miscellaneous utilities.
// ------------------------

TBAG_CONSTEXPR static char const * const OP_CODE_NAME_CONTINUE = "CONTINUE";
TBAG_CONSTEXPR static char const * const OP_CODE_NAME_TEXT     = "TEXT";
TBAG_CONSTEXPR static char const * const OP_CODE_NAME_BINARY   = "BINARY";
TBAG_CONSTEXPR static char const * const OP_CODE_NAME_NCF1     = "NCF1";
TBAG_CONSTEXPR static char const * const OP_CODE_NAME_NCF2     = "NCF2";
TBAG_CONSTEXPR static char const * const OP_CODE_NAME_NCF3     = "NCF3";
TBAG_CONSTEXPR static char const * const OP_CODE_NAME_NCF4     = "NCF4";
TBAG_CONSTEXPR static char const * const OP_CODE_NAME_NCF5     = "NCF5";
TBAG_CONSTEXPR static char const * const OP_CODE_NAME_CLOSE    = "CLOSE";
TBAG_CONSTEXPR static char const * const OP_CODE_NAME_PING     = "PING";
TBAG_CONSTEXPR static char const * const OP_CODE_NAME_PONG     = "PONG";
TBAG_CONSTEXPR static char const * const OP_CODE_NAME_CF1      = "CF1";
TBAG_CONSTEXPR static char const * const OP_CODE_NAME_CF2      = "CF2";
TBAG_CONSTEXPR static char const * const OP_CODE_NAME_CF3      = "CF3";
TBAG_CONSTEXPR static char const * const OP_CODE_NAME_CF4      = "CF4";
TBAG_CONSTEXPR static char const * const OP_CODE_NAME_CF5      = "CF5";

char const * getOpCodeName(OpCode code) TBAG_NOEXCEPT
{
    switch (code) {
    case OpCode::OC_CONTINUATION_FRAME          : return OP_CODE_NAME_CONTINUE;
    case OpCode::OC_TEXT_FRAME                  : return OP_CODE_NAME_TEXT;
    case OpCode::OC_BINARY_FRAME                : return OP_CODE_NAME_BINARY;
    case OpCode::OC_RESERVED_NON_CONTROL_FRAME_1: return OP_CODE_NAME_NCF1;
    case OpCode::OC_RESERVED_NON_CONTROL_FRAME_2: return OP_CODE_NAME_NCF2;
    case OpCode::OC_RESERVED_NON_CONTROL_FRAME_3: return OP_CODE_NAME_NCF3;
    case OpCode::OC_RESERVED_NON_CONTROL_FRAME_4: return OP_CODE_NAME_NCF4;
    case OpCode::OC_RESERVED_NON_CONTROL_FRAME_5: return OP_CODE_NAME_NCF5;
    case OpCode::OC_CONNECTION_CLOSE            : return OP_CODE_NAME_CLOSE;
    case OpCode::OC_DENOTES_PING                : return OP_CODE_NAME_PING;
    case OpCode::OC_DENOTES_PONG                : return OP_CODE_NAME_PONG;
    case OpCode::OC_RESERVED_CONTROL_FRAME_1    : return OP_CODE_NAME_CF1;
    case OpCode::OC_RESERVED_CONTROL_FRAME_2    : return OP_CODE_NAME_CF2;
    case OpCode::OC_RESERVED_CONTROL_FRAME_3    : return OP_CODE_NAME_CF3;
    case OpCode::OC_RESERVED_CONTROL_FRAME_4    : return OP_CODE_NAME_CF4;
    case OpCode::OC_RESERVED_CONTROL_FRAME_5    : return OP_CODE_NAME_CF5;
    default: return "UNKNOWN";
    }
}

bool existsWebSocketVersion13(std::string const & versions)
{
    for (auto & ver : fromDelimiterString(versions)) {
        try {
            if (std::stoi(string::trim(ver)) == WEBSOCKET_VERSION_HYBI13) {
                return true;
            }
        } catch (...) {
            // EMPTY.
        }
    }
    return false;
}

std::string getUpgradeWebSocketKey(std::string const & original_key)
{
    encrypt::Sha1Hash sha1_key;
    if (encrypt::encryptSha1(original_key + WEBSOCKET_HANDSHAKE_GUID, sha1_key) == false) {
        return std::string();
    }

    std::vector<uint8_t> const SHA1_BUFFER(sha1_key.begin(), sha1_key.end());
    std::string base64_key;

    if (encrypt::encodeBase64WithBinary(SHA1_BUFFER, base64_key) == false) {
        return std::string();
    }

    return base64_key;
}

std::string generateRandomWebSocketKey()
{
    std::string base64;
    encrypt::encodeBase64(id::Uuid::ver4().toString(), base64);
    return base64;
}

Err updateRequestWebSocket(HttpBuilder & request, std::string const & key)
{
    request.setVersion(1, 1);
    if (request.getMethod().empty()) {
        request.setMethod(HttpMethod::M_GET);
    }

    request.insertIfNotExists(HEADER_CONNECTION, VALUE_UPGRADE);
    request.insertIfNotExists(HEADER_UPGRADE, VALUE_WEBSOCKET);
    request.insertIfNotExists(HEADER_SEC_WEBSOCKET_VERSION, std::to_string(WEBSOCKET_VERSION_HYBI13));

    if (request.existsHeader(HEADER_SEC_WEBSOCKET_KEY) == false) {
        if (key.empty()) {
            return Err::E_KEYGEN;
        }
        request.insertHeader(HEADER_SEC_WEBSOCKET_KEY, key);
    }
    return Err::E_SUCCESS;
}

Err updateResponseWebSocket(HttpParser const & request, HttpBuilder & response)
{
    if (request.getMethodName() != getHttpMethodName(HttpMethod::M_GET)) {
        return Err::E_ILLARGS;
    }
    if (request.existsHeaderValue(HEADER_CONNECTION, VALUE_UPGRADE) == false) {
        return Err::E_ILLARGS;
    }
    if (request.existsHeaderValue(HEADER_UPGRADE, VALUE_WEBSOCKET) == false) {
        return Err::E_ILLARGS;
    }
    if (existsWebSocketVersion13(request.getHeader(HEADER_SEC_WEBSOCKET_VERSION)) == false) {
        return Err::E_VERSION_MISMATCH;
    }

    std::string const UPGRADE_KEY = getUpgradeWebSocketKey(request.getHeader(HEADER_SEC_WEBSOCKET_KEY));
    if (UPGRADE_KEY.empty()) {
        return Err::E_ILLARGS;
    }

    response.setVersion(1, 1);
    response.setStatus(HttpStatus::SC_SWITCHING_PROTOCOLS);
    response.insertIfNotExists(HEADER_UPGRADE, VALUE_WEBSOCKET);
    response.insertIfNotExists(HEADER_CONNECTION, VALUE_UPGRADE);
    response.insertIfNotExists(HEADER_SEC_WEBSOCKET_ACCEPT, UPGRADE_KEY);

    return Err::E_SUCCESS;
}

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

