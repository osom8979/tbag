/**
 * @file   TimeoutToShutdown.cpp
 * @brief  TimeoutToShutdown class implementation.
 * @author zer0
 * @date   2017-05-05
 */

#include <libtbag/uvpp/ex/TimeoutToShutdown.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/uvpp/Loop.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {
namespace ex   {

TimeoutToShutdown::TimeoutToShutdown(Loop & loop, Stream * stream, bool auto_close)
        : Timer(loop), _stream(stream)
{
    _cancel.store(true);
    _auto_close.store(auto_close);
}

TimeoutToShutdown::~TimeoutToShutdown()
{
    // EMPTY.
}

uerr TimeoutToShutdown::start(uint64_t timeout)
{
    bool EXCHANGE = true;
    if (_cancel.compare_exchange_weak(EXCHANGE, false)) {
        return Timer::start(timeout);
    }
    return uerr::UVPP_EBUSY;
}

void TimeoutToShutdown::onTimer()
{
    if (_cancel.load() == false && _stream != nullptr) {
        _stream->shutdown(_request);
    }

    if (_auto_close.load()) {
        stop();
        close();
    }
}

void TimeoutToShutdown::onClose()
{
    // EMPTY.
}

} // namespace ex
} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

