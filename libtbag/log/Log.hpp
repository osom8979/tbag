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

Logger * createDefaultConsoleLogger();
Logger * createDefaultFileLogger(std::string const & path, bool auto_flush = false);
void removeDefaultLogger();

Logger * getDefaultLogger();
void setDefaultLevel(LogLevel level);

template <typename ... Args>
inline void emergency(Logger * logger, std::string const & format, Args && ... args) { if (logger) { logger->logf(LogLevel::LEVEL_EMERGENCY, format, std::forward<Args>(args) ...); } }
template <typename ... Args>
inline void alert    (Logger * logger, std::string const & format, Args && ... args) { if (logger) { logger->logf(LogLevel::LEVEL_ALERT    , format, std::forward<Args>(args) ...); } }
template <typename ... Args>
inline void critical (Logger * logger, std::string const & format, Args && ... args) { if (logger) { logger->logf(LogLevel::LEVEL_CRITICAL , format, std::forward<Args>(args) ...); } }
template <typename ... Args>
inline void error    (Logger * logger, std::string const & format, Args && ... args) { if (logger) { logger->logf(LogLevel::LEVEL_ERROR    , format, std::forward<Args>(args) ...); } }
template <typename ... Args>
inline void warning  (Logger * logger, std::string const & format, Args && ... args) { if (logger) { logger->logf(LogLevel::LEVEL_WARNING  , format, std::forward<Args>(args) ...); } }
template <typename ... Args>
inline void notice   (Logger * logger, std::string const & format, Args && ... args) { if (logger) { logger->logf(LogLevel::LEVEL_NOTICE   , format, std::forward<Args>(args) ...); } }
template <typename ... Args>
inline void info     (Logger * logger, std::string const & format, Args && ... args) { if (logger) { logger->logf(LogLevel::LEVEL_INFO     , format, std::forward<Args>(args) ...); } }
template <typename ... Args>
inline void debug    (Logger * logger, std::string const & format, Args && ... args) { if (logger) { logger->logf(LogLevel::LEVEL_DEBUG    , format, std::forward<Args>(args) ...); } }

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

} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#if defined(DISABLE_TBAG_LOG)
# define tbLOG(name, level, msg, ...)
#else
# define tbLOG(name, level, msg, ...)
#endif

#define tbLOGE(name, msg, ...)  tbLOG(name, tbEMERGENCY , msg, __VA_ARGS__)
#define tbLOGA(name, msg, ...)  tbLOG(name, tbALERT     , msg, __VA_ARGS__)
#define tbLOGC(name, msg, ...)  tbLOG(name, tbCRITICAL  , msg, __VA_ARGS__)
#define tbLOGR(name, msg, ...)  tbLOG(name, tbERROR     , msg, __VA_ARGS__)
#define tbLOGW(name, msg, ...)  tbLOG(name, tbWARNING   , msg, __VA_ARGS__)
#define tbLOGN(name, msg, ...)  tbLOG(name, tbNOTICE    , msg, __VA_ARGS__)
#define tbLOGI(name, msg, ...)  tbLOG(name, tbINFO      , msg, __VA_ARGS__)
#define tbLOGD(name, msg, ...)  tbLOG(name, tbDEBUG     , msg, __VA_ARGS__)

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_LOG_HPP__

