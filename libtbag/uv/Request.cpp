/**
 * @file   Request.cpp
 * @brief  Request class implementation.
 * @author zer0
 * @date   2016-12-27
 */

#include <libtbag/uv/Request.hpp>
#include <libtbag/log/Log.hpp>
#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uv {

Request::Request(UvRequsetType type, Handle * owner) : Native(static_cast<UvType>(type)), _owner(owner)
{
    if (isRequest() == false) {
        __tbag_error("Request::Request({}) type is not request type.", static_cast<int>(type));
        throw std::bad_alloc();
    }
    Parent::castNative<uv_req_t>()->data = this; // IMPORTANT!!
}

Request::~Request()
{
    // EMPTY.
}

bool Request::cancel()
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
    int const CODE = ::uv_cancel(Parent::castNative<uv_req_t>());
    if (CODE != 0) {
        __tbag_error("Request::cancel() error [{}].", CODE);
        return false;
    }
    return true;
}

std::size_t Request::getNativeSize() const TBAG_NOEXCEPT
{
    return ::uv_req_size(Parent::castNative<uv_req_t>()->type);
}

} // namespace uv

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

