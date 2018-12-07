/**
 * @file   FunctionalPipe.hpp
 * @brief  FunctionalPipe class prototype.
 * @author zer0
 * @date   2017-09-05
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIONALPIPE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIONALPIPE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Type.hpp>

#include <libtbag/uvpp/func/FunctionalStream.hpp>
#include <libtbag/uvpp/Pipe.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {
namespace func {

/**
 * FunctionalPipe class prototype.
 *
 * @author zer0
 * @date   2017-09-05
 */
template <typename PipeType>
struct FunctionalPipe : public FunctionalStream<PipeType>
{
    using Parent    = FunctionalStream<PipeType>;
    using OnConnect = std::function<void(ConnectRequest&, Err)>;

    STATIC_ASSERT_CHECK_IS_BASE_OF(libtbag::uvpp::Pipe, Parent);

    OnConnect connect_cb;

    template <typename ... Args>
    FunctionalPipe(Args && ... args) : Parent(std::forward<Args>(args) ...)
    { /* EMPTY. */ }

    virtual ~FunctionalPipe()
    { /* EMPTY. */ }

    virtual void onConnect(ConnectRequest & request, Err code) override
    {
        if (connect_cb) {
            connect_cb(request, code);
        } else {
            Parent::onConnect(request, code);
        }
    }
};

/**
 * FuncPipe typedef.
 *
 * @author zer0
 * @date   2017-09-05
 */
using FuncPipe = FunctionalPipe<libtbag::uvpp::Pipe>;

} // namespace func
} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIONALPIPE_HPP__

