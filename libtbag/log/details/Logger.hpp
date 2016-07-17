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
#include <libtbag/Noncopyable.hpp>
#include <libtbag/log/details/Severity.hpp>
#include <libtbag/log/sink/Sink.hpp>

#include <string>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log     {
namespace details {

enum class LogLevel : int
{
    LEVEL_EMERGENCY = ::libtbag::log::details::LOG_SEVERITY_EMERGENCY,
    LEVEL_ALERT     = ::libtbag::log::details::LOG_SEVERITY_ALERT,
    LEVEL_CRITICAL  = ::libtbag::log::details::LOG_SEVERITY_CRITICAL,
    LEVEL_ERROR     = ::libtbag::log::details::LOG_SEVERITY_ERROR,
    LEVEL_WARNING   = ::libtbag::log::details::LOG_SEVERITY_WARNING,
    LEVEL_NOTICE    = ::libtbag::log::details::LOG_SEVERITY_NOTICE,
    LEVEL_INFO      = ::libtbag::log::details::LOG_SEVERITY_INFORMATIONAL,
    LEVEL_DEBUG     = ::libtbag::log::details::LOG_SEVERITY_DEBUG,
    LEVEL_OFF       = ::libtbag::log::details::LOG_SEVERITY_OFF,
};

/**
 * Logger class prototype.
 *
 * @author zer0
 * @date   2016-07-15
 */
class Logger : public Noncopyable
{
public:
    using SinkType = sink::BaseSinkInterface<char>;
    using SinkPtr  = std::unique_ptr<SinkType>;

private:
    SinkPtr _sink;
    Severity _severity;

public:
    Logger();
    Logger(SinkType * sink);
    virtual ~Logger();

public:
    void setSeverity(Severity const & severity);
    void setLogLevel(LogLevel level);

public:
    void log(LogLevel level, std::string const & msg);

public:
    inline void emergency(std::string const & msg)
    { this->log(LogLevel::LEVEL_EMERGENCY, msg);    }
    inline void alert(std::string const & msg)
    {   this->log(LogLevel::LEVEL_ALERT, msg);      }
    inline void critical(std::string const & msg)
    {   this->log(LogLevel::LEVEL_CRITICAL, msg);   }
    inline void error(std::string const & msg)
    {   this->log(LogLevel::LEVEL_ERROR, msg);      }
    inline void warning(std::string const & msg)
    {   this->log(LogLevel::LEVEL_WARNING, msg);    }
    inline void notice(std::string const & msg)
    {   this->log(LogLevel::LEVEL_NOTICE, msg);     }
    inline void informational(std::string const & msg)
    {   this->log(LogLevel::LEVEL_INFO, msg);       }
    inline void debug(std::string const & msg)
    {   this->log(LogLevel::LEVEL_DEBUG, msg);      }
};

} // namespace details
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_DETAILS_LOGGER_HPP__

