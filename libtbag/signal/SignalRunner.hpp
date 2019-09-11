/**
 * @file   SignalRunner.hpp
 * @brief  SignalRunner class prototype.
 * @author zer0
 * @date   2019-09-11
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_SIGNAL_SIGNALRUNNER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_SIGNAL_SIGNALRUNNER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/signal/SignalCatcher.hpp>

#include <atomic>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace signal {

/**
 * SignalRunner class prototype.
 *
 * @author zer0
 * @date   2019-09-11
 */
class TBAG_API SignalRunner : private Noncopyable
{
public:
    struct Callback
    {
        Callback() { /* EMPTY. */ }
        virtual ~Callback() { /* EMPTY. */ }

        /** SetUp callback. */
        virtual bool onSetUp() { return true; }
        /** Call an asynchronous operation. The function should exit immediately. */
        virtual void onAsync() { /* EMPTY. */ }
        /** TearDown callback. */
        virtual void onTearDown() { /* EMPTY. */ }
    };

private:
    SignalCatcher _signal;
    Callback * _callback;

private:
    std::atomic_bool _enable_stdout;

public:
    SignalRunner(Callback * cb = nullptr, bool enable_stdout = true);
    ~SignalRunner();

public:
    bool run(int tick_ms = 1);
};

} // namespace signal

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_SIGNAL_SIGNALRUNNER_HPP__

