/**
 * @file   CallableUdp.hpp
 * @brief  CallableUdp class prototype.
 * @author zer0
 * @date   2017-01-12
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UV_EX_CALLABLEUDP_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UV_EX_CALLABLEUDP_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/uv/Loop.hpp>
#include <libtbag/uv/Udp.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uv {
namespace ex {

/**
 * CallableUdp class prototype.
 *
 * @author zer0
 * @date   2017-01-12
 */
class CallableUdp : public Udp
{
public:
    // @formatter:off
    struct Callback
    {
        // Event of Udp.
        virtual void onSend(UdpSendRequest & request, Err code)
        { /* EMPTY. */ }
        virtual binf onAlloc(std::size_t suggested_size)
        { return binf(); }
        virtual void onRead(Err code, char const * buffer, std::size_t size, sockaddr const * addr, unsigned int flags)
        { /* EMPTY. */ }

        // Event of Handle.
        virtual void onClose()          { /* EMPTY. */ }
        virtual void onWalk(void * arg) { /* EMPTY. */ }
    };
    // @formatter:on

private:
    Callback * _cb;

public:
    // @formatter:off
    CallableUdp() : Udp(), _cb(nullptr)
    { /* EMPTY. */ }
    CallableUdp(Loop & l, Callback * c = nullptr) : Udp(l), _cb(c)
    { /* EMPTY. */ }
    virtual ~CallableUdp()
    { /* EMPTY. */ }
    // @formatter:on

public:
    inline void setCallback(Callback * callback) TBAG_NOEXCEPT
    { _cb = callback; }

public:
    // @formatter:off
    virtual void onSend(UdpSendRequest & request, Err code) override
    { if (_cb != nullptr) { _cb->onSend(request, code); } }
    virtual binf onAlloc(std::size_t suggested_size) override
    { if (_cb != nullptr) { return _cb->onAlloc(suggested_size); } return binf(); }
    virtual void onRead(Err code, char const * buffer, std::size_t size, sockaddr const * addr, unsigned int flags) override
    { if (_cb != nullptr) { _cb->onRead(code, buffer, size, addr, flags); } }

    virtual void onClose() override
    { if (_cb != nullptr) { _cb->onClose(); } }
    virtual void onWalk(void * arg) override
    { if (_cb != nullptr) { _cb->onWalk(arg); } }
    // @formatter:on
};

} // namespace ex
} // namespace uv

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UV_EX_CALLABLEUDP_HPP__

