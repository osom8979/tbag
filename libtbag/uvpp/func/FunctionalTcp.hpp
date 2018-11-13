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

#include <libtbag/uvpp/func/FunctionalStream.hpp>
#include <libtbag/uvpp/Tcp.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {

// Forward declaration.
class Loop;

namespace func {

/**
 * FunctionalTcp class prototype.
 *
 * @author zer0
 * @date   2017-09-05
 */
template <typename TcpType, typename MutexType = lock::FakeLock>
class FunctionalTcp : public FunctionalStream<TcpType, MutexType>
{
public:
    using Base  = FunctionalStream<TcpType, MutexType>;
    using Guard = typename Base::Guard;
    using OnConnect = std::function<void(ConnectRequest&, Err)>;

private:
    OnConnect _connect_cb;

public:
    template <typename ... Args>
    FunctionalTcp(Args && ... args) : Base(std::forward<Args>(args) ...)
    { /* EMPTY. */ }
    virtual ~FunctionalTcp()
    { /* EMPTY. */ }

public:
    void setOnConnect(OnConnect const & cb)
    {
        Guard guard(Base::_mutex);
        _connect_cb = cb;
    }

public:
    virtual void onConnect(ConnectRequest & request, Err code) override
    {
        Guard guard(Base::_mutex);
        if (static_cast<bool>(Base::_write_cb)) {
            Base::_write_cb(request, code);
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

