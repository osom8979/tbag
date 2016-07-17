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

constexpr bool isAsynchronousLogging() noexcept
{
    return true;
}

constexpr bool isMultithreadLogging() noexcept
{
    return false;
}

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

::libtbag::log::details::Logger * createDefaultConsoleLogger();
::libtbag::log::details::Logger * createDefaultFileLogger(std::string const & path, bool auto_flush = false);

::libtbag::log::details::Logger * getDefaultLogger();

void setDefaultLevel(LogLevel level);

/**
 * @warning
 *  REMOVE DEFAULT LOG.
 */
void removeDefaultLogger();

} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#if defined(DISABLE_TBAG_LOG)
# define tbLOG(level, msg, ...)
#else
# define tbLOG(level, msg, ...)
#endif

#define tbLOGE(msg, ...)  tbLOG(tbEMERGENCY , msg, __VA_ARGS__)
#define tbLOGA(msg, ...)  tbLOG(tbALERT     , msg, __VA_ARGS__)
#define tbLOGC(msg, ...)  tbLOG(tbCRITICAL  , msg, __VA_ARGS__)
#define tbLOGR(msg, ...)  tbLOG(tbERROR     , msg, __VA_ARGS__)
#define tbLOGW(msg, ...)  tbLOG(tbWARNING   , msg, __VA_ARGS__)
#define tbLOGN(msg, ...)  tbLOG(tbNOTICE    , msg, __VA_ARGS__)
#define tbLOGI(msg, ...)  tbLOG(tbINFO      , msg, __VA_ARGS__)
#define tbLOGD(msg, ...)  tbLOG(tbDEBUG     , msg, __VA_ARGS__)

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_LOG_HPP__

