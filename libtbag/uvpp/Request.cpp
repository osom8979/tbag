/**
 * @file   Request.cpp
 * @brief  Request class implementation.
 * @author zer0
 * @date   2016-12-27
 * @date   2017-02-01 (Move package: libtbag/uv -> libtbag/uvpp)
 */

#include <libtbag/uvpp/Request.hpp>
#include <libtbag/log/Log.hpp>
#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {

Request::Request(ureq type, Handle * owner) : Native(static_cast<utype>(type)), _owner(owner)
{
    if (isRequest() == false) {
        tDLogE("Request::Request({}) type is not request type.", static_cast<int>(type));
        throw std::bad_alloc();
    }
    Parent::cast<uv_req_t>()->data = this; // IMPORTANT!!
}

Request::~Request()
{
    // EMPTY.
}

Err Request::cancel()
{
    // Returns 0 on success, or an error code < 0 on failure.
    //
    // Only cancellation of uv_fs_t, uv_getaddrinfo_t, uv_getnameinfo_t
    // and uv_work_t requests is currently supported.
    //
    // Cancelled requests have their callbacks invoked some time in the future.
    // It’s not safe to free the memory associated with the request until the callback is called.
    //
    // Here is how cancellation is reported to the callback:
    //  - A uv_fs_t request has its req->result field set to UV_ECANCELED.
    //  - A uv_work_t, uv_getaddrinfo_t or c:type:uv_getnameinfo_t request
    //    has its callback invoked with status == UV_ECANCELED.
    int const CODE = ::uv_cancel(Parent::cast<uv_req_t>());
    return convertUvErrorToErrWithLogging("Request::cancel()", CODE);
}

std::size_t Request::getNativeSize() const TBAG_NOEXCEPT
{
    return ::uv_req_size(Parent::cast<uv_req_t>()->type);
}

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

