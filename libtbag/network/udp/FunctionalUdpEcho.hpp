/**
 * @file   FunctionalUdpEcho.hpp
 * @brief  FunctionalUdpEcho class prototype.
 * @author zer0
 * @date   2017-09-15
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_UDP_FUNCTIONALUDPECHO_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_UDP_FUNCTIONALUDPECHO_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/network/udp/UdpEcho.hpp>

#include <functional>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace udp     {

/**
 * FunctionalUdpEcho class prototype.
 *
 * @author zer0
 * @date   2017-08-14
 */
class FunctionalUdpEcho : public UdpEcho
{
public:
    using OnEcho = std::function<void(std::string &, SocketAddress &)>;
    using OnEnd  = std::function<void(void)>;

private:
    OnEcho _echo_cb;
    OnEnd  _end_cb;

public:
    FunctionalUdpEcho(Loop & loop) : UdpEcho(loop)
    { /* EMPTY. */ }
    FunctionalUdpEcho(Loop & loop, SharedSafetyAsync async, send_flags const & send) : UdpEcho(loop, async, send)
    { /* EMPTY. */ }
    FunctionalUdpEcho(Loop & loop, SharedSafetyAsync async, recv_flags const & recv) : UdpEcho(loop, async, recv)
    { /* EMPTY. */ }
    FunctionalUdpEcho(Loop & loop, SharedSafetyAsync async, unsigned int flags = UdpNode::UDP_NODE_FLAG_NOTING) : UdpEcho(loop, async, flags)
    { /* EMPTY. */ }
    virtual ~FunctionalUdpEcho()
    { /* EMPTY. */ }

public:
    inline void setOnEcho(OnEcho const & cb) { _echo_cb = cb; }
    inline void setOnEnd (OnEnd  const & cb) { _end_cb = cb; }

protected:
    virtual void onEcho(std::string & message, SocketAddress & addr) override
    {
        if (static_cast<bool>(_echo_cb)) {
            _echo_cb(message, addr);
        }
    }

    virtual void onEnd() override
    {
        if (static_cast<bool>(_end_cb)) {
            _end_cb();
        }
    }
};

/**
 * FunctionalUdpEchoServer class prototype.
 *
 * @author zer0
 * @date   2017-08-14
 */
struct FunctionalUdpEchoServer : public FunctionalUdpEcho
{
    FunctionalUdpEchoServer(Loop & loop, std::string const & bind_ip, int port, uint64_t timeout = 0)
            : FunctionalUdpEcho(loop)
    {
        if (initServer(bind_ip, port, timeout) != Err::E_SUCCESS) {
            throw std::bad_alloc();
        }
    }

    virtual ~FunctionalUdpEchoServer()
    { /* EMPTY. */ }
};

/**
 * FunctionalUdpEchoClient class prototype.
 *
 * @author zer0
 * @date   2017-08-14
 */
struct FunctionalUdpEchoClient : public FunctionalUdpEcho
{
    FunctionalUdpEchoClient(Loop & loop, std::string const & bind_ip, int port, int broadcast_port, uint64_t timeout = 0)
            : FunctionalUdpEcho(loop)
    {
        if (initClient(bind_ip, port, broadcast_port, timeout) != Err::E_SUCCESS) {
            throw std::bad_alloc();
        }
    }

    virtual ~FunctionalUdpEchoClient()
    { /* EMPTY. */ }
};

using FuncUdpEcho       = FunctionalUdpEcho;
using FuncUdpEchoServer = FunctionalUdpEchoServer;
using FuncUdpEchoClient = FunctionalUdpEchoClient;

} // namespace udp
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_UDP_FUNCTIONALUDPECHO_HPP__

