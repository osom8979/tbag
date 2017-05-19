/**
 * @file   Async.cpp
 * @brief  Async class implementation.
 * @author zer0
 * @date   2016-12-28
 * @date   2017-02-01 (Move package: libtbag/uv -> libtbag/uvpp)
 */

#include <libtbag/uvpp/Async.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/uvpp/Loop.hpp>

#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {

// --------------------
// Global libuv events.
// --------------------

static void __global_uv_async_cb__(uv_async_t * handle)
{
    Async * async = static_cast<Async*>(handle->data);
    if (async == nullptr) {
        tDLogE("__global_uv_async_cb__() handle.data is nullptr.");
    } else if (isDeletedAddress(async)) {
        tDLogE("__global_uv_async_cb__() handle.data is deleted.");
    } else {
        async->onAsync();
    }
}

// ---------------------
// Async implementation.
// ---------------------

Async::Async(Loop & loop) : Handle(uhandle::ASYNC)
{
    if (init(loop) != Err::E_SUCCESS) {
        throw std::bad_alloc();
    }
}

Async::~Async()
{
    // EMPTY.
}

Err Async::init(Loop & loop)
{
    int const CODE = ::uv_async_init(loop.cast<uv_loop_t>(), Parent::cast<uv_async_t>(), __global_uv_async_cb__);
    return convertUvErrorToErrWithLogging("Async::init()", CODE);
}

Err Async::send()
{
    int const CODE = ::uv_async_send(Parent::cast<uv_async_t>());
    return convertUvErrorToErrWithLogging("Async::send()", CODE);
}

// --------------
// Event methods.
// --------------

void Async::onAsync()
{
    tDLogD("Async::onAsync() called.");
}

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

