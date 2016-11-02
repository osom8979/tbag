/**
 * @file   Logger.hpp
 * @brief  Logger class prototype.
 * @author zer0
 * @date   2016-07-15
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOG_DETAILS_LOGGER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOG_DETAILS_LOGGER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/log/details/MsgPacket.hpp>
#include <libtbag/log/details/Severity.hpp>
#include <libtbag/log/sink/Sink.hpp>
#include <libtbag/log/details/Formatter.hpp>

#include <string>
#include <memory>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log     {
namespace details {

/**
 * Logger class prototype.
 *
 * @author zer0
 * @date   2016-07-15
 */
class TBAG_API Logger : public Noncopyable
{
public:
    using SinkType = sink::SinkInterface;
    using SinkPtr  = std::unique_ptr<SinkType>;

private:
    SinkPtr   _sink;
    Severity  _severity;
    Formatter _formatter;

public:
    Logger(SinkType * sink);
    Logger();
    virtual ~Logger();

public:
    void setSeverity(Severity const & severity);
    void setLogLevel(LogLevel level);

public:
    void log(MsgPacket const & msg);

public:
    template <typename ... Args>
    void logf(LogLevel level, std::string const & format, Args && ... args)
    {
        this->log(SinkType::makeMessage(level
                , _formatter.getDefaultPrefix(level) + _formatter.format(format, std::forward<Args>(args) ...)));
    }
};

} // namespace details
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_DETAILS_LOGGER_HPP__

