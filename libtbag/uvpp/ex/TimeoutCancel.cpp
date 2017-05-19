/**
 * @file   TimeoutCancel.cpp
 * @brief  TimeoutCancel class implementation.
 * @author zer0
 * @date   2017-05-19
 */

#include <libtbag/uvpp/ex/TimeoutCancel.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/uvpp/Loop.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {
namespace ex   {

TimeoutCancel::TimeoutCancel(Loop & loop, Request * request, bool auto_close)
        : Timer(loop), _request(request)
{
    _cancel.store(true);
    _auto_close.store(auto_close);
}

TimeoutCancel::~TimeoutCancel()
{
    // EMPTY.
}

Err TimeoutCancel::start(uint64_t timeout)
{
    _cancel.store(false);
    return Timer::start(timeout);
}

void TimeoutCancel::onTimer()
{
    if (_request != nullptr) {
        if (_cancel.load() == false) {
            tDLogD("TimeoutCancel::onTimer() request (@{}]) cancel.",
                   static_cast<void*>(_request));
            _request->cancel();
        } else {
            tDLogD("TimeoutCancel::onTimer() 'request cancel' cancel.");
        }
    } else {
        tDLogE("TimeoutCancel::onTimer() request is nullptr.");
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

void TimeoutCancel::onClose()
{
    // EMPTY.
}

} // namespace ex
} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

