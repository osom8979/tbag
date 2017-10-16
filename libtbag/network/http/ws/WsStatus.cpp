/**
 * @file   WsStatus.cpp
 * @brief  WsStatus class implementation.
 * @author zer0
 * @date   2017-10-01
 */

#include <libtbag/network/http/ws/WsStatus.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/bitwise/Endian.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {
namespace ws      {

uint16_t getWsStatusCodeNumber(WsStatusCode code) TBAG_NOEXCEPT
{
    switch (code) {
#define _TBAG_XX(num, name, str) case WsStatusCode::WSSC_##name: return num;
    TBAG_WEB_SOCKET_STATUS_CODE_MAP(_TBAG_XX)
#undef _TBAG_XX
    default: return TBAG_UNKNOWN_WEBSOCKET_STATUS_CODE;
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

WsStatusCode getWsStatusCode(uint16_t code) TBAG_NOEXCEPT
{
    switch (code) {
#define _TBAG_XX(num, name, str) case num: return WsStatusCode::WSSC_##name;
    TBAG_WEB_SOCKET_STATUS_CODE_MAP(_TBAG_XX)
#undef _TBAG_XX
    default: return WsStatusCode::WSSC_UNKNOWN;
    }
}

// ------------------------
// WsStatus implementation.
// ------------------------

WsStatus::WsStatus() : code(0), reason()
{
    // EMPTY.
}

WsStatus::WsStatus(uint16_t c) : code(c), reason()
{
    // EMPTY.
}

WsStatus::WsStatus(uint16_t c, std::string const & r) : code(c), reason(r)
{
    // EMPTY.
}

WsStatus::WsStatus(char const * buffer, std::size_t size) : code(0), reason()
{
    parse(buffer, size);
}

WsStatus::WsStatus(util::Buffer const & payload) : code(0), reason()
{
    parse(payload);
}

WsStatus::WsStatus(WsStatusCode s) : code(getWsStatusCodeNumber(s)), reason(getWsStatusCodeReason(s))
{
    // EMPTY.
}

WsStatus::~WsStatus()
{
    // EMPTY.
}

void WsStatus::setWsStatusCode(WsStatusCode s)
{
    code   = ws::getWsStatusCodeNumber(s);
    reason = ws::getWsStatusCodeReason(s);
}

WsStatusCode WsStatus::getWsStatusCode() const TBAG_NOEXCEPT
{
    return ws::getWsStatusCode(code);
}

Err WsStatus::parse(char const * buffer, std::size_t size)
{
    Err const   CODE_RESULT = getStatusCode(buffer, size, &code);
    Err const REASON_RESULT = getReason(buffer, size, &reason);
    if (isSuccess(CODE_RESULT) && isSuccess(REASON_RESULT)) {
        return Err::E_SUCCESS;
    }

    code = 0;
    reason.clear();
    return Err::E_PARSING;
}

Err WsStatus::parse(util::Buffer const & payload)
{
    return parse(payload.data(), payload.size());
}

Err WsStatus::getStatusCode(char const * payload_begin, std::size_t payload_length, uint16_t * result)
{
    if (payload_length < sizeof(uint16_t)) {
        return Err::E_ILLARGS;
    }

    uint16_t temp = 0;
    ::memcpy(&temp, &payload_begin[0], sizeof(temp));
    if (result != nullptr) {
        *result = bitwise::toHost(temp);
    }
    return Err::E_SUCCESS;
}

Err WsStatus::getReason(char const * payload_begin, std::size_t payload_length, std::string * result)
{
    if (payload_length <= sizeof(uint16_t)) {
        return Err::E_ILLARGS;
    }

    if (result != nullptr) {
        result->assign(&payload_begin[sizeof(uint16_t)],
                       &payload_begin[sizeof(uint16_t)] + payload_length - sizeof(uint16_t));
    }
    return Err::E_SUCCESS;
}

std::string WsStatus::toString() const
{
    return ws::getWsStatusCodeName(ws::getWsStatusCode(code));
}

} // namespace ws
} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

