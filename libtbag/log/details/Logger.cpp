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

Logger::Logger() : _sink(new ::libtbag::log::sink::NullSink<>()), _severity(static_cast<int>(LogLevel::LEVEL_INFO))
{
    // EMPTY.
}

Logger::Logger(SinkType * sink) : _sink(sink), _severity(static_cast<int>(LogLevel::LEVEL_INFO))
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

void Logger::log(LogLevel level, std::string const & msg)
{
    if (static_cast<bool>(_sink) && _severity.isContain(DEFAULT_SEVERITY[static_cast<int>(level)].flag) ) {
        _sink->write(MsgPacket(Severity(static_cast<int>(level)), msg));
    }
}

} // namespace details
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

