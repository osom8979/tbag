/**
 * @file   Async.cpp
 * @brief  Async class implementation.
 * @author zer0
 * @date   2016-12-28
 */

#include <libtbag/uv/Async.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/uv/Loop.hpp>

#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uv {

// --------------------
// Global libuv events.
// --------------------

static void __global_uv_async_cb__(uv_async_t * handle)
{
    Async * async = static_cast<Async*>(handle->data);
    if (async == nullptr) {
        __tbag_error("__global_uv_async_cb__() handle data is nullptr.");
        return;
    }
    async->onAsync();
}

// ---------------------
// Async implementation.
// ---------------------

Async::Async() : Handle(UvHandleType::ASYNC)
{
    // EMPTY.
}

Async::Async(Loop & loop) : Async()
{
    if (init(loop) == false) {
        throw std::bad_alloc();
    }
}

Async::~Async()
{
    // EMPTY.
}

bool Async::init(Loop & loop)
{
    int const CODE = ::uv_async_init(loop.cast<uv_loop_t>(), Parent::cast<uv_async_t>(), __global_uv_async_cb__);
    if (CODE != 0) {
        __tbag_error("Async::Async() error [{}] {}", CODE, getUvErrorName(CODE));
        return false;
    }
    return true;
}

bool Async::send()
{
    int const CODE = ::uv_async_send(Parent::cast<uv_async_t>());
    if (CODE != 0) {
        __tbag_error("Async::send() error [{}] {}", CODE, getUvErrorName(CODE));
        return false;
    }
    return true;
}

// --------------
// Event methods.
// --------------

void Async::onAsync()
{
    __tbag_debug("Async::onAsync() called.");
}

} // namespace uv

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

