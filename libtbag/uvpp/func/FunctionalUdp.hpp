/**
 * @file   FunctionalUdp.hpp
 * @brief  FunctionalUdp class prototype.
 * @author zer0
 * @date   2017-06-15
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIONALUDP_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIONALUDP_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Type.hpp>

#include <libtbag/uvpp/func/FunctionalHandle.hpp>
#include <libtbag/uvpp/Udp.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {

// Forward declaration.
class Loop;

namespace func {

/**
 * FunctionalUdp class prototype.
 *
 * @author zer0
 * @date   2017-06-15
 */
template <typename UdpType>
struct FunctionalUdp : public FunctionalHandle<UdpType>
{
    using Parent  = FunctionalHandle<UdpType>;
    using OnSend  = std::function<void(UdpSendRequest&, Err)>;
    using OnAlloc = std::function<binf(std::size_t)>;
    using OnRecv  = std::function<void(Err, char const *, std::size_t, sockaddr const *, unsigned int)>;

    STATIC_ASSERT_CHECK_IS_BASE_OF(libtbag::uvpp::Udp, Parent);

    OnSend  send_cb;
    OnAlloc alloc_cb;
    OnRecv  recv_cb;

    template <typename ... Args>
    FunctionalUdp(Args && ... args) : Parent(std::forward<Args>(args) ...)
    { /* EMPTY. */ }

    virtual ~FunctionalUdp()
    { /* EMPTY. */ }

    virtual void onSend(UdpSendRequest & request, Err code) override
    {
        if (send_cb) {
            send_cb(request, code);
        } else {
            Parent::onSend(request, code);
        }
    }

    virtual binf onAlloc(std::size_t suggested_size) override
    {
        if (alloc_cb) {
            return alloc_cb(suggested_size);
        } else {
            return Parent::onAlloc(suggested_size);
        }
    }

    virtual void onRecv(Err code, char const * buffer, std::size_t size, sockaddr const * addr, unsigned int flags) override
    {
        if (recv_cb) {
            recv_cb(code, buffer, size, addr, flags);
        } else {
            Parent::onRecv(code, buffer, size, addr, flags);
        }
    }
};

/**
 * FuncUdp typedef.
 *
 * @author zer0
 * @date   2017-06-15
 */
using FuncUdp = FunctionalUdp<libtbag::uvpp::Udp>;

} // namespace func
} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIONALUDP_HPP__

