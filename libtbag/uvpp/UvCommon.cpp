/**
 * @file   UvCommon.cpp
 * @brief  UvCommon class implementation.
 * @author zer0
 * @date   2016-12-27
 * @date   2017-01-03 (Rename: Type -> UvCommon)
 * @date   2017-02-01 (Move package: libtbag/uv -> libtbag/uvpp)
 */

#include <libtbag/uvpp/UvCommon.hpp>
#include <libtbag/log/Log.hpp>
#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {

void initialize()
{
    ::uv_default_loop();
}

void release()
{
    ::uv_loop_close(::uv_default_loop());
}

// @formatter:off
#define _TBAG_XX(name, type) case UvType::name: return true;
#define _TBAG_NX(name, type)
bool isHandle (utype type) { switch (type) { TBAG_UV_HANDLE_MAP(_TBAG_XX, _TBAG_NX, _TBAG_NX) default: return false; } }
bool isRequest(utype type) { switch (type) { TBAG_UV_HANDLE_MAP(_TBAG_NX, _TBAG_XX, _TBAG_NX) default: return false; } }
bool isEtc    (utype type) { switch (type) { TBAG_UV_HANDLE_MAP(_TBAG_NX, _TBAG_NX, _TBAG_XX) default: return false; } }
#undef _TBAG_XX
#undef _TBAG_NX
// @formatter:on

bool isStream(utype type)
{
    return (type == utype::TCP || type == utype::PIPE || type == utype::TTY);
}

// ---------------------
// Native handle helper.
// ---------------------

#ifndef UV_NATIVE_HANDLE_MAP
#define UV_NATIVE_HANDLE_MAP(_TBAG_XX) \
    _TBAG_XX(UV_UNKNOWN_HANDLE) \
    _TBAG_XX(UV_NAMED_PIPE)     \
    _TBAG_XX(UV_TTY)            \
    _TBAG_XX(UV_TCP)            \
    _TBAG_XX(UV_UDP)            \
    _TBAG_XX(UV_PREPARE)        \
    _TBAG_XX(UV_CHECK)          \
    _TBAG_XX(UV_IDLE)           \
    _TBAG_XX(UV_ASYNC)          \
    _TBAG_XX(UV_TIMER)          \
    _TBAG_XX(UV_PROCESS)        \
    _TBAG_XX(UV_FS_EVENT)       \
    _TBAG_XX(UV_POLL)           \
    _TBAG_XX(UV_FS_POLL)        \
    _TBAG_XX(UV_SIGNAL)         \
    _TBAG_XX(UV_FILE)
#endif

char const * getUvNativeHandleName(void const * handle)
{
    switch (static_cast<uv_handle_t const *>(handle)->type) {
#define _TBAG_XX(name) case name: return #name;
    UV_NATIVE_HANDLE_MAP(_TBAG_XX)
#undef _TBAG_XX
    default: return "UNKNOWN_HANDLE";
    }
}

bool isUvNativeHandleType(void const * handle)
{
    switch (static_cast<uv_handle_t const *>(handle)->type) {
#define _TBAG_XX(name) case name: return true;
    UV_NATIVE_HANDLE_MAP(_TBAG_XX)
#undef _TBAG_XX
    default: return false;
    }
}

binf defaultOnAlloc(std::vector<char> & buffer, std::size_t suggested_size)
{
    if (buffer.size() < suggested_size) {
        buffer.resize(suggested_size);
    }

    binf info;
    info.buffer = &buffer[0];
    info.size   =  buffer.size();
    return info;
}

// --------
// Network.
// --------

std::string getIpName(sockaddr const * address)
{
    if (address->sa_family == AF_INET) {
        return getIpName(reinterpret_cast<sockaddr_in const *>(address));
    } else if (address->sa_family == AF_INET6) {
        return getIpName(reinterpret_cast<sockaddr_in6 const *>(address));
    }
    return std::string();
}

std::string getIpName(sockaddr_in const * address)
{
    char name[16] = {0,}; // e.g. 255.255.255.255
    if (::uv_ip4_name(address, name, sizeof(name)) == 0) {
        return std::string(name);
    }
    return std::string();
}

std::string getIpName(sockaddr_in6 const * address)
{
    char name[40] = {0,}; // e.g. 2001:0db8:85a3:08d3:1319:8a2e:0370:7334
    if (::uv_ip6_name(address, name, sizeof(name)) == 0) {
        return std::string(name);
    }
    return std::string();
}

int getPortNumber(sockaddr const * address)
{
    switch (address->sa_family) {
    case AF_INET:  return ntohs(((struct sockaddr_in const *)address)->sin_port);
    case AF_INET6: return ntohs(((struct sockaddr_in6 const *)address)->sin6_port);
    default:       return UNKNOWN_PORT_NUMBER;
    }
}

Err initAddress(std::string const & ip, int port, sockaddr_in * addr)
{
    int const CODE = ::uv_ip4_addr(ip.c_str(), port, addr);
    if (CODE != 0) {
        tDLogE("initAddress({}, {}) ipv4 error [{}] {}", ip, port, CODE, getUvErrorName(CODE));
        return convertUvErrorToErr(CODE);
    }
    return Err::E_SUCCESS;
}

Err initAddress(std::string const & ip, int port, sockaddr_in6 * addr)
{
    int const CODE = ::uv_ip6_addr(ip.c_str(), port, addr);
    if (CODE != 0) {
        tDLogE("initAddress({}, {}) ipv6 error [{}] {}", ip, port, CODE, getUvErrorName(CODE));
        return convertUvErrorToErr(CODE);
    }
    return Err::E_SUCCESS;
}

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

