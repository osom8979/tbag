/**
 * @file   Tcp.cpp
 * @brief  Tcp class implementation.
 * @author zer0
 * @date   2016-11-04
 */

#include <libtbag/network/socket/Tcp.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/loop/UvEventLoop.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <cassert>
#include <cstring>
#include <cctype>

#include <algorithm>

#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace socket  {

namespace tcp_details {

static std::string getIpName(sockaddr_in * address)
{
    char name[16] = {0,}; // e.g. 255.255.255.255
    if (::uv_ip4_name(address, name, sizeof(name)) == 0) {
        return std::string(name);
    }
    return std::string();
}

static std::string getIpName(sockaddr_in6 * address)
{
    char name[40] = {0,}; // e.g. 2001:0db8:85a3:08d3:1319:8a2e:0370:7334
    if (::uv_ip6_name(address, name, sizeof(name)) == 0) {
        return std::string(name);
    }
    return std::string();
}

static std::string getPeerName(uv_tcp_t const * handle)
{
    sockaddr addr = {0,};
    int length = sizeof(addr);

    if (::uv_tcp_getpeername(handle, &addr, &length) == 0) {
        if (addr.sa_family == AF_INET) {
            return getIpName((sockaddr_in*)&addr);
        } else if (addr.sa_family == AF_INET6) {
            return getIpName((sockaddr_in6*)&addr);
        }
    }
    return std::string();
}

static std::string getSocketName(uv_tcp_t const * handle)
{
    sockaddr addr = {0,};
    int length = sizeof(addr);

    if (::uv_tcp_getsockname(handle, &addr, &length) == 0) {
        if (addr.sa_family == AF_INET) {
            return getIpName((sockaddr_in*)&addr);
        } else if (addr.sa_family == AF_INET6) {
            return getIpName((sockaddr_in6*)&addr);
        }
    }
    return std::string();
}

} // namespace tcp_details

// -------------------
// Tcp implementation.
// -------------------

Tcp::Tcp() : uv::Handle(uv::UvHandleType::TCP)
{
    // EMPTY.
}

Tcp::~Tcp()
{
    // EMPTY.
}

bool Tcp::init(libtbag::loop::UvEventLoop & loop)
{
    int const CODE = ::uv_tcp_init(static_cast<uv_loop_t*>(loop.getNative()), this->castNative<uv_tcp_t>());
    if (CODE != 0) {
        __tbag_error("Tcp::init() error [{}]", CODE);
        return false;
    }
    return true;
}

std::string Tcp::getPeerName() const
{
    return tcp_details::getPeerName(this->castNative<uv_tcp_t>());
}

std::string Tcp::getSocketName() const
{
    return tcp_details::getSocketName(this->castNative<uv_tcp_t>());
}

bool Tcp::isIpv4(std::string const & address)
{
    auto tokens = string::splitTokens(address, ".");
    if (tokens.size() != 4) {
        return false;
    }

    for (auto cursor : tokens) {
        if (!std::all_of(cursor.begin(), cursor.end(), ::isdigit)) {
            return false;
        }
    }
    return true;
}

bool Tcp::isIpv6(std::string const & address)
{
    return !Tcp::isIpv4(address);
}

} // namespace socket
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

