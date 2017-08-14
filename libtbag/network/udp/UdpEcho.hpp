/**
 * @file   UdpEcho.hpp
 * @brief  UdpEcho class prototype.
 * @author zer0
 * @date   2017-08-14
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_UDP_UDPECHO_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_UDP_UDPECHO_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>

#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/uvpp/func/FunctionalTimer.hpp>
#include <libtbag/network/SocketAddress.hpp>
#include <libtbag/network/udp/UdpNode.hpp>

#include <cstdint>
#include <string>
#include <memory>
#include <functional>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace udp     {

/**
 * UdpEcho class prototype.
 *
 * @author zer0
 * @date   2017-08-14
 */
class TBAG_API UdpEcho : public UdpNode
{
public:
    using Loop = uvpp::Loop;
    using FuncTimer = uvpp::func::FuncTimer;
    using SharedFuncTimer = std::shared_ptr<FuncTimer>;

public:
    enum class EchoType
    {
        ET_UNKNOWN,
        ET_SERVER,
        ET_CLIENT,
    };

private:
    EchoType _type;
    SharedFuncTimer _timer;

public:
    UdpEcho(Loop & loop);
    virtual ~UdpEcho();

public:
    inline EchoType getType() const TBAG_NOEXCEPT { return _type; }

private:
    Err initDefault(std::string const & bind_ip, int port, uint64_t timeout = 0);

public:
    Err initServer(std::string const & bind_ip, int port, uint64_t timeout = 0);
    Err initClient(std::string const & bind_ip, int port, int broadcast_port, uint64_t timeout = 0);

public:
    Err echo(std::string const & message);

protected:
    virtual void onWrite(Err code) final override;
    virtual void onRead (Err code, ReadPacket const & packet) final override;
    virtual void onClose() final override;

public:
    virtual void onEcho(std::string & message, SocketAddress & addr) { /* EMPTY. */ }
    virtual void onEnd() { /* EMPTY. */ }
};

/**
 * FunctionalUdpEcho class prototype.
 *
 * @author zer0
 * @date   2017-08-14
 */
struct FunctionalUdpEcho : public UdpEcho
{
    using OnEcho = std::function<void(std::string &, SocketAddress &)>;
    using OnEnd  = std::function<void(void)>;

    OnEcho echo_cb;
    OnEnd  end_cb;

    FunctionalUdpEcho(Loop & loop) : UdpEcho(loop)
    { /* EMPTY. */ }
    virtual ~FunctionalUdpEcho()
    { /* EMPTY. */ }

    inline void setOnEcho(OnEcho const & cb) { echo_cb = cb; }
    inline void setOnEnd (OnEnd  const & cb) { end_cb = cb; }

    virtual void onEcho(std::string & message, SocketAddress & addr) override
    {
        if (static_cast<bool>(echo_cb)) {
            echo_cb(message, addr);
        }
    }

    virtual void onEnd() override
    {
        if (static_cast<bool>(end_cb)) {
            end_cb();
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

using FuncUdpEchoServer = FunctionalUdpEchoServer;
using FuncUdpEchoClient = FunctionalUdpEchoClient;

} // namespace udp
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_UDP_UDPECHO_HPP__

