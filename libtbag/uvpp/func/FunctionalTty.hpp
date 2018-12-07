/**
 * @file   FunctionalTty.hpp
 * @brief  FunctionalTty class prototype.
 * @author zer0
 * @date   2018-12-07
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIONALTTY_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIONALTTY_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Type.hpp>

#include <libtbag/uvpp/func/FunctionalStream.hpp>
#include <libtbag/uvpp/Tty.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {
namespace func {

/**
 * FunctionalTty class prototype.
 *
 * @author zer0
 * @date   2018-12-07
 */
template <typename TtyType>
struct FunctionalTty : public FunctionalStream<TtyType>
{
    using Parent = FunctionalStream<TtyType>;

    STATIC_ASSERT_CHECK_IS_BASE_OF(libtbag::uvpp::Tty, Parent);

    template <typename ... Args>
    FunctionalTty(Args && ... args) : Parent(std::forward<Args>(args) ...)
    { /* EMPTY. */ }

    virtual ~FunctionalTty()
    { /* EMPTY. */ }
};

/**
 * FuncTty typedef.
 *
 * @author zer0
 * @date   2018-12-07
 */
using FuncTty = FunctionalTty<libtbag::uvpp::Tty>;

} // namespace func
} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIONALTTY_HPP__

