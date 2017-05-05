/**
 * @file   TcpNetServer.hpp
 * @brief  TcpNetServer class prototype.
 * @author zer0
 * @date   2017-05-05
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DETAILS_TCPNETSERVER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DETAILS_TCPNETSERVER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/network/Server.hpp>

#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/uvpp/Tcp.hpp>
#include <libtbag/uvpp/Timer.hpp>
#include <libtbag/uvpp/ex/SafetyAsync.hpp>
#include <libtbag/uvpp/Request.hpp>

#include <memory>
#include <vector>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace details {

// Forward declaration.
class ServerImpl;
class NodeImpl;
class TcpNetServer;

/**
 * ServerImpl class prototype.
 *
 * @author zer0
 * @date   2017-05-05
 */
class TBAG_API ServerImpl : public uvpp::Tcp, public details::NetTypes
{
public:
    friend class TcpNetClient;

private:
    TcpNetServer & _parent;

public:
    ServerImpl(Loop & loop, TcpNetServer & parent);
    virtual ~ServerImpl();

public:
    virtual void onConnection(uerr code) override;
    virtual void onClose() override;
};

/**
 * NodeImpl class prototype.
 *
 * @author zer0
 * @date   2017-05-05
 */
class TBAG_API NodeImpl : public uvpp::Tcp, public details::NetTypes
{
public:
    friend class TcpNetClient;

private:
    TcpNetServer & _parent;
    WriteRequest _write_req;
    Buffer _buffer;

public:
    NodeImpl(Loop & loop, TcpNetServer & parent);
    virtual ~NodeImpl();

public:
    virtual void onWrite(WriteRequest & request, uerr code) override;
    virtual binf onAlloc(std::size_t suggested_size) override;
    virtual void onRead(uerr code, char const * buffer, std::size_t size) override;
    virtual void onClose() override;
};

/**
 * TcpNetServer class prototype.
 *
 * @author zer0
 * @date   2017-05-05
 */
class TBAG_API TcpNetServer : public Server
{
private:
    // Insert member variables.

public:
    TcpNetServer(Loop & loop);
    virtual ~TcpNetServer();

public:
    virtual Type getType() const override;
};

} // namespace details
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DETAILS_TCPNETSERVER_HPP__

