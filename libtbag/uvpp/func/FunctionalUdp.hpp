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
#include <libtbag/lock/FakeLock.hpp>

#include <functional>
#include <mutex>

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
template <typename UdpType, typename MutexType = lock::FakeLock>
class FunctionalUdp : public UdpType
{
public:
    using Parent = UdpType;
    using Mutex  = MutexType;
    using Guard  = std::lock_guard<Mutex>;

    STATIC_ASSERT_CHECK_IS_BASE_OF(libtbag::uvpp::Udp, Parent);
    TBAG_UVPP_FUNCTIONAL_HANDLE_DEFAULT(Guard, _mutex);

public:
    using OnSend  = std::function<void(UdpSendRequest&, Err)>;
    using OnAlloc = std::function<binf(std::size_t)>;
    using OnRecv  = std::function<void(Err, char const *, std::size_t, sockaddr const *, unsigned int)>;

private:
    Mutex _mutex;
    OnSend  _send_cb;
    OnAlloc _alloc_cb;
    OnRecv  _recv_cb;

public:
    FunctionalUdp(Loop & loop) : Parent(loop)
    { /* EMPTY. */ }
    virtual ~FunctionalUdp()
    { /* EMPTY. */ }

public:
    void setOnSend(OnSend const & cb)
    {
        Guard guard(_mutex);
        _send_cb = cb;
    }

    void setOnAlloc(OnAlloc const & cb)
    {
        Guard guard(_mutex);
        _alloc_cb = cb;
    }

    void setOnRecv(OnRecv const & cb)
    {
        Guard guard(_mutex);
        _recv_cb = cb;
    }

public:
    virtual void onSend(UdpSendRequest & request, Err code) override
    {
        Guard guard(_mutex);
        if (static_cast<bool>(_send_cb)) {
            _send_cb(request, code);
        }
    }

    virtual binf onAlloc(std::size_t suggested_size) override
    {
        Guard guard(_mutex);
        if (static_cast<bool>(_alloc_cb)) {
            return _alloc_cb(suggested_size);
        }
        return binf();
    }

    virtual void onRecv(Err code, char const * buffer, std::size_t size, sockaddr const * addr, unsigned int flags) override
    {
        Guard guard(_mutex);
        if (static_cast<bool>(_recv_cb)) {
            _recv_cb(code, buffer, size, addr, flags);
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

