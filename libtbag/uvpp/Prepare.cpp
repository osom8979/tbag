/**
 * @file   Prepare.cpp
 * @brief  Prepare class implementation.
 * @author zer0
 * @date   2017-05-02
 */

#include <libtbag/uvpp/Prepare.hpp>
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

static void __global_uv_prepare_cb__(uv_prepare_t * handle)
{
    Prepare * h = static_cast<Prepare*>(handle->data);
    if (h == nullptr) {
        __tbag_error("__global_uv_prepare_cb__() handle.data is nullptr.");
    } else if (isDeletedAddress(h)) {
        __tbag_error("__global_uv_prepare_cb__() handle.data is deleted.");
    } else {
        h->onPrepare();
    }
}

// --------------------
// Idle implementation.
// --------------------

Prepare::Prepare() : Handle(uhandle::PREPARE)
{
    // EMPTY.
}

Prepare::Prepare(Loop & loop) : Prepare()
{
    if (init(loop) != uerr::UVPP_SUCCESS) {
        throw std::bad_alloc();
    }
}

Prepare::~Prepare()
{
    // EMPTY.
}

uerr Prepare::init(Loop & loop)
{
    int const CODE = ::uv_prepare_init(loop.cast<uv_loop_t>(), Parent::cast<uv_prepare_t>());
    return getUerr2("Prepare::init()", CODE);
}

uerr Prepare::start()
{
    int const CODE = ::uv_prepare_start(Parent::cast<uv_prepare_t>(), __global_uv_prepare_cb__);
    return getUerr2("Prepare::start()", CODE);
}

uerr Prepare::stop()
{
    int const CODE = ::uv_prepare_stop(Parent::cast<uv_prepare_t>());
    return getUerr2("Prepare::stop()", CODE);
}

void Prepare::onPrepare()
{
    __tbag_debug("Prepare::onPrepare() called.");
}

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

