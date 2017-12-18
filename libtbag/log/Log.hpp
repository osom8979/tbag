/**
 * @file   Log.hpp
 * @brief  Log class prototype.
 * @author zer0
 * @date   2016-04-04
 *
 * @remarks
 *  # Push log:         {name,severity,message} -> (Bypass)                -> LoggerManager            @n
 *  # Select logger:    LoggerManager           -> (use name)              -> Logger{severity,message} @n
 *  # Select severity:  Logger                  -> (filtering severity)    -> Logger{severity,message} @n
 *  # Make packet:      Logger                  -> (use severity, message) -> PacketGenerator{message} @n
 *  # Pass to sink:     PacketGenerator         -> (use message)           -> Sink                     @n
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOG_LOG_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOG_LOG_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/Unit.hpp>

#include <libtbag/3rd/tinyxml2/tinyxml2.h>
#include <libtbag/pattern/Singleton.hpp>
#include <libtbag/log/level/Severity.hpp>
#include <libtbag/log/mgr/Logger.hpp>
#include <libtbag/log/msg/PacketGenerator.hpp>
#include <libtbag/log/node/LogXmlNode.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log {

TBAG_CONSTEXPR char const * const TBAG_DEFAULT_LOGGER_NAME = "tbag";

//TBAG_CONSTEXPR bool isAsynchronousLogging() TBAG_NOEXCEPT { return true;  }
//TBAG_CONSTEXPR bool  isMultithreadLogging() TBAG_NOEXCEPT { return false; }

using Severity = ::libtbag::log::level::Severity;
using Logger   = ::libtbag::log::mgr::Logger;
using MakeType = ::libtbag::log::msg::PacketGenerator::MakeType;

Severity const       OFF_SEVERITY = ::libtbag::log::level::OFF_SEVERITY;
Severity const EMERGENCY_SEVERITY = ::libtbag::log::level::EMERGENCY_SEVERITY;
Severity const     ALERT_SEVERITY = ::libtbag::log::level::ALERT_SEVERITY;
Severity const  CRITICAL_SEVERITY = ::libtbag::log::level::CRITICAL_SEVERITY;
Severity const     ERROR_SEVERITY = ::libtbag::log::level::ERROR_SEVERITY;
Severity const   WARNING_SEVERITY = ::libtbag::log::level::WARNING_SEVERITY;
Severity const    NOTICE_SEVERITY = ::libtbag::log::level::NOTICE_SEVERITY;
Severity const      INFO_SEVERITY = ::libtbag::log::level::INFORMATIONAL_SEVERITY;
Severity const     DEBUG_SEVERITY = ::libtbag::log::level::DEBUG_SEVERITY;

TBAG_API Logger * createColorConsoleLogger(std::string const & name, bool auto_flush = false);
TBAG_API Logger * createConsoleLogger(std::string const & name, MakeType type = MakeType::DEFAULT,
                                      bool mutex = true, bool auto_flush = false);
TBAG_API Logger * createFileLogger(std::string const & name, std::string const & path,
                                   MakeType type = MakeType::DEFAULT, bool mutex = true, bool auto_flush = false);
TBAG_API Logger * createRotateFileLogger(std::string const & name, std::string const & path,
                                         std::size_t max_size = MEGA_BYTE_TO_BYTE, MakeType type = MakeType::DEFAULT,
                                         bool mutex = true, bool auto_flush = false);

TBAG_API Logger * createDefaultConsoleLogger(bool auto_flush = false);
TBAG_API Logger * createDefaultColorConsoleLogger(bool auto_flush = false);
TBAG_API Logger * createDefaultFileLogger(std::string const & path, bool auto_flush = false);
TBAG_API Logger * createDefaultRotateFileLogger(std::string const & path,
                                                std::size_t max_size = MEGA_BYTE_TO_BYTE,
                                                bool auto_flush = false);

TBAG_API bool removeLogger(std::string const & name);
TBAG_API bool removeDefaultLogger();

TBAG_API Logger * getLogger(std::string const & name);
TBAG_API Logger * getDefaultLogger();

TBAG_API bool existsLogger(std::string const & name);

TBAG_API void setSeverity(std::string const & name, Severity level);
TBAG_API void setDefaultSeverity(Severity level);

TBAG_API Severity getSeverity(std::string const & name);
TBAG_API Severity getDefaultSeverity();

/**
 * SeverityGuard class.
 *
 * @author zer0
 * @date   2017-05-09
 */
class TBAG_API SeverityGuard : private Noncopyable
{
private:
    std::string _name;
    Severity _save;

public:
    SeverityGuard(std::string const & name = TBAG_DEFAULT_LOGGER_NAME,
                  Severity const & severity = OFF_SEVERITY)
            : _name(name), _save(getSeverity(_name))
    {
        setSeverity(_name, severity);
    }

    ~SeverityGuard()
    {
        setSeverity(_name, _save);
    }
};

template <typename ... Args>
inline void logging(Logger * logger, Severity level, std::string const & format, Args && ... args)
{
    if (logger) {
        logger->logf(level, format, std::forward<Args>(args) ...);
    }
}

// @formatter:off
template <typename ... Args>
inline void emergency(Logger * logger, std::string const & format, Args && ... args)
{ logging(logger, EMERGENCY_SEVERITY, format, std::forward<Args>(args) ...); }
template <typename ... Args>
inline void alert    (Logger * logger, std::string const & format, Args && ... args)
{ logging(logger, ALERT_SEVERITY, format, std::forward<Args>(args) ...); }
template <typename ... Args>
inline void critical (Logger * logger, std::string const & format, Args && ... args)
{ logging(logger, CRITICAL_SEVERITY, format, std::forward<Args>(args) ...); }
template <typename ... Args>
inline void error    (Logger * logger, std::string const & format, Args && ... args)
{ logging(logger, ERROR_SEVERITY, format, std::forward<Args>(args) ...); }
template <typename ... Args>
inline void warning  (Logger * logger, std::string const & format, Args && ... args)
{ logging(logger, WARNING_SEVERITY, format, std::forward<Args>(args) ...); }
template <typename ... Args>
inline void notice   (Logger * logger, std::string const & format, Args && ... args)
{ logging(logger, NOTICE_SEVERITY, format, std::forward<Args>(args) ...); }
template <typename ... Args>
inline void info     (Logger * logger, std::string const & format, Args && ... args)
{ logging(logger, INFO_SEVERITY, format, std::forward<Args>(args) ...); }
template <typename ... Args>
inline void debug    (Logger * logger, std::string const & format, Args && ... args)
{ logging(logger, DEBUG_SEVERITY, format, std::forward<Args>(args) ...); }
// @formatter:on

// @formatter:off
template <typename ... Args>
inline void emergency(std::string const & msg, Args && ... args)
{ emergency(getDefaultLogger(), msg, std::forward<Args>(args) ...); }
template <typename ... Args>
inline void alert    (std::string const & msg, Args && ... args)
{ alert    (getDefaultLogger(), msg, std::forward<Args>(args) ...); }
template <typename ... Args>
inline void critical (std::string const & msg, Args && ... args)
{ critical (getDefaultLogger(), msg, std::forward<Args>(args) ...); }
template <typename ... Args>
inline void error    (std::string const & msg, Args && ... args)
{ error    (getDefaultLogger(), msg, std::forward<Args>(args) ...); }
template <typename ... Args>
inline void warning  (std::string const & msg, Args && ... args)
{ warning  (getDefaultLogger(), msg, std::forward<Args>(args) ...); }
template <typename ... Args>
inline void notice   (std::string const & msg, Args && ... args)
{ notice   (getDefaultLogger(), msg, std::forward<Args>(args) ...); }
template <typename ... Args>
inline void info     (std::string const & msg, Args && ... args)
{ info     (getDefaultLogger(), msg, std::forward<Args>(args) ...); }
template <typename ... Args>
inline void debug    (std::string const & msg, Args && ... args)
{ debug    (getDefaultLogger(), msg, std::forward<Args>(args) ...); }
// @formatter:on

} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#if defined(DISABLE_TBAG_LOG)
# define tLog(name, level, ...)
#else
# define tLog(name, level, ...)  ::libtbag::log::logging(::libtbag::log::getLogger(name), level, __VA_ARGS__)
#endif

#define tLogM(name, ...)  tLog(name, ::libtbag::log::EMERGENCY_SEVERITY, __VA_ARGS__)
#define tLogA(name, ...)  tLog(name, ::libtbag::log::ALERT_SEVERITY    , __VA_ARGS__)
#define tLogC(name, ...)  tLog(name, ::libtbag::log::CRITICAL_SEVERITY , __VA_ARGS__)
#define tLogE(name, ...)  tLog(name, ::libtbag::log::ERROR_SEVERITY    , __VA_ARGS__)
#define tLogW(name, ...)  tLog(name, ::libtbag::log::WARNING_SEVERITY  , __VA_ARGS__)
#define tLogN(name, ...)  tLog(name, ::libtbag::log::NOTICE_SEVERITY   , __VA_ARGS__)
#define tLogI(name, ...)  tLog(name, ::libtbag::log::INFO_SEVERITY     , __VA_ARGS__)
#define tLogD(name, ...)  tLog(name, ::libtbag::log::DEBUG_SEVERITY    , __VA_ARGS__)

#define tDLogM(...)  tLogM(::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define tDLogA(...)  tLogA(::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define tDLogC(...)  tLogC(::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define tDLogE(...)  tLogE(::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define tDLogW(...)  tLogW(::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define tDLogN(...)  tLogN(::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define tDLogI(...)  tLogI(::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define tDLogD(...)  tLogD(::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, __VA_ARGS__)

#if defined(DISABLE_TBAG_LOG)
# define tLogIf(condition, name, level, ...)
#else
# define tLogIf(condition, name, level, ...) do { if (condition) { tLog(name, level, __VA_ARGS__); } } while (false)
#endif

#define tLogIfM(c, name, ...)  tLogIf(c, name, ::libtbag::log::EMERGENCY_SEVERITY, __VA_ARGS__)
#define tLogIfA(c, name, ...)  tLogIf(c, name, ::libtbag::log::ALERT_SEVERITY    , __VA_ARGS__)
#define tLogIfC(c, name, ...)  tLogIf(c, name, ::libtbag::log::CRITICAL_SEVERITY , __VA_ARGS__)
#define tLogIfE(c, name, ...)  tLogIf(c, name, ::libtbag::log::ERROR_SEVERITY    , __VA_ARGS__)
#define tLogIfW(c, name, ...)  tLogIf(c, name, ::libtbag::log::WARNING_SEVERITY  , __VA_ARGS__)
#define tLogIfN(c, name, ...)  tLogIf(c, name, ::libtbag::log::NOTICE_SEVERITY   , __VA_ARGS__)
#define tLogIfI(c, name, ...)  tLogIf(c, name, ::libtbag::log::INFO_SEVERITY     , __VA_ARGS__)
#define tLogIfD(c, name, ...)  tLogIf(c, name, ::libtbag::log::DEBUG_SEVERITY    , __VA_ARGS__)

#define tDLogIfM(c, ...)  tLogIfM(c, ::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define tDLogIfA(c, ...)  tLogIfA(c, ::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define tDLogIfC(c, ...)  tLogIfC(c, ::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define tDLogIfE(c, ...)  tLogIfE(c, ::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define tDLogIfW(c, ...)  tLogIfW(c, ::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define tDLogIfN(c, ...)  tLogIfN(c, ::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define tDLogIfI(c, ...)  tLogIfI(c, ::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define tDLogIfD(c, ...)  tLogIfD(c, ::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, __VA_ARGS__)

#if defined(DISABLE_TBAG_LOG)
# define tLogS(name, level, ...)
#else
# define tLogS(name, level, ...) tLogIf(::libtbag::log::getSeverity(name).isContain(level), name, level, __VA_ARGS__)
#endif

#define tLogSM(name, ...)  tLogS(name, ::libtbag::log::EMERGENCY_SEVERITY, __VA_ARGS__)
#define tLogSA(name, ...)  tLogS(name, ::libtbag::log::ALERT_SEVERITY    , __VA_ARGS__)
#define tLogSC(name, ...)  tLogS(name, ::libtbag::log::CRITICAL_SEVERITY , __VA_ARGS__)
#define tLogSE(name, ...)  tLogS(name, ::libtbag::log::ERROR_SEVERITY    , __VA_ARGS__)
#define tLogSW(name, ...)  tLogS(name, ::libtbag::log::WARNING_SEVERITY  , __VA_ARGS__)
#define tLogSN(name, ...)  tLogS(name, ::libtbag::log::NOTICE_SEVERITY   , __VA_ARGS__)
#define tLogSI(name, ...)  tLogS(name, ::libtbag::log::INFO_SEVERITY     , __VA_ARGS__)
#define tLogSD(name, ...)  tLogS(name, ::libtbag::log::DEBUG_SEVERITY    , __VA_ARGS__)

#define tDLogSM(...)  tLogSM(::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define tDLogSA(...)  tLogSA(::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define tDLogSC(...)  tLogSC(::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define tDLogSE(...)  tLogSE(::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define tDLogSW(...)  tLogSW(::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define tDLogSN(...)  tLogSN(::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define tDLogSI(...)  tLogSI(::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define tDLogSD(...)  tLogSD(::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, __VA_ARGS__)

/**
 * @def tDLogD
 *
 * @warning
 *  Don't use this macros from user level developers.
 */
#if defined(ENABLE_TBAG_LIBRARY_DEBUGGING_LOG) && !defined(NDEBUG)
# include <cstdio>
# include <libtbag/3rd/fmt/format.h>
# define __tbag_debug(...) ::puts(::fmt::format("[DEBUG] " FILE_STRING ":" LINE_STRING " " __VA_ARGS__).c_str())
# define __tbag_error(...) ::puts(::fmt::format("[ERROR] " FILE_STRING ":" LINE_STRING " " __VA_ARGS__).c_str())
#else
# define __tbag_debug(...)
# define __tbag_error(...)
#endif

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_LOG_HPP__

