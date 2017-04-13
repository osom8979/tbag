/**
 * @file   Logger.cpp
 * @brief  Logger class implementation.
 * @author zer0
 * @date   2017-04-13
 */

#include <libtbag/log/mgr/Logger.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/log/sink/NullSink.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log {
namespace mgr {

Logger::Logger(SinkType * sink) : _sink(sink), _severity(details::INFORMATIONAL_SEVERITY)
{
    // EMPTY.
}

Logger::Logger() : Logger(new ::libtbag::log::sink::NullSink())
{
    // EMPTY.
}

Logger::~Logger()
{
    // EMPTY.
}

void Logger::setSeverity(Severity const & severity)
{
    _severity = severity;
}

void Logger::log(MsgPacket const & msg)
{
    if (static_cast<bool>(_sink) && _severity.isContain(msg.getSeverity())) {
        _sink->write(msg);
    }
}

} // namespace mgr
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

