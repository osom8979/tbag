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

inline void emergency    (Logger * logger, std::string const & msg) { if (logger) { logger->emergency(msg);     } }
inline void alert        (Logger * logger, std::string const & msg) { if (logger) { logger->alert(msg);         } }
inline void critical     (Logger * logger, std::string const & msg) { if (logger) { logger->critical(msg);      } }
inline void error        (Logger * logger, std::string const & msg) { if (logger) { logger->error(msg);         } }
inline void warning      (Logger * logger, std::string const & msg) { if (logger) { logger->warning(msg);       } }
inline void notice       (Logger * logger, std::string const & msg) { if (logger) { logger->notice(msg);        } }
inline void informational(Logger * logger, std::string const & msg) { if (logger) { logger->informational(msg); } }
inline void debug        (Logger * logger, std::string const & msg) { if (logger) { logger->debug(msg);         } }

inline void emergency    (std::string const & msg) { emergency    (getDefaultLogger(), msg); }
inline void alert        (std::string const & msg) { alert        (getDefaultLogger(), msg); }
inline void critical     (std::string const & msg) { critical     (getDefaultLogger(), msg); }
inline void error        (std::string const & msg) { error        (getDefaultLogger(), msg); }
inline void warning      (std::string const & msg) { warning      (getDefaultLogger(), msg); }
inline void notice       (std::string const & msg) { notice       (getDefaultLogger(), msg); }
inline void informational(std::string const & msg) { informational(getDefaultLogger(), msg); }
inline void debug        (std::string const & msg) { debug        (getDefaultLogger(), msg); }

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

