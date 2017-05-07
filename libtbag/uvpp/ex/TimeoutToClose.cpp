/**
 * @file   TimeoutToClose.cpp
 * @brief  TimeoutToClose class implementation.
 * @author zer0
 * @date   2017-05-05
 */

#include <libtbag/uvpp/ex/TimeoutToClose.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/uvpp/Loop.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {
namespace ex   {

TimeoutToClose::TimeoutToClose(Loop & loop, Handle * handle, bool auto_close)
        : Timer(loop), _handle(handle)
{
    _cancel.store(true);
    _auto_close.store(auto_close);
}

TimeoutToClose::~TimeoutToClose()
{
    // EMPTY.
}

uerr TimeoutToClose::start(uint64_t timeout)
{
    bool EXCHANGE = true;
    if (_cancel.compare_exchange_weak(EXCHANGE, false)) {
        return Timer::start(timeout);
    }
    return uerr::UVPP_EBUSY;
}

void TimeoutToClose::onTimer()
{
    if (_cancel.load() == false && _handle != nullptr) {
        _handle->close();
    }

    if (_auto_close.load()) {
        stop();
        close();
    }
}

void TimeoutToClose::onClose()
{
    // EMPTY.
}

} // namespace ex
} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

