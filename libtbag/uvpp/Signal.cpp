/**
 * @file   Signal.cpp
 * @brief  Signal class implementation.
 * @author zer0
 * @date   2017-05-02
 */

#include <libtbag/uvpp/Signal.hpp>
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

static void __global_uv_signal_cb__(uv_signal_t * handle, int signum)
{
    Signal * h = static_cast<Signal*>(handle->data);
    if (h == nullptr) {
        __tbag_error("__global_uv_signal_cb__() handle.data is nullptr.");
    } else if (isDeletedAddress(h)) {
        __tbag_error("__global_uv_signal_cb__() handle.data is deleted.");
    } else {
        h->onSignal(signum);
    }
}

// ----------------------
// Signal implementation.
// ----------------------

Signal::Signal() : Handle(uhandle::SIGNAL)
{
    // EMPTY.
}

Signal::Signal(Loop & loop) : Signal()
{
    if (init(loop) != uerr::UVPP_SUCCESS) {
        throw std::bad_alloc();
    }
}

Signal::~Signal()
{
    // EMPTY.
}

int Signal::getSignalNumber() const TBAG_NOEXCEPT
{
    return Parent::cast<uv_signal_t>()->signum;
}

uerr Signal::init(Loop & loop)
{
    int const CODE = ::uv_signal_init(loop.cast<uv_loop_t>(), Parent::cast<uv_signal_t>());
    return getUerr2("Signal::init()", CODE);
}

uerr Signal::start(int signum)
{
    int const CODE = ::uv_signal_start(Parent::cast<uv_signal_t>(), __global_uv_signal_cb__, signum);
    return getUerr2("Signal::start()", CODE);
}

uerr Signal::startOneshot(int signum)
{
    // New in version 1.12.0.
    //int const CODE = ::uv_signal_start_oneshot(Parent::cast<uv_signal_t>(), __global_uv_signal_cb__, signum);
    //return getUerr2("Signal::startOneshot()", CODE);

    return uerr::UVPP_ENOSYS;
}

uerr Signal::stop()
{
    int const CODE = ::uv_signal_stop(Parent::cast<uv_signal_t>());
    return getUerr2("Signal::stop()", CODE);
}

// --------------
// Event methods.
// --------------

void Signal::onSignal(int signum)
{
    __tbag_debug("Signal::onSignal({}) called.", signum);
}

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

