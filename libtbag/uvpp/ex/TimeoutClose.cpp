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
    _cancel.store(false);
    return Timer::start(timeout);
}

void TimeoutClose::onTimer()
{
    if (_handle != nullptr) {
        if (_cancel.load() == false) {
            tDLogD("TimeoutClose::onTimer() request handle(@{}[{}]) close.",
                         static_cast<void*>(_handle), _handle->getName());
            _handle->close();
        } else {
            tDLogD("TimeoutClose::onTimer() request cancel.");
        }
    } else {
        tDLogE("TimeoutClose::onTimer() handle is nullptr.");
    }

    if (_auto_close.load()) {
        if (isActive()) {
            stop();
        }
        if (isClosing() == false) {
            close();
        }
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

