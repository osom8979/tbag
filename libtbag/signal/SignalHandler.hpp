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
    _TBAG_XX(SIGNAL_ABORT                   , SIGABRT, "abort()") \
    _TBAG_XX(SIGNAL_FLOATING_POINT_EXCEPTION, SIGFPE , "floating point exception") \
    _TBAG_XX(SIGNAL_ILLEGAL_INSTRUCTION     , SIGILL , "illegal instruction (not reset when caught)") \
    _TBAG_XX(SIGNAL_INTERRUPT               , SIGINT , "interrupt") \
    _TBAG_XX(SIGNAL_SEGMENTATION_VIOLATION  , SIGSEGV, "segmentation violation") \
    _TBAG_XX(SIGNAL_TERMINATION             , SIGTERM, "software termination signal from kill") \
    /* -- END -- */
#endif

#define _TBAG_XX(name, signal, message) static int const name = signal;
TBAG_SIGNAL_MAP(_TBAG_XX)
#undef _TBAG_XX

static int const SIGNAL_STD_TERMINATE = std::numeric_limits<int>::min(); // C++ terminate signal.

static int const FIRST_ORDER = std::numeric_limits<int>::max();
static int const LAST_ORDER  = std::numeric_limits<int>::min();

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

