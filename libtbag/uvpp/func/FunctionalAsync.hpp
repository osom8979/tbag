/**
 * @file   FunctionalAsync.hpp
 * @brief  FunctionalAsync class prototype.
 * @author zer0
 * @date   2017-05-31
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIONALASYNC_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIONALASYNC_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Type.hpp>

#include <libtbag/uvpp/func/FunctionalHandle.hpp>
#include <libtbag/uvpp/Async.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {

// Forward declaration.
class Loop;

namespace func {

/**
 * FunctionalAsync class prototype.
 *
 * @author zer0
 * @date   2017-05-31
 */
template <typename AsyncType>
struct FunctionalAsync : public FunctionalHandle<AsyncType>
{
    using Parent  = FunctionalHandle<AsyncType>;
    using OnAsync = std::function<void(void)>;

    STATIC_ASSERT_CHECK_IS_BASE_OF(libtbag::uvpp::Async, Parent);

    OnAsync async_cb;

    template <typename ... Args>
    FunctionalAsync(Args && ... args) : Parent(std::forward<Args>(args) ...)
    { /* EMPTY. */ }

    virtual ~FunctionalAsync()
    { /* EMPTY. */ }

    virtual void onAsync() override
    {
        if (async_cb) {
            async_cb();
        } else {
            Parent::onAsync();
        }
    }
};

/**
 * FuncAsync typedef.
 *
 * @author zer0
 * @date   2017-05-30
 */
using FuncAsync = FunctionalAsync<libtbag::uvpp::Async>;

} // namespace func
} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIONALASYNC_HPP__

