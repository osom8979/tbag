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
#include <libtbag/Type.hpp>

#include <libtbag/uvpp/func/FunctionalHandle.hpp>
#include <libtbag/uvpp/Signal.hpp>

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
template <typename SignalType>
struct FunctionalSignal : public FunctionalHandle<SignalType>
{
    using Parent   = FunctionalHandle<SignalType>;
    using OnSignal = std::function<void(int)>;

    STATIC_ASSERT_CHECK_IS_BASE_OF(libtbag::uvpp::Signal, Parent);

    OnSignal signal_cb;

    template <typename ... Args>
    FunctionalSignal(Args && ... args) : Parent(std::forward<Args>(args) ...)
    { /* EMPTY. */ }

    virtual ~FunctionalSignal()
    { /* EMPTY. */ }

    virtual void onSignal(int signum) override
    {
        if (signal_cb) {
            signal_cb(signum);
        } else {
            Parent::onSignal(signum);
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

