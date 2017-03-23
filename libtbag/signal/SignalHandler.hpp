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
#include <string>
#include <limits>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace signal {

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

#if defined(max)
TBAG_PUSH_MACRO(max);
#undef max
#define __RESTORE_MAX__
#endif

TBAG_CONSTEXPR int const SIGNAL_STD_TERMINATE = std::numeric_limits<int>::min(); // C++ terminate signal.
TBAG_CONSTEXPR int const FIRST_ORDER = std::numeric_limits<int>::max();
TBAG_CONSTEXPR int const LAST_ORDER  = std::numeric_limits<int>::min();

#if defined(__RESTORE_MAX__)
TBAG_POP_MACRO(max);
#undef __RESTORE_MAX__
#endif

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
TBAG_API void raise(int signal);

TBAG_API void registerDefaultStdTerminateHandler();
TBAG_API void registerDefaultHandler();

} // namespace signal

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_SIGNAL_SIGNALHANDLER_HPP__

