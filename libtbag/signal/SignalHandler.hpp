/**
 * @file   SignalHandler.hpp
 * @brief  SignalHandler class prototype.
 * @author zer0
 * @date   2016-12-24
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_SIGNAL_SIGNALHANDLER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_SIGNAL_SIGNALHANDLER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <csignal>
#include <climits>
#include <cstdlib>

#include <string>
#include <exception>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace signal {

// ==================
namespace __impl {
/** @warning Don't use this method from user level developers. */
TBAG_API void createInstance();
TBAG_API void releaseInstance();
} // namespace __impl
// ==================

#ifndef TBAG_SIGNAL_MAP
#define TBAG_SIGNAL_MAP(_TBAG_XX) \
    _TBAG_XX(SIGNAL_INTERRUPT               , SIGINT /*  2 */, "interrupt") \
    _TBAG_XX(SIGNAL_ILLEGAL_INSTRUCTION     , SIGILL /*  4 */, "illegal instruction (not reset when caught)") \
    _TBAG_XX(SIGNAL_FLOATING_POINT_EXCEPTION, SIGFPE /*  8 */, "floating point exception") \
    _TBAG_XX(SIGNAL_SEGMENTATION_VIOLATION  , SIGSEGV/* 11 */, "segmentation violation") \
    _TBAG_XX(SIGNAL_TERMINATION             , SIGTERM/* 15 */, "software termination signal from kill") \
    _TBAG_XX(SIGNAL_ABORT                   , SIGABRT/* ?? */, "abort()") /* Windows(22), OSX(6) */ \
    /* -- END -- */
#endif

#define _TBAG_XX(name, signal, message) static int const name = signal;
TBAG_SIGNAL_MAP(_TBAG_XX)
#undef _TBAG_XX

// Don't use the std::numeric_limits class.
// Avoid collisions of min & max symbol in MSVC.
TBAG_CONSTEXPR int const SIGNAL_STD_TERMINATE = INT_MAX; // C++ terminate signal.
TBAG_CONSTEXPR int const FIRST_ORDER = INT_MAX;
TBAG_CONSTEXPR int const LAST_ORDER  = INT_MIN;

/**
 * Signal handler interface.
 *
 * @author zer0
 * @date   2016-12-24
 */
struct SignalHandler
{
    virtual void run(int signal) = 0;
};

TBAG_API std::string getSignalName(int signal_number);

TBAG_API void registerStdTerminateHandler(SignalHandler * handler, int order = 0);
TBAG_API void registerHandler(int signal, SignalHandler * handler, int order = 0);

TBAG_API void registerDefaultStdTerminateHandler(std::string const & logger_name = "");
TBAG_API void registerDefaultHandler(std::string const & logger_name = "");

TBAG_API SignalHandler * createDefaultSignalHandler(std::string const & logger_name = "");

// @formatter:off
inline int raise(int signal)
{ return std::raise(signal); }
inline void terminate() TBAG_NOEXCEPT_SP_OP(std::terminate())
{ std::terminate(); }
inline void exit(int code)
{ std::exit(code); }
inline void exitForce(int code)
{ std::_Exit(code); }
// @formatter:on

} // namespace signal

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_SIGNAL_SIGNALHANDLER_HPP__

