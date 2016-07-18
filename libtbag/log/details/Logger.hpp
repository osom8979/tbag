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
#include <libtbag/log/details/MsgPacket.hpp>
#include <libtbag/log/details/Severity.hpp>
#include <libtbag/log/sink/Sink.hpp>

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
class Logger : public Noncopyable
{
public:
    using CharType = char;
    using SinkType = sink::BaseSinkInterface<CharType>;
    using Message  = typename SinkType::Message;
    using SinkPtr  = std::unique_ptr<SinkType>;
    using String   = std::basic_string<CharType>;

private:
    SinkPtr _sink;
    Severity _severity;

public:
    Logger(SinkType * sink);
    Logger();
    virtual ~Logger();

public:
    void setSeverity(Severity const & severity);
    void setLogLevel(LogLevel level);

public:
    void log(Message const & msg);
    void log(LogLevel level, String const & msg);

public:
    template <typename ... Args>
    void logf(LogLevel level, String const & format, Args && ... args)
    {
        Message msg = SinkType::makeMessage(level);
        msg.format(format, std::forward<Args>(args) ...);
        this->log(msg);
    }

public:
    inline void emergency(String const & msg)
    {   this->log(LogLevel::LEVEL_EMERGENCY, msg);  }
    inline void alert(String const & msg)
    {   this->log(LogLevel::LEVEL_ALERT, msg);      }
    inline void critical(String const & msg)
    {   this->log(LogLevel::LEVEL_CRITICAL, msg);   }
    inline void error(String const & msg)
    {   this->log(LogLevel::LEVEL_ERROR, msg);      }
    inline void warning(String const & msg)
    {   this->log(LogLevel::LEVEL_WARNING, msg);    }
    inline void notice(String const & msg)
    {   this->log(LogLevel::LEVEL_NOTICE, msg);     }
    inline void informational(String const & msg)
    {   this->log(LogLevel::LEVEL_INFO, msg);       }
    inline void debug(String const & msg)
    {   this->log(LogLevel::LEVEL_DEBUG, msg);      }

public:
    friend Logger & operator <<(Logger & logger, Message const & msg);
};

} // namespace details
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_DETAILS_LOGGER_HPP__

