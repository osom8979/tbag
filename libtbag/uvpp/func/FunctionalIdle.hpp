/**
 * @file   FunctionalIdle.hpp
 * @brief  FunctionalIdle class prototype.
 * @author zer0
 * @date   2017-05-31
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIONALIDLE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIONALIDLE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Type.hpp>

#include <libtbag/uvpp/func/FunctionalHandle.hpp>
#include <libtbag/uvpp/Idle.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {
namespace func {

/**
 * FunctionalIdle class prototype.
 *
 * @author zer0
 * @date   2017-05-31
 */
template <typename IdleType>
struct FunctionalIdle : public FunctionalHandle<IdleType>
{
    using Parent = FunctionalHandle<IdleType>;
    using OnIdle = std::function<void(void)>;

    STATIC_ASSERT_CHECK_IS_BASE_OF(libtbag::uvpp::Idle, Parent);

    OnIdle idle_cb;

    template <typename ... Args>
    FunctionalIdle(Args && ... args) : Parent(std::forward<Args>(args) ...)
    { /* EMPTY. */ }

    virtual ~FunctionalIdle()
    { /* EMPTY. */ }

    virtual void onIdle() override
    {
        if (idle_cb) {
            idle_cb();
        } else {
            Parent::onIdle();
        }
    }
};

/**
 * FuncIdle typedef.
 *
 * @author zer0
 * @date   2017-05-30
 */
using FuncIdle = FunctionalIdle<libtbag::uvpp::Idle>;

} // namespace func
} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIONALIDLE_HPP__

