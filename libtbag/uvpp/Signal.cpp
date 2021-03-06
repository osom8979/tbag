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
        tDLogE("__global_uv_signal_cb__() handle.data is nullptr.");
    } else if (isDeletedAddress(h)) {
        tDLogE("__global_uv_signal_cb__() handle.data is deleted.");
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
    auto const CODE = init(loop);
    if (isFailure(CODE)) {
        throw ErrException(CODE);
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

Err Signal::init(Loop & loop)
{
    int const CODE = ::uv_signal_init(loop.cast<uv_loop_t>(), Parent::cast<uv_signal_t>());
    return convertUvErrorToErrWithLogging("Signal::init()", CODE);
}

Err Signal::start(int signum)
{
    int const CODE = ::uv_signal_start(Parent::cast<uv_signal_t>(), __global_uv_signal_cb__, signum);
    return convertUvErrorToErrWithLogging("Signal::start()", CODE);
}

Err Signal::startOneshot(int signum)
{
#if (UV_VERSION_MAJOR >= 1) && (UV_VERSION_MINOR >= 12)
    // New in version 1.12.0.
    int const CODE = ::uv_signal_start_oneshot(Parent::cast<uv_signal_t>(), __global_uv_signal_cb__, signum);
    return convertUvErrorToErrWithLogging("Signal::startOneshot()", CODE);
#else
    return E_ENOSYS;
#endif
}

Err Signal::stop()
{
    int const CODE = ::uv_signal_stop(Parent::cast<uv_signal_t>());
    return convertUvErrorToErrWithLogging("Signal::stop()", CODE);
}

// --------------
// Event methods.
// --------------

void Signal::onSignal(int signum)
{
    tDLogD("Signal::onSignal({}) called.", signum);
}

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

