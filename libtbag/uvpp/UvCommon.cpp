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

#include <cassert>
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
    std::size_t const SIZE = INET_ADDR_MAX_BYTE_SIZE + 1;
    char name[SIZE] = {0,}; // e.g. 255.255.255.255
    if (::uv_ip4_name(address, name, sizeof(name)) == 0) {
        return std::string(name);
    }
    return std::string();
}

std::string getIpName(sockaddr_in6 const * address)
{
    std::size_t const SIZE = INET6_ADDR_MAX_BYTE_SIZE + 1;
    char name[SIZE] = {0,};
    if (::uv_ip6_name(address, name, sizeof(name)) == 0) {
        return std::string(name);
    }
    return std::string();
}

int getPortNumber(sockaddr const * address)
{
    if (address->sa_family == AF_INET) {
        return getPortNumber((struct sockaddr_in const *)address);
    } else if (address->sa_family == AF_INET6) {
        return getPortNumber((struct sockaddr_in6 const *)address);
    }
    return UNKNOWN_PORT_NUMBER;
}

int getPortNumber(sockaddr_in const * address)
{
    if (address->sin_family == AF_INET) {
        return ntohs(address->sin_port);
    }
    return UNKNOWN_PORT_NUMBER;
}

int getPortNumber(sockaddr_in6 const * address)
{
    if (address->sin6_family == AF_INET6) {
        return ntohs(address->sin6_port);
    }
    return UNKNOWN_PORT_NUMBER;
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

Err convertInetNtop(int family, void const * address, std::string & text)
{
    assert(family == AF_INET || family == AF_INET6);
    std::size_t const SIZE = INET6_ADDR_MAX_BYTE_SIZE + 1;
    char name[SIZE] = {0,};

    // Cross-platform IPv6-capable implementation of inet_ntop(3) and inet_pton(3).
    // On success they return 0. In case of error the target dst pointer is unmodified.
    int const CODE = ::uv_inet_ntop(family, address, name, sizeof(name));
    if (CODE != 0) {
        tDLogE("convertInetNtop() {} error", getUvErrorName(CODE));
    }
    text.assign(name);
    return convertUvErrorToErr(CODE);
}

Err convertInetPton(int family, std::string const & text, void * address)
{
    assert(family == AF_INET || family == AF_INET6);
    // Cross-platform IPv6-capable implementation of inet_ntop(3) and inet_pton(3).
    // On success they return 0. In case of error the target dst pointer is unmodified.

    // When the Family parameter is AF_INET, this buffer should be large enough to hold an IN_ADDR structure.
    // When the Family parameter is AF_INET6, this buffer should be large enough to hold an IN6_ADDR structure.
    int const CODE = ::uv_inet_pton(family, text.c_str(), address);
    if (CODE != 0) {
        tDLogE("convertInetPton() {} error", getUvErrorName(CODE));
    }
    return convertUvErrorToErr(CODE);
}

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

