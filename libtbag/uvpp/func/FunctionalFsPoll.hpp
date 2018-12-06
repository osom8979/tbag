/**
 * @file   FunctionalFsPoll.hpp
 * @brief  FunctionalFsPoll class prototype.
 * @author zer0
 * @date   2018-12-06
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIONALFSPOLL_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIONALFSPOLL_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Type.hpp>

#include <libtbag/uvpp/func/FunctionalHandle.hpp>
#include <libtbag/uvpp/FsPoll.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {
namespace func {

// Forward declaration.
class Loop;

/**
 * FunctionalFsPoll class prototype.
 *
 * @author zer0
 * @date   2018-12-06
 */
template <typename CheckType>
struct FunctionalFsPoll : public FunctionalHandle<CheckType>
{
    using Parent    = FunctionalHandle<CheckType>;
    using FileState = libtbag::uvpp::FsPoll::FileState;
    using OnFsPoll  = std::function<void(Err, FileState const &, FileState const &)>;

    STATIC_ASSERT_CHECK_IS_BASE_OF(libtbag::uvpp::FsPoll, Parent);

    OnFsPoll fs_poll_cb;

    template <typename ... Args>
    FunctionalFsPoll(Args && ... args) : Parent(std::forward<Args>(args) ...)
    { /* EMPTY. */ }

    virtual ~FunctionalFsPoll()
    { /* EMPTY. */ }

    virtual void onFsPoll(Err status, FileState const & prev, FileState const & curr) override
    {
        if (fs_poll_cb) {
            fs_poll_cb(status, prev, curr);
        } else {
            Parent::onFsPoll(status, prev, curr);
        }
    }
};

/**
 * FuncFsPoll typedef.
 *
 * @author zer0
 * @date   2018-12-06
 */
using FuncFsPoll = FunctionalFsPoll<libtbag::uvpp::FsPoll>;

} // namespace func
} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIONALFSPOLL_HPP__

