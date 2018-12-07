/**
 * @file   FunctionalTcp.hpp
 * @brief  FunctionalTcp class prototype.
 * @author zer0
 * @date   2017-09-05
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIONALTCP_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIONALTCP_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Type.hpp>

#include <libtbag/uvpp/func/FunctionalStream.hpp>
#include <libtbag/uvpp/Tcp.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {
namespace func {

/**
 * FunctionalTcp class prototype.
 *
 * @author zer0
 * @date   2017-09-05
 */
template <typename TcpType>
struct FunctionalTcp : public FunctionalStream<PipeType>
{
    using Parent    = FunctionalStream<TcpType>;
    using OnConnect = std::function<void(ConnectRequest&, Err)>;

    STATIC_ASSERT_CHECK_IS_BASE_OF(libtbag::uvpp::Tcp, Parent);

    OnConnect connect_cb;

    template <typename ... Args>
    FunctionalTcp(Args && ... args) : Parent(std::forward<Args>(args) ...)
    { /* EMPTY. */ }

    virtual ~FunctionalTcp()
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
 * FuncTcp typedef.
 *
 * @author zer0
 * @date   2017-09-05
 */
using FuncTcp = FunctionalTcp<libtbag::uvpp::Tcp>;

} // namespace func
} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIONALTCP_HPP__

