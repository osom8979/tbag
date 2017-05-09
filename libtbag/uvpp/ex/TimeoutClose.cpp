/**
 * @file   TimeoutClose.cpp
 * @brief  TimeoutClose class implementation.
 * @author zer0
 * @date   2017-05-09
 */

#include <libtbag/uvpp/ex/TimeoutClose.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/uvpp/Loop.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {
namespace ex   {

TimeoutClose::TimeoutClose(Loop & loop, Handle * handle, bool auto_close)
        : Timer(loop), _handle(handle)
{
    _cancel.store(true);
    _auto_close.store(auto_close);
}

TimeoutClose::~TimeoutClose()
{
    // EMPTY.
}

uerr TimeoutClose::start(uint64_t timeout)
{
    bool EXCHANGE = true;
    if (_cancel.compare_exchange_weak(EXCHANGE, false)) {
        return Timer::start(timeout);
    }
    return uerr::UVPP_EBUSY;
}

void TimeoutClose::onTimer()
{
    if (_handle != nullptr) {
        if (_cancel.load() == false) {
            __tbag_debug("TimeoutClose::onTimer() request close handle(@{}[{}]).",
                         static_cast<void*>(_handle), _handle->getName());
            _handle->close();
        } else {
            __tbag_debug("TimeoutClose::onTimer() request cancel.");
        }
    } else {
        __tbag_error("TimeoutClose::onTimer() handle is nullptr.");
    }

    if (_auto_close.load()) {
        stop();
        close();
    }
}

void TimeoutClose::onClose()
{
    // EMPTY.
}

} // namespace ex
} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

