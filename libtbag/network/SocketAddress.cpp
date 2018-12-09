/**
 * @file   SocketAddress.cpp
 * @brief  SocketAddress class implementation.
 * @author zer0
 * @date   2017-06-18
 */

#include <libtbag/network/SocketAddress.hpp>
#include <libtbag/net/Ip.hpp>
#include <libtbag/bitwise/Endian.hpp>
#include <libtbag/uvpp/UvCommon.hpp>
#include <libtbag/uvpp/Dns.hpp>
#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/log/Log.hpp>

#include <cstring>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {

SocketAddress::SocketAddress()
{
    ::memset(&_addr, 0x00, sizeof(_addr));
}

SocketAddress::SocketAddress(struct sockaddr const * addr) : SocketAddress()
{
    if (isFailure(init(addr))) {
        throw std::bad_alloc();
    }
}

SocketAddress::SocketAddress(struct sockaddr_in const * addr) : SocketAddress()
{
    if (isFailure(init(addr))) {
        throw std::bad_alloc();
    }
}

SocketAddress::SocketAddress(struct sockaddr_in6 const * addr) : SocketAddress()
{
    if (isFailure(init(addr))) {
        throw std::bad_alloc();
    }
}

SocketAddress::SocketAddress(SocketAddress const & obj)
{
    (*this) = obj;
}

SocketAddress::SocketAddress(SocketAddress && obj)
{
    (*this) = std::move(obj);
}

SocketAddress::~SocketAddress()
{
    // EMPTY.
}

SocketAddress & SocketAddress::operator =(SocketAddress const & obj)
{
    if (this != &obj) {
        ::memcpy(&_addr, &obj._addr, sizeof(_addr));
    }
    return *this;
}

SocketAddress & SocketAddress::operator =(SocketAddress && obj)
{
    if (this != &obj) {
        std::swap(_addr, obj._addr);
    }
    return *this;
}

Err SocketAddress::init(struct sockaddr const * in)
{
    if (in->sa_family == AF_INET) {
        return init((struct sockaddr_in const *)in);
    } else if (in->sa_family == AF_INET6) {
        return init((struct sockaddr_in6 const *)in);
    }
    return Err::E_UNKNOWN;
}

Err SocketAddress::init(struct sockaddr_in const * in)
{
    ::memset(&_addr, 0x00, sizeof(_addr));
    ::memcpy(&_addr.ipv4, in, sizeof(sockaddr_in));
    return Err::E_SUCCESS;
}

Err SocketAddress::init(struct sockaddr_in6 const * in)
{
    ::memset(&_addr, 0x00, sizeof(_addr));
    ::memcpy(&_addr.ipv6, in, sizeof(sockaddr_in6));
    return Err::E_SUCCESS;
}

Err SocketAddress::initIpv4(std::string const & ip, int port)
{
    return uvpp::initAddress(ip, port, &_addr.ipv4);
}

Err SocketAddress::initIpv6(std::string const & ip, int port)
{
    return uvpp::initAddress(ip, port, &_addr.ipv6);
}

Err SocketAddress::initName(std::string const & host, std::string const & service, int port)
{
    using namespace uvpp;

    Loop loop;
    DnsAddrInfo dns;
    Err const REQUEST_CODE = dns.requestAddrInfoWithSync(loop, host, service);
    if (REQUEST_CODE != Err::E_SUCCESS) {
        return REQUEST_CODE;
    }

    addrinfo const * info = dns.getAddrInfo();
    assert(info != nullptr);
    if (info->ai_addrlen == 0) {
        return Err::E_ENFOUND;
    }

    addrinfo * next = dns.getAddrInfo()->ai_next;
    assert(next != nullptr);

    sockaddr * sa = info->ai_addr;
    assert(sa != nullptr);

    if (sa->sa_family == AF_INET) {
        sockaddr_in const * ipv4_sa = reinterpret_cast<sockaddr_in const *>(sa);
        int const ASSIGN_PORT = service.empty() ? port : uvpp::getPortNumber(ipv4_sa);
        return initIpv4(uvpp::getIpName(ipv4_sa), ASSIGN_PORT);
    } else if (sa->sa_family == AF_INET6) {
        sockaddr_in6 const * ipv6_sa = reinterpret_cast<sockaddr_in6 const *>(sa);
        int const ASSIGN_PORT = service.empty() ? port : uvpp::getPortNumber(ipv6_sa);
        return initIpv6(uvpp::getIpName(ipv6_sa), ASSIGN_PORT);
    }
    return Err::E_UNKNOWN;
}

Err SocketAddress::init(std::string const & host, int port)
{
    if (libtbag::net::isIpv4(host)) {
        return initIpv4(host, port);
    } else if (libtbag::net::isIpv6(host)) {
        return initIpv6(host, port);
    }
    return initName(host, std::string(), port);
}

Err SocketAddress::init(Uri const & uri)
{
    if (uri.isHost() == false) {
        tDLogE("SocketAddress::initUri() Unknown host from uri: {}.", uri.getString());
        return Err::E_ILLARGS;
    }
    if (uri.isPort()) {
        return init(uri.getHost(), uri.getPortNumber());
    }

    if (uri.isSchema() == false) {
        tDLogE("SocketAddress::initUri() Unknown schema from uri: {}.", uri.getString());
        return Err::E_ILLARGS;
    }

    std::string const SERVICE = uri.getSchema();
    std::string const HOST    = uri.getHost();

    assert(SERVICE.empty() == false);
    assert(HOST.empty() == false);

    return initName(HOST, SERVICE);
}

std::string SocketAddress::getIpName() const
{
    if (_addr.ipv4.sin_port == AF_INET) {
        return uvpp::getIpName(&_addr.ipv4);
    } else if (_addr.ipv6.sin6_port == AF_INET6) {
        return uvpp::getIpName(&_addr.ipv6);
    }
    return std::string();
}

int SocketAddress::getPortNumber() const
{
    if (_addr.ipv4.sin_port == AF_INET) {
        return uvpp::getPortNumber(&_addr.ipv4);
    } else if (_addr.ipv6.sin6_port == AF_INET6) {
        return uvpp::getPortNumber(&_addr.ipv6);
    }
    return 0;
}

} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

