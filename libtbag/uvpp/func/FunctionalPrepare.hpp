/**
 * @file   FunctionalPrepare.hpp
 * @brief  FunctionalPrepare class prototype.
 * @author zer0
 * @date   2017-05-31
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIONALPREPARE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIONALPREPARE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Type.hpp>

#include <libtbag/uvpp/func/FunctionalHandle.hpp>
#include <libtbag/uvpp/Prepare.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {

// Forward declaration.
class Loop;

namespace func {

/**
 * FunctionalPrepare class prototype.
 *
 * @author zer0
 * @date   2017-05-31
 */
template <typename PrepareType>
struct FunctionalPrepare : public FunctionalHandle<PrepareType>
{
    using Parent    = FunctionalHandle<PrepareType>;
    using OnPrepare = std::function<void(void)>;

    STATIC_ASSERT_CHECK_IS_BASE_OF(libtbag::uvpp::Prepare, Parent);

    OnPrepare prepare_cb;

    template <typename ... Args>
    FunctionalPrepare(Args && ... args) : Parent(std::forward<Args>(args) ...)
    { /* EMPTY. */ }

    virtual ~FunctionalPrepare()
    { /* EMPTY. */ }

    virtual void onPrepare() override
    {
        if (prepare_cb) {
            prepare_cb();
        } else {
            Parent::onPrepare();
        }
    }
};

/**
 * FuncPrepare typedef.
 *
 * @author zer0
 * @date   2017-05-30
 */
using FuncPrepare = FunctionalPrepare<libtbag::uvpp::Prepare>;

} // namespace func
} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIONALPREPARE_HPP__

