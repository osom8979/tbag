/**
 * @file   FunctionalSignal.hpp
 * @brief  FunctionalSignal class prototype.
 * @author zer0
 * @date   2017-07-14
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIONALSIGNAL_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIONALSIGNAL_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

#include <libtbag/uvpp/func/FunctionalHandle.hpp>
#include <libtbag/uvpp/Signal.hpp>
#include <libtbag/lock/FakeLock.hpp>

#include <functional>
#include <mutex>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {

// Forward declaration.
class Loop;

namespace func {

/**
 * FunctionalSignal class prototype.
 *
 * @author zer0
 * @date   2017-07-14
 */
template <typename SignalType, typename MutexType = lock::FakeLock>
class FunctionalSignal : public SignalType
{
public:
    using Parent = SignalType;
    using Mutex  = MutexType;
    using Guard  = std::lock_guard<Mutex>;

    STATIC_ASSERT_CHECK_IS_BASE_OF(libtbag::uvpp::Signal, Parent);
    TBAG_UVPP_FUNCTIONAL_HANDLE_DEFAULT(Guard, _mutex);

public:
    using OnSignal = std::function<void(int)>;

private:
    Mutex _mutex;
    OnSignal _signal_cb;

public:
    FunctionalSignal(Loop & loop) : Parent(loop)
    { /* EMPTY. */ }
    virtual ~FunctionalSignal()
    { /* EMPTY. */ }

public:
    void setOnSignal(OnSignal const & cb)
    {
        Guard guard(_mutex);
        _signal_cb = cb;
    }

public:
    virtual void onSignal(int signum) override
    {
        Guard guard(_mutex);
        if (static_cast<bool>(_signal_cb)) {
            _signal_cb(signum);
        }
    }
};

/**
 * FuncSignal typedef.
 *
 * @author zer0
 * @date   2017-07-14
 */
using FuncSignal = FunctionalSignal<libtbag::uvpp::Signal>;

} // namespace func
} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIONALSIGNAL_HPP__

