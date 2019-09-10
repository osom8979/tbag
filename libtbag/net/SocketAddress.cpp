/**
 * @file   SocketAddress.cpp
 * @brief  SocketAddress class implementation.
 * @author zer0
 * @date   2017-06-18
 * @date   2019-01-19 (Move: libtbag/network -> libtbag/net)
 */

#include <libtbag/net/SocketAddress.hpp>
#include <libtbag/net/Ip.hpp>
#include <libtbag/bitwise/Endian.hpp>
#include <libtbag/uvpp/UvCommon.hpp>
#include <libtbag/uvpp/UvUtils.hpp>
#include <libtbag/uvpp/Dns.hpp>
#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/log/Log.hpp>

#include <cstring>
#include <bitset>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace net {

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

SocketAddress::SocketAddress(SocketAddress && obj) TBAG_NOEXCEPT
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

SocketAddress & SocketAddress::operator =(SocketAddress && obj) TBAG_NOEXCEPT
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
    return E_UNKNOWN;
}

Err SocketAddress::init(struct sockaddr_in const * in)
{
    ::memset(&_addr, 0x00, sizeof(_addr));
    ::memcpy(&_addr.ipv4, in, sizeof(sockaddr_in));
    return E_SUCCESS;
}

Err SocketAddress::init(struct sockaddr_in6 const * in)
{
    ::memset(&_addr, 0x00, sizeof(_addr));
    ::memcpy(&_addr.ipv6, in, sizeof(sockaddr_in6));
    return E_SUCCESS;
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
    if (REQUEST_CODE != E_SUCCESS) {
        return REQUEST_CODE;
    }

    addrinfo const * info = dns.getAddrInfo();
    assert(info != nullptr);
    if (info->ai_addrlen == 0) {
        return E_NFOUND;
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
    return E_UNKNOWN;
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
    if (!uri.isHost()) {
        tDLogE("SocketAddress::initUri() Unknown host from uri: {}.", uri.getString());
        return E_ILLARGS;
    }
    if (uri.isPort()) {
        return init(uri.getHost(), uri.getPortNumber());
    }

    if (!uri.isSchema()) {
        tDLogE("SocketAddress::initUri() Unknown schema from uri: {}.", uri.getString());
        return E_ILLARGS;
    }

    std::string const SERVICE = uri.getSchema();
    std::string const HOST    = uri.getHost();

    assert(!SERVICE.empty());
    assert(!HOST.empty());

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

// -----------------------
// Miscellaneous utilities
// -----------------------

Err findHostNameOfConnectedInterfaceByIpAddress(std::string const & ip, int flags, std::string & result_host)
{
    libtbag::net::SocketAddress client_socket_address;
    auto const code1 = client_socket_address.init(ip, 0);
    if (isFailure(code1)) {
        return code1;
    }

    auto const CLIENT_FAMILY = client_socket_address.getCommon()->sa_family;

    libtbag::uvpp::Loop loop;
    libtbag::uvpp::DnsNameInfo name;

    for (auto const & interface : libtbag::uvpp::getInterfaceAddresses()) {
        auto const addr_family = interface.address.common.sa_family;
        auto const mask_family = interface.netmask.common.sa_family;

        if (CLIENT_FAMILY == AF_INET && addr_family == AF_INET && mask_family == AF_INET) {
            auto const mask_addr = interface.netmask.in4.sin_addr.s_addr;
            auto const addr_addr = interface.address.in4.sin_addr.s_addr;
            auto const client_addr = client_socket_address.getIpv4()->sin_addr.s_addr;

            if ((mask_addr & addr_addr) == (mask_addr & client_addr)) {
                auto const code2 = name.requestNameInfoWithSync(loop, &interface.address.common, flags);
                if (isSuccess(code2)) {
                    result_host = name.getHost();
                    return E_SUCCESS;
                }
            }
        } else if (CLIENT_FAMILY == AF_INET6 && addr_family == AF_INET6 && mask_family == AF_INET6) {
            using Ipv6AddressBitset = std::bitset<128/*IP6 address is 128bit*/>;
            Ipv6AddressBitset mask_addr(interface.netmask.in6.sin6_addr.s6_addr);
            Ipv6AddressBitset addr_addr(interface.address.in6.sin6_addr.s6_addr);
            Ipv6AddressBitset client_addr(client_socket_address.getIpv6()->sin6_addr.s6_addr);

            if ((mask_addr & addr_addr) == (mask_addr & client_addr)) {
                auto const code3 = name.requestNameInfoWithSync(loop, &interface.address.common, flags);
                if (isSuccess(code3)) {
                    result_host = name.getHost();
                    return E_SUCCESS;
                }
            }
        }
    }

    return E_NFOUND;
}

std::string findHostNameOfConnectedInterfaceByIpAddress(std::string const & ip, int flags)
{
    std::string result_host;
    auto const code = findHostNameOfConnectedInterfaceByIpAddress(ip, flags, result_host);
    if (isFailure(code)) {
        return {};
    }
    return result_host;
}

std::string findHostNameOfConnectedInterfaceByIpAddress(std::string const & ip)
{
    return findHostNameOfConnectedInterfaceByIpAddress(ip, NI_NUMERICHOST);
}

} // namespace net

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

