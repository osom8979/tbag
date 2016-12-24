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
#include <libtbag/Noncopyable.hpp>

#include <csignal>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace signal {

struct SignalHandler
{
    virtual void run(int signal) = 0;
};

TBAG_API void registerHandler(int signal, int order, SignalHandler * handler);

} // namespace signal

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_SIGNAL_SIGNALHANDLER_HPP__

