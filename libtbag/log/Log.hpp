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

char const * const TBAG_DEFAULT_LOGGER_NAME   = "__tbag_default_logger__";
char const * const TBAG_DEBUGGING_LOGGER_NAME = "__tbag_debugging_logger__";

//TBAG_CONSTEXPR bool isAsynchronousLogging() TBAG_NOEXCEPT { return true;  }
//TBAG_CONSTEXPR bool  isMultithreadLogging() TBAG_NOEXCEPT { return false; }

using Logger   = ::libtbag::log::details::Logger;
using LogLevel = ::libtbag::log::details::LogLevel;

TBAG_API Logger * createConsoleLogger(std::string const & name, bool auto_flush = false);
TBAG_API Logger * createColorConsoleLogger(std::string const & name, bool auto_flush = false);
TBAG_API Logger * createFileLogger(std::string const & name, std::string const & path, bool auto_flush = false);

TBAG_API Logger * createDefaultConsoleLogger(bool auto_flush = false);
TBAG_API Logger * createDefaultColorConsoleLogger(bool auto_flush = false);
TBAG_API Logger * createDefaultFileLogger(std::string const & path, bool auto_flush = false);

TBAG_API void removeLogger(std::string const & name);
TBAG_API void removeDefaultLogger();

TBAG_API Logger * getLogger(std::string const & name);
TBAG_API Logger * getDefaultLogger();

TBAG_API void setLevel(std::string const & name, LogLevel level);
TBAG_API void setLevel(std::string const & name, int level);
TBAG_API void setDefaultLevel(LogLevel level);

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
# define tLog(name, level, ...)
#else
# define tLog(name, level, ...)  ::libtbag::log::logging(::libtbag::log::getLogger(name), level, __VA_ARGS__)
#endif

#define tLogM(name, ...)  tLog(name, ::libtbag::log::LogLevel::LEVEL_EMERGENCY, __VA_ARGS__)
#define tLogA(name, ...)  tLog(name, ::libtbag::log::LogLevel::LEVEL_ALERT    , __VA_ARGS__)
#define tLogC(name, ...)  tLog(name, ::libtbag::log::LogLevel::LEVEL_CRITICAL , __VA_ARGS__)
#define tLogE(name, ...)  tLog(name, ::libtbag::log::LogLevel::LEVEL_ERROR    , __VA_ARGS__)
#define tLogW(name, ...)  tLog(name, ::libtbag::log::LogLevel::LEVEL_WARNING  , __VA_ARGS__)
#define tLogN(name, ...)  tLog(name, ::libtbag::log::LogLevel::LEVEL_NOTICE   , __VA_ARGS__)
#define tLogI(name, ...)  tLog(name, ::libtbag::log::LogLevel::LEVEL_INFO     , __VA_ARGS__)
#define tLogD(name, ...)  tLog(name, ::libtbag::log::LogLevel::LEVEL_DEBUG    , __VA_ARGS__)

#define tDLogM(...)  tLogM(::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define tDLogA(...)  tLogA(::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define tDLogC(...)  tLogC(::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define tDLogE(...)  tLogE(::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define tDLogW(...)  tLogW(::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define tDLogN(...)  tLogN(::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define tDLogI(...)  tLogI(::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define tDLogD(...)  tLogD(::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, __VA_ARGS__)

/**
 * @def __tbag_debug
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

