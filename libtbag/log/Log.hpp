/**
 * @file   Log.hpp
 * @brief  Log class prototype.
 * @author zer0
 * @date   2016-04-04
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
#include <libtbag/pattern/Singleton.hpp>
#include <libtbag/log/details/Severity.hpp>
#include <libtbag/log/details/Logger.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log {

TBAG_CONSTEXPR char const * const TBAG_DEFAULT_LOGGER_NAME = "__tbag_default_logger__";

//TBAG_CONSTEXPR bool isAsynchronousLogging() TBAG_NOEXCEPT { return true;  }
//TBAG_CONSTEXPR bool  isMultithreadLogging() TBAG_NOEXCEPT { return false; }

using Logger   = ::libtbag::log::details::Logger;
using LogLevel = ::libtbag::log::details::LogLevel;

TBAG_EXPORTS Logger * createConsoleLogger(std::string const & name, bool auto_flush = false);
TBAG_EXPORTS Logger * createColorConsoleLogger(std::string const & name, bool auto_flush = false);
TBAG_EXPORTS Logger * createFileLogger(std::string const & name, std::string const & path, bool auto_flush = false);

TBAG_EXPORTS Logger * createDefaultConsoleLogger(bool auto_flush = false);
TBAG_EXPORTS Logger * createDefaultColorConsoleLogger(bool auto_flush = false);
TBAG_EXPORTS Logger * createDefaultFileLogger(std::string const & path, bool auto_flush = false);

TBAG_EXPORTS void removeLogger(std::string const & name);
TBAG_EXPORTS void removeDefaultLogger();

TBAG_EXPORTS Logger * getLogger(std::string const & name);
TBAG_EXPORTS Logger * getDefaultLogger();

TBAG_EXPORTS void setLevel(std::string const & name, LogLevel level);
TBAG_EXPORTS void setLevel(std::string const & name, int level);
TBAG_EXPORTS void setDefaultLevel(LogLevel level);

template <typename ... Args>
inline void logging(Logger * logger, LogLevel level, std::string const & format, Args && ... args)
{
    if (logger) {
        logger->logf(level, format, std::forward<Args>(args) ...);
    }
}

// @formatter:off
template <typename ... Args>
inline void emergency(Logger * logger, std::string const & format, Args && ... args) { logging(logger, LogLevel::LEVEL_EMERGENCY, format, std::forward<Args>(args) ...); }
template <typename ... Args>
inline void alert    (Logger * logger, std::string const & format, Args && ... args) { logging(logger, LogLevel::LEVEL_ALERT    , format, std::forward<Args>(args) ...); }
template <typename ... Args>
inline void critical (Logger * logger, std::string const & format, Args && ... args) { logging(logger, LogLevel::LEVEL_CRITICAL , format, std::forward<Args>(args) ...); }
template <typename ... Args>
inline void error    (Logger * logger, std::string const & format, Args && ... args) { logging(logger, LogLevel::LEVEL_ERROR    , format, std::forward<Args>(args) ...); }
template <typename ... Args>
inline void warning  (Logger * logger, std::string const & format, Args && ... args) { logging(logger, LogLevel::LEVEL_WARNING  , format, std::forward<Args>(args) ...); }
template <typename ... Args>
inline void notice   (Logger * logger, std::string const & format, Args && ... args) { logging(logger, LogLevel::LEVEL_NOTICE   , format, std::forward<Args>(args) ...); }
template <typename ... Args>
inline void info     (Logger * logger, std::string const & format, Args && ... args) { logging(logger, LogLevel::LEVEL_INFO     , format, std::forward<Args>(args) ...); }
template <typename ... Args>
inline void debug    (Logger * logger, std::string const & format, Args && ... args) { logging(logger, LogLevel::LEVEL_DEBUG    , format, std::forward<Args>(args) ...); }
// @formatter:on

// @formatter:off
template <typename ... Args>
inline void emergency(std::string const & msg, Args && ... args) { emergency(getDefaultLogger(), msg, std::forward<Args>(args) ...); }
template <typename ... Args>
inline void alert    (std::string const & msg, Args && ... args) { alert    (getDefaultLogger(), msg, std::forward<Args>(args) ...); }
template <typename ... Args>
inline void critical (std::string const & msg, Args && ... args) { critical (getDefaultLogger(), msg, std::forward<Args>(args) ...); }
template <typename ... Args>
inline void error    (std::string const & msg, Args && ... args) { error    (getDefaultLogger(), msg, std::forward<Args>(args) ...); }
template <typename ... Args>
inline void warning  (std::string const & msg, Args && ... args) { warning  (getDefaultLogger(), msg, std::forward<Args>(args) ...); }
template <typename ... Args>
inline void notice   (std::string const & msg, Args && ... args) { notice   (getDefaultLogger(), msg, std::forward<Args>(args) ...); }
template <typename ... Args>
inline void info     (std::string const & msg, Args && ... args) { info     (getDefaultLogger(), msg, std::forward<Args>(args) ...); }
template <typename ... Args>
inline void debug    (std::string const & msg, Args && ... args) { debug    (getDefaultLogger(), msg, std::forward<Args>(args) ...); }
// @formatter:on

} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#if defined(DISABLE_TBAG_LOG)
# define tLog(name, level, msg)
# define tLogF(name, level, msg, ...)
#else
# define tLog(name, level, msg)        ::libtbag::log::logging(::libtbag::log::getLogger(name), level, msg)
# define tLogF(name, level, msg, ...)  ::libtbag::log::logging(::libtbag::log::getLogger(name), level, msg, __VA_ARGS__)
#endif

#define tLogE(name, msg)  tLog(name, ::libtbag::log::LogLevel::LEVEL_EMERGENCY, msg)
#define tLogA(name, msg)  tLog(name, ::libtbag::log::LogLevel::LEVEL_ALERT    , msg)
#define tLogC(name, msg)  tLog(name, ::libtbag::log::LogLevel::LEVEL_CRITICAL , msg)
#define tLogR(name, msg)  tLog(name, ::libtbag::log::LogLevel::LEVEL_ERROR    , msg)
#define tLogW(name, msg)  tLog(name, ::libtbag::log::LogLevel::LEVEL_WARNING  , msg)
#define tLogN(name, msg)  tLog(name, ::libtbag::log::LogLevel::LEVEL_NOTICE   , msg)
#define tLogI(name, msg)  tLog(name, ::libtbag::log::LogLevel::LEVEL_INFO     , msg)
#define tLogD(name, msg)  tLog(name, ::libtbag::log::LogLevel::LEVEL_DEBUG    , msg)

#define tLogEF(name, msg, ...)  tLogF(name, ::libtbag::log::LogLevel::LEVEL_EMERGENCY, msg, __VA_ARGS__)
#define tLogAF(name, msg, ...)  tLogF(name, ::libtbag::log::LogLevel::LEVEL_ALERT    , msg, __VA_ARGS__)
#define tLogCF(name, msg, ...)  tLogF(name, ::libtbag::log::LogLevel::LEVEL_CRITICAL , msg, __VA_ARGS__)
#define tLogRF(name, msg, ...)  tLogF(name, ::libtbag::log::LogLevel::LEVEL_ERROR    , msg, __VA_ARGS__)
#define tLogWF(name, msg, ...)  tLogF(name, ::libtbag::log::LogLevel::LEVEL_WARNING  , msg, __VA_ARGS__)
#define tLogNF(name, msg, ...)  tLogF(name, ::libtbag::log::LogLevel::LEVEL_NOTICE   , msg, __VA_ARGS__)
#define tLogIF(name, msg, ...)  tLogF(name, ::libtbag::log::LogLevel::LEVEL_INFO     , msg, __VA_ARGS__)
#define tLogDF(name, msg, ...)  tLogF(name, ::libtbag::log::LogLevel::LEVEL_DEBUG    , msg, __VA_ARGS__)

#define tDLogE(msg) tLogE(::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, msg)
#define tDLogA(msg) tLogA(::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, msg)
#define tDLogC(msg) tLogC(::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, msg)
#define tDLogR(msg) tLogR(::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, msg)
#define tDLogW(msg) tLogW(::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, msg)
#define tDLogN(msg) tLogN(::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, msg)
#define tDLogI(msg) tLogI(::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, msg)
#define tDLogD(msg) tLogD(::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, msg)

#define tDLogEF(msg, ...)  tLogEF(::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, msg, __VA_ARGS__)
#define tDLogAF(msg, ...)  tLogAF(::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, msg, __VA_ARGS__)
#define tDLogCF(msg, ...)  tLogCF(::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, msg, __VA_ARGS__)
#define tDLogRF(msg, ...)  tLogRF(::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, msg, __VA_ARGS__)
#define tDLogWF(msg, ...)  tLogWF(::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, msg, __VA_ARGS__)
#define tDLogNF(msg, ...)  tLogNF(::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, msg, __VA_ARGS__)
#define tDLogIF(msg, ...)  tLogIF(::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, msg, __VA_ARGS__)
#define tDLogDF(msg, ...)  tLogDF(::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, msg, __VA_ARGS__)

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_LOG_HPP__

