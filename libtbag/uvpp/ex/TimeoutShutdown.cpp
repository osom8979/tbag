/**
 * @file   TimeoutShutdown.cpp
 * @brief  TimeoutShutdown class implementation.
 * @author zer0
 * @date   2017-05-09
 */

#include <libtbag/uvpp/ex/TimeoutShutdown.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/uvpp/Loop.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {
namespace ex   {

TimeoutShutdown::TimeoutShutdown(Loop & loop, Stream * stream, bool auto_close)
        : Timer(loop), _stream(stream)
{
    _cancel.store(true);
    _auto_close.store(auto_close);
}

TimeoutShutdown::~TimeoutShutdown()
{
    // EMPTY.
}

uerr TimeoutShutdown::start(uint64_t timeout)
{
    _cancel.store(false);
    return Timer::start(timeout);
}

void TimeoutShutdown::onTimer()
{
    if (_cancel.load() == false && _stream != nullptr) {
        _stream->shutdown(_request);
    }

    if (_auto_close.load()) {
        stop();
        close();
    }
}

void TimeoutShutdown::onClose()
{
    // EMPTY.
}

} // namespace ex
} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

