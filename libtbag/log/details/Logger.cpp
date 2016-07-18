/**
 * @file   Logger.cpp
 * @brief  Logger class implementation.
 * @author zer0
 * @date   2016-07-15
 */

#include <libtbag/log/details/Logger.hpp>
#include <libtbag/log/sink/NullSink.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log     {
namespace details {

Logger::Logger(SinkType * sink) : _sink(sink), _severity(LogLevel::LEVEL_INFO)
{
    // EMPTY.
}

Logger::Logger() : Logger(new ::libtbag::log::sink::NullSink<>())
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

void Logger::setLogLevel(LogLevel level)
{
    _severity.setFlag(DEFAULT_SEVERITY[static_cast<int>(level)].flag);
    _severity.setText(DEFAULT_SEVERITY[static_cast<int>(level)].text);
}

void Logger::log(Message const & msg)
{
    if (static_cast<bool>(_sink) && _severity.isContain(msg.getSeverity().getFlag()) ) {
        _sink->write(msg);
    }
}

} // namespace details
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

