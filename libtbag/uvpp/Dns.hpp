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
    Loop * getLoop();
    Loop const * getLoop() const;

    /** Pointer to a struct addrinfo containing the result. */
    addrinfo const * getAddrInfo() const;

public:
    /**
     * Asynchronous getaddrinfo(3).
     *
     * @param[in] loop
     *      Loop object.
     * @param[in] node
     *      DNS name or IP address (e.g. "www.example.com")
     * @param[in] service
     *      Service name or Port number. (e.g. "http")
     * @param[in] hints
     *      Hint of address information.
     */
    Err requestAddrInfo(Loop & loop,
                         std::string const & hostname,
                         std::string const & service = "",
                         struct addrinfo const * hints = nullptr);

    /** Synchronous getaddrinfo(3). */
    Err requestAddrInfoWithSync(Loop & loop,
                                std::string const & hostname,
                                std::string const & service = "",
                                struct addrinfo const * hints = nullptr);

    /** Free the struct addrinfo. */
    void freeAddrInfo();

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
    /** Loop that started this getnameinfo request and where completion will be reported. */
    Loop * getLoop();
    Loop const * getLoop() const;

    /** Char array containing the resulting host. */
    std::string getHost() const;

    /** Char array containing the resulting service. */
    std::string getService() const;

public:
    /** Asynchronous getnameinfo(3). */
    Err requestNameInfo(Loop & loop, struct sockaddr const * addr, int flags);

    /** Synchronous getnameinfo(3). */
    Err requestNameInfoWithSync(Loop & loop, struct sockaddr const * addr, int flags);

// Event methods.
public:
    virtual void onGetNameInfo(int status, std::string const & hostname, std::string const & service);
};

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_DNS_HPP__

