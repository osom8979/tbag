/**
 * @file   Logger.cpp
 * @brief  Logger class implementation.
 * @author zer0
 * @date   2017-04-13
 */

#include <libtbag/log/mgr/Logger.hpp>
#include <libtbag/log/sink/NullSink.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log {
namespace mgr {

Logger::Logger() : _sink(new ::libtbag::log::sink::NullSink()), _severity(level::INFORMATIONAL_SEVERITY)
{
    // EMPTY.
}

Logger::Logger(SinkType * sink) : _sink(sink), _severity(level::INFORMATIONAL_SEVERITY)
{
    // EMPTY.
}

Logger::~Logger()
{
    // EMPTY.
}

void Logger::write(Severity const & severity, String const & message)
{
    if (static_cast<bool>(_sink)) {
        auto packet = _sink->makePacket(severity, message);
        if (_severity.isContain(packet.getSeverity())) {
            _sink->safeWrite(packet.toString());
        }
    }
}

void Logger::flush()
{
    if (static_cast<bool>(_sink)) {
        _sink->safeFlush();
    }
}

} // namespace mgr
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

