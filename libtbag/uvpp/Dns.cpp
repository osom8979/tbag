/**
 * @file   Dns.cpp
 * @brief  Dns class implementation.
 * @author zer0
 * @date   2017-05-03
 */

#include <libtbag/uvpp/Dns.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <cassert>
#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {

// --------------------
// Global libuv events.
// --------------------

static void __global_uv_getaddrinfo_cb__(uv_getaddrinfo_t * req, int status, struct addrinfo * res)
{
    // Callback which will be called with the getaddrinfo request result once complete.
    // In case it was cancelled, status will have a value of UV_ECANCELED.

    DnsAddrInfo * r = static_cast<DnsAddrInfo*>(req->data);
    if (r == nullptr) {
        tDLogE("__global_uv_getaddrinfo_cb__() request.data is nullptr.");
    } else if (isDeletedAddress(r)) {
        tDLogE("__global_uv_getaddrinfo_cb__() request.data is deleted.");
    } else {
        r->onGetAddrInfo(convertUvErrorToErr(status), res);
    }
}

static void __global_uv_getnameinfo_cb__(uv_getnameinfo_t * req, int status, char const * hostname, char const * service)
{
    // Callback which will be called with the getnameinfo request result once complete.
    // In case it was cancelled, status will have a value of UV_ECANCELED.

    DnsNameInfo * r = static_cast<DnsNameInfo*>(req->data);
    if (r == nullptr) {
        tDLogE("__global_uv_getnameinfo_cb__() request.data is nullptr.");
    } else if (isDeletedAddress(r)) {
        tDLogE("__global_uv_getnameinfo_cb__() request.data is deleted.");
    } else {
        std::string hostname_str;
        std::string service_str;
        if (hostname != nullptr) {
            hostname_str.assign(hostname);
        }
        if (service != nullptr) {
            service_str.assign(service);
        }
        r->onGetNameInfo(convertUvErrorToErr(status), hostname_str, service_str);
    }
}

static std::string __exchange_service_name_cb__(std::string const & service)
{
    auto const LOWER_SERVICE_NAME = libtbag::string::lower(service);
    if (LOWER_SERVICE_NAME == "ws") {
        return "http";
    } else if (LOWER_SERVICE_NAME == "wss") {
        return "https";
    }
    return service;
}

// ---------------------------
// DnsAddrInfo implementation.
// ---------------------------

DnsAddrInfo::DnsAddrInfo() : Request(ureq::GETADDRINFO, nullptr)
{
    // EMPTY.
}

DnsAddrInfo::~DnsAddrInfo()
{
    freeAddrInfo();
}

Loop * DnsAddrInfo::getLoop()
{
    uv_loop_t * loop = Parent::cast<uv_getaddrinfo_t>()->loop;
    assert(loop != nullptr && isDeletedAddress(loop) == false);
    return static_cast<Loop*>(loop->data);
}

Loop const * DnsAddrInfo::getLoop() const
{
    uv_loop_t * loop = Parent::cast<uv_getaddrinfo_t>()->loop;
    assert(loop != nullptr && isDeletedAddress(loop) == false);
    return static_cast<Loop*>(loop->data);
}

addrinfo const * DnsAddrInfo::getAddrInfo() const
{
    // Must be freed by the user with uv_freeaddrinfo().

    uv_getaddrinfo_t * native = Parent::cast<uv_getaddrinfo_t>();
    if (native == nullptr) {
        return nullptr;
    } else if (isDeletedAddress(native)) {
        return nullptr;
    }

    return native->addrinfo;
}

int DnsAddrInfo::getAddrInfoSize() const
{
    addrinfo const * addr = getAddrInfo();
    if (addr == nullptr) {
        return 0;
    }
    return static_cast<int>(addr->ai_addrlen);
}

sockaddr const * DnsAddrInfo::findSockAddr(FindFlag flag) const
{
    struct addrinfo const * info = getAddrInfo();
    for (; info != nullptr; info = info->ai_next) {
        assert(info != nullptr);

        sockaddr * sa = info->ai_addr;
        assert(sa != nullptr);

        if (flag == FindFlag::JUST_FIRST) {
            return sa;
        } else if (flag == FindFlag::MOST_IPV4) {
            if (sa->sa_family == AF_INET) {
                return sa;
            }
        } else if (flag == FindFlag::MOST_IPV6) {
            if (sa->sa_family == AF_INET6) {
                return sa;
            }
        } else {
            tDLogE("DnsAddrInfo::findSockAddr() Unknown flag type: {}", static_cast<int>(flag));
            return nullptr; // Unknown flags.
        }
    }

    // Not found.
    return nullptr;
}

sockaddr const * DnsAddrInfo::findFirst() const
{
    return findSockAddr(FindFlag::JUST_FIRST);
}

sockaddr_in const * DnsAddrInfo::findFirstIPv4() const
{
    return reinterpret_cast<sockaddr_in const *>(findSockAddr(FindFlag::MOST_IPV4));
}

sockaddr_in6 const * DnsAddrInfo::findFirstIPv6() const
{
    return reinterpret_cast<sockaddr_in6 const *>(findSockAddr(FindFlag::MOST_IPV6));
}

Err DnsAddrInfo::requestAddrInfo(Loop & loop,
                                std::string const & hostname,
                                std::string const & service,
                                struct addrinfo const * hints)
{
    // Either node or service may be NULL but not both.
    //
    // hints is a pointer to a struct addrinfo with additional address type constraints, or NULL.
    // Consult man -s 3 getaddrinfo for more details.
    //
    // Returns 0 on success or an error code < 0 on failure.
    // If successful, the callback will get called sometime in the future with the lookup result, which is either:
    //  - status == 0, the res argument points to a valid struct addrinfo, or
    //  - status < 0, the res argument is NULL. See the UV_EAI_* constants.
    //
    // Call uv_freeaddrinfo() to free the addrinfo structure.
    //
    // Changed in version 1.3.0:
    // the callback parameter is now allowed to be NULL, in which case the request will run synchronously.

    auto const CHANGED_SERVICE_NAME = __exchange_service_name_cb__(service);
    char const * real_hostname = hostname.c_str();
    char const * real_service  = CHANGED_SERVICE_NAME.empty() ? nullptr : CHANGED_SERVICE_NAME.c_str();

    int const CODE = ::uv_getaddrinfo(loop.cast<uv_loop_t>(), Parent::cast<uv_getaddrinfo_t>(),
                                      __global_uv_getaddrinfo_cb__, real_hostname, real_service, hints);
    return convertUvErrorToErr(CODE);
}

Err DnsAddrInfo::requestAddrInfoWithSync(Loop & loop,
                                         std::string const & hostname,
                                         std::string const & service,
                                         struct addrinfo const * hints)
{
    auto const CHANGED_SERVICE_NAME = __exchange_service_name_cb__(service);
    char const * real_hostname = hostname.c_str();
    char const * real_service  = CHANGED_SERVICE_NAME.empty() ? nullptr : CHANGED_SERVICE_NAME.c_str();

    int const CODE = ::uv_getaddrinfo(loop.cast<uv_loop_t>(), Parent::cast<uv_getaddrinfo_t>(),
                                      nullptr, real_hostname, real_service, hints);
    return convertUvErrorToErr(CODE);
}

void DnsAddrInfo::freeAddrInfo()
{
    struct addrinfo * ai = Parent::cast<uv_getaddrinfo_t>()->addrinfo;
    if (ai != nullptr) {
        freeAddrInfo(ai);
        Parent::cast<uv_getaddrinfo_t>()->addrinfo = nullptr;
    }
}

void DnsAddrInfo::freeAddrInfo(struct addrinfo * ai)
{
    ::uv_freeaddrinfo(ai);
}

void DnsAddrInfo::onGetAddrInfo(Err code, struct addrinfo * res)
{
    tDLogD("DnsAddrInfo::onGetAddrInfo({}) called.", getErrName(code));
}

// ---------------------------
// DnsNameInfo implementation.
// ---------------------------

DnsNameInfo::DnsNameInfo() : Request(ureq::GETNAMEINFO, nullptr)
{
    // EMPTY.
}

DnsNameInfo::~DnsNameInfo()
{
    // EMPTY.
}

Loop * DnsNameInfo::getLoop()
{
    uv_loop_t * loop = Parent::cast<uv_getnameinfo_t>()->loop;
    assert(loop != nullptr && isDeletedAddress(loop) == false);
    return static_cast<Loop*>(loop->data);
}

Loop const * DnsNameInfo::getLoop() const
{
    uv_loop_t * loop = Parent::cast<uv_getnameinfo_t>()->loop;
    assert(loop != nullptr && isDeletedAddress(loop) == false);
    return static_cast<Loop*>(loop->data);
}

std::string DnsNameInfo::getHost() const
{
    uv_getnameinfo_t * native = Parent::cast<uv_getnameinfo_t>();
    if (native == nullptr) {
        return std::string();
    } else if (isDeletedAddress(native)) {
        return std::string();
    }
    return std::string(native->host);
}

std::string DnsNameInfo::getService() const
{
    uv_getnameinfo_t * native = Parent::cast<uv_getnameinfo_t>();
    if (native == nullptr) {
        return std::string();
    } else if (isDeletedAddress(native)) {
        return std::string();
    }
    return std::string(native->service);
}

Err DnsNameInfo::requestNameInfo(Loop & loop, struct sockaddr const * addr, int flags)
{
    // Returns 0 on success or an error code < 0 on failure.
    // If successful, the callback will get called sometime in the future with the lookup result.
    // Consult man -s 3 getnameinfo for more details.
    //
    // Changed in version 1.3.0:
    // the callback parameter is now allowed to be NULL, in which case the request will run synchronously.

    int const CODE = ::uv_getnameinfo(loop.cast<uv_loop_t>(), Parent::cast<uv_getnameinfo_t>(),
                                      __global_uv_getnameinfo_cb__, addr, flags);
    return convertUvErrorToErr(CODE);
}

Err DnsNameInfo::requestNameInfoWithSync(Loop & loop, struct sockaddr const * addr, int flags)
{
    int const CODE = ::uv_getnameinfo(loop.cast<uv_loop_t>(), Parent::cast<uv_getnameinfo_t>(),
                                      nullptr, addr, flags);
    return convertUvErrorToErr(CODE);
}

void DnsNameInfo::onGetNameInfo(Err code, std::string const & hostname, std::string const & service)
{
    tDLogD("DnsNameInfo::onGetNameInfo({}, {}, {}) called.", getErrName(code), hostname, service);
}

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

