/**
 * @file   Dns.hpp
 * @brief  Dns class prototype.
 * @author zer0
 * @date   2017-05-03
 *
 * @remarks
 *  libuv provides asynchronous variants of getaddrinfo and getnameinfo.
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVPP_DNS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVPP_DNS_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/uvpp/Request.hpp>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {

// Forward declaration.
class Loop;

/**
 * Dns address info class prototype.
 *
 * @author zer0
 * @date   2017-05-03
 */
class TBAG_API DnsAddrInfo : public Request
{
public:
    using Parent = Request;

public:
    DnsAddrInfo();
    virtual ~DnsAddrInfo();

public:
    /** Loop that started this getaddrinfo request and where completion will be reported. */
    Loop const * getLoop() const;

    /** Pointer to a struct addrinfo containing the result. */
    addrinfo const * getAddrInfo() const;

public:
    /** Asynchronous getaddrinfo(3). */
    uerr getAddrInfo(Loop & loop, std::string const & node, std::string const & service, struct addrinfo const * hints);

    /** Synchronous getaddrinfo(3). */
    uerr getAddrInfo(std::string const & node, std::string const & service, struct addrinfo const * hints);

public:
    /** Free the struct addrinfo. Passing NULL is allowed and is a no-op. */
    static void freeAddrInfo(struct addrinfo * ai);

// Event methods.
public:
    virtual void onGetAddrInfo(int status, struct addrinfo * res);
};

/**
 * Dns name info class prototype.
 *
 * @author zer0
 * @date   2017-05-03
 */
class TBAG_API DnsNameInfo : public Request
{
public:
    using Parent = Request;

public:
    DnsNameInfo();
    virtual ~DnsNameInfo();

public:
    /** Asynchronous getnameinfo(3). */
    uerr getNameInfo(Loop & loop, struct sockaddr const * addr, int flags);

    /** Synchronous getnameinfo(3). */
    uerr getNameInfo(struct sockaddr const * addr, int flags);

// Event methods.
public:
    virtual void onGetNameInfo(int status, std::string const & hostname, std::string const & service);
};

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_DNS_HPP__

