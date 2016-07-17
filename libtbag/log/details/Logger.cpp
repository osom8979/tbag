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

Logger::Logger() : _sink(new ::libtbag::log::sink::NullSink<>())
{
    // EMPTY.
}

Logger::Logger(SinkType * sink) : _sink(sink)
{
    // EMPTY.
}

Logger::~Logger()
{
    // EMPTY.
}

} // namespace details
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

