/**
 * @file   Idle.cpp
 * @brief  Idle class implementation.
 * @author zer0
 * @date   2016-12-28
 */

#include <libtbag/uv/Idle.hpp>
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

static void __global_uv_idle_cb__(uv_idle_t * handle)
{
    Idle * h = static_cast<Idle*>(handle->data);
    if (h == nullptr) {
        __tbag_error("__global_uv_idle_cb__() handle data is nullptr.");
        return;
    }
    h->onIdle();
}

// --------------------
// Idle implementation.
// --------------------

Idle::Idle() : Handle(UvHandleType::IDLE)
{
    // EMPTY.
}

Idle::Idle(Loop & loop) : Idle()
{
    if (init(loop) == false) {
        throw std::bad_alloc();
    }
}

Idle::~Idle()
{
    // EMPTY.
}

bool Idle::init(Loop & loop)
{
    int const CODE = ::uv_idle_init(loop.cast<uv_loop_t>(), Parent::cast<uv_idle_t>());
    if (CODE != 0) {
        __tbag_error("Idle::init() error [{}] {}", CODE, getUvErrorName(CODE));
        return false;
    }
    return true;
}

bool Idle::start()
{
    int const CODE = ::uv_idle_start(Parent::cast<uv_idle_t>(), __global_uv_idle_cb__);
    if (CODE != 0) {
        __tbag_error("Idle::start() error [{}] {}", CODE, getUvErrorName(CODE));
        return false;
    }
    return true;
}

bool Idle::stop()
{
    int const CODE = ::uv_idle_stop(Parent::cast<uv_idle_t>());
    if (CODE != 0) {
        __tbag_error("Idle::start() error [{}] {}", CODE, getUvErrorName(CODE));
        return false;
    }
    return true;
}

// --------------
// Event methods.
// --------------

void Idle::onIdle()
{
    __tbag_debug("Idle::onIdle() called.");
}

} // namespace uv

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

