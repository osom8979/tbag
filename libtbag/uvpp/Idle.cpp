/**
 * @file   Idle.cpp
 * @brief  Idle class implementation.
 * @author zer0
 * @date   2016-12-28
 * @date   2017-02-01 (Move package: libtbag/uv -> libtbag/uvpp)
 */

#include <libtbag/uvpp/Idle.hpp>
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

static void __global_uv_idle_cb__(uv_idle_t * handle)
{
    Idle * h = static_cast<Idle*>(handle->data);
    if (h == nullptr) {
        tDLogE("__global_uv_idle_cb__() handle.data is nullptr.");
    } else if (isDeletedAddress(h)) {
        tDLogE("__global_uv_idle_cb__() handle.data is deleted.");
    } else {
        h->onIdle();
    }
}

// --------------------
// Idle implementation.
// --------------------

Idle::Idle(Loop & loop) : Handle(uhandle::IDLE)
{
    if (init(loop) != uerr::UVPP_SUCCESS) {
        throw std::bad_alloc();
    }
}

Idle::~Idle()
{
    // EMPTY.
}

uerr Idle::init(Loop & loop)
{
    int const CODE = ::uv_idle_init(loop.cast<uv_loop_t>(), Parent::cast<uv_idle_t>());
    return getUerr2("Idle::init()", CODE);
}

uerr Idle::start()
{
    int const CODE = ::uv_idle_start(Parent::cast<uv_idle_t>(), __global_uv_idle_cb__);
    return getUerr2("Idle::start()", CODE);
}

uerr Idle::stop()
{
    int const CODE = ::uv_idle_stop(Parent::cast<uv_idle_t>());
    return getUerr2("Idle::stop()", CODE);
}

// --------------
// Event methods.
// --------------

void Idle::onIdle()
{
    tDLogD("Idle::onIdle() called.");
}

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

