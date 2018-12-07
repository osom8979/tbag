/**
 * @file   FunctionalCheck.hpp
 * @brief  FunctionalCheck class prototype.
 * @author zer0
 * @date   2017-05-31
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIONALCHECK_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIONALCHECK_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Type.hpp>

#include <libtbag/uvpp/func/FunctionalHandle.hpp>
#include <libtbag/uvpp/Check.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {
namespace func {

/**
 * FunctionalCheck class prototype.
 *
 * @author zer0
 * @date   2017-05-31
 */
template <typename CheckType>
struct FunctionalCheck : public FunctionalHandle<CheckType>
{
    using Parent  = FunctionalHandle<CheckType>;
    using OnCheck = std::function<void(void)>;

    STATIC_ASSERT_CHECK_IS_BASE_OF(libtbag::uvpp::Check, Parent);

    OnCheck check_cb;

    template <typename ... Args>
    FunctionalCheck(Args && ... args) : Parent(std::forward<Args>(args) ...)
    { /* EMPTY. */ }

    virtual ~FunctionalCheck()
    { /* EMPTY. */ }

    virtual void onCheck() override
    {
        if (check_cb) {
            check_cb();
        } else {
            Parent::onCheck();
        }
    }
};

/**
 * FuncCheck typedef.
 *
 * @author zer0
 * @date   2017-05-30
 */
using FuncCheck = FunctionalCheck<libtbag::uvpp::Check>;

} // namespace func
} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIONALCHECK_HPP__

