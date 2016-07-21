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
#include <libtbag/Noncopyable.hpp>
#include <libtbag/pattern/Singleton.hpp>
#include <libtbag/log/details/Severity.hpp>
#include <libtbag/log/details/Logger.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log {

constexpr char const * const TBAG_DEFAULT_LOGGER_NAME = "__tbag_default_logger__";

//constexpr bool isAsynchronousLogging() noexcept { return true;  }
//constexpr bool  isMultithreadLogging() noexcept { return false; }

using Logger   = ::libtbag::log::details::Logger;
using LogLevel = ::libtbag::log::details::LogLevel;

Logger * createDefaultConsoleLogger(bool auto_flush = false);
Logger * createDefaultColorConsoleLogger(bool auto_flush = false);
Logger * createDefaultFileLogger(std::string const & path, bool auto_flush = false);
void removeDefaultLogger();

Logger * getLogger(std::string const & name);
Logger * getDefaultLogger();

void setLevel(LogLevel level);
void setDefaultLevel(LogLevel level);

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
# define tLog(name, level, msg, ...)
#else
# define tLog(name, level, msg, ...) ::libtbag::log::logging(::libtbag::log::getLogger(name), level, msg, __VA_ARGS__)
#endif

#define tLogE(name, msg, ...)  tLog(name, ::libtbag::log::LogLevel::LEVEL_EMERGENCY, msg, __VA_ARGS__)
#define tLogA(name, msg, ...)  tLog(name, ::libtbag::log::LogLevel::LEVEL_ALERT    , msg, __VA_ARGS__)
#define tLogC(name, msg, ...)  tLog(name, ::libtbag::log::LogLevel::LEVEL_CRITICAL , msg, __VA_ARGS__)
#define tLogR(name, msg, ...)  tLog(name, ::libtbag::log::LogLevel::LEVEL_ERROR    , msg, __VA_ARGS__)
#define tLogW(name, msg, ...)  tLog(name, ::libtbag::log::LogLevel::LEVEL_WARNING  , msg, __VA_ARGS__)
#define tLogN(name, msg, ...)  tLog(name, ::libtbag::log::LogLevel::LEVEL_NOTICE   , msg, __VA_ARGS__)
#define tLogI(name, msg, ...)  tLog(name, ::libtbag::log::LogLevel::LEVEL_INFO     , msg, __VA_ARGS__)
#define tLogD(name, msg, ...)  tLog(name, ::libtbag::log::LogLevel::LEVEL_DEBUG    , msg, __VA_ARGS__)

#define tDLogE(msg, ...)  tLogE(::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, msg, __VA_ARGS__)
#define tDLogA(msg, ...)  tLogA(::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, msg, __VA_ARGS__)
#define tDLogC(msg, ...)  tLogC(::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, msg, __VA_ARGS__)
#define tDLogR(msg, ...)  tLogR(::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, msg, __VA_ARGS__)
#define tDLogW(msg, ...)  tLogW(::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, msg, __VA_ARGS__)
#define tDLogN(msg, ...)  tLogN(::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, msg, __VA_ARGS__)
#define tDLogI(msg, ...)  tLogI(::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, msg, __VA_ARGS__)
#define tDLogD(msg, ...)  tLogD(::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, msg, __VA_ARGS__)

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_LOG_HPP__

