/**
 * @file   FunctionalPoll.hpp
 * @brief  FunctionalPoll class prototype.
 * @author zer0
 * @date   2018-12-06
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIONALPOLL_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIONALPOLL_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Type.hpp>

#include <libtbag/uvpp/func/FunctionalHandle.hpp>
#include <libtbag/uvpp/Poll.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {
namespace func {

/**
 * FunctionalPoll class prototype.
 *
 * @author zer0
 * @date   2018-12-06
 */
template <typename CheckType>
struct FunctionalPoll : public FunctionalHandle<CheckType>
{
    using Parent    = FunctionalHandle<CheckType>;
    using EventType = typename Parent::EventType;
    using OnPoll    = std::function<void(Err, EventType)>;

    STATIC_ASSERT_CHECK_IS_BASE_OF(libtbag::uvpp::Poll, Parent);
    STATIC_ASSERT_CHECK_IS_SAME(libtbag::uvpp::Poll::EventType, EventType);

    OnPoll poll_cb;

    template <typename ... Args>
    FunctionalPoll(Args && ... args) : Parent(std::forward<Args>(args) ...)
    { /* EMPTY. */ }

    virtual ~FunctionalPoll()
    { /* EMPTY. */ }

    virtual void onPoll(Err status, EventType events) override
    {
        if (poll_cb) {
            poll_cb(status, events);
        } else {
            Parent::onPoll(status, events);
        }
    }
};

/**
 * FuncPoll typedef.
 *
 * @author zer0
 * @date   2018-12-06
 */
using FuncPoll = FunctionalPoll<libtbag::uvpp::Poll>;

} // namespace func
} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIONALPOLL_HPP__

