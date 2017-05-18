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
        tDLogE("__global_uv_prepare_cb__() handle.data is nullptr.");
    } else if (isDeletedAddress(h)) {
        tDLogE("__global_uv_prepare_cb__() handle.data is deleted.");
    } else {
        h->onPrepare();
    }
}

// -----------------------
// Prepare implementation.
// -----------------------

Prepare::Prepare(Loop & loop) : Handle(uhandle::PREPARE)
{
    if (init(loop) != Err::E_SUCCESS) {
        throw std::bad_alloc();
    }
}

Prepare::~Prepare()
{
    // EMPTY.
}

Err Prepare::init(Loop & loop)
{
    int const CODE = ::uv_prepare_init(loop.cast<uv_loop_t>(), Parent::cast<uv_prepare_t>());
    return convertUvErrorToErrWithLogging("Prepare::init()", CODE);
}

Err Prepare::start()
{
    int const CODE = ::uv_prepare_start(Parent::cast<uv_prepare_t>(), __global_uv_prepare_cb__);
    return convertUvErrorToErrWithLogging("Prepare::start()", CODE);
}

Err Prepare::stop()
{
    int const CODE = ::uv_prepare_stop(Parent::cast<uv_prepare_t>());
    return convertUvErrorToErrWithLogging("Prepare::stop()", CODE);
}

void Prepare::onPrepare()
{
    tDLogD("Prepare::onPrepare() called.");
}

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

