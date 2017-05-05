/**
 * @file   TcpServer.hpp
 * @brief  TcpServer class prototype.
 * @author zer0
 * @date   2017-05-06
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DETAILS_TCPSERVER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DETAILS_TCPSERVER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/network/Server.hpp>
#include <libtbag/network/details/NetCommon.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace details {

// Forward declaration.
class TcpRealServer;
class TcpRealNode;
class TcpServer;

/**
 * TcpRealServer class prototype.
 *
 * @author zer0
 * @date   2017-05-05
 */
class TBAG_API TcpRealServer : public details::NetCommon, public uvpp::Tcp
{
public:
    friend class TcpServer;

private:
    TcpServer & _parent;

public:
    TcpRealServer(Loop & loop, TcpServer & parent);
    virtual ~TcpRealServer();

public:
    virtual void onConnection(uerr code) override;
    virtual void onClose() override;
};

/**
 * TcpRealNode class prototype.
 *
 * @author zer0
 * @date   2017-05-05
 */
class TBAG_API TcpRealNode : public details::NetCommon, public uvpp::Tcp
{
public:
    friend class TcpServer;

private:
    TcpServer  & _parent;
    WriteRequest _write_req;
    Buffer       _buffer;

public:
    TcpRealNode(Loop & loop, TcpServer & parent);
    virtual ~TcpRealNode();

public:
    virtual void onWrite(WriteRequest & request, uerr code) override;
    virtual binf onAlloc(std::size_t suggested_size) override;
    virtual void onRead(uerr code, char const * buffer, std::size_t size) override;
    virtual void onClose() override;
};

/**
 * TcpServer class prototype.
 *
 * @author zer0
 * @date   2017-05-05
 */
class TBAG_API TcpServer : public Server
{
private:
    // Insert member variables.

public:
    TcpServer(Loop & loop);
    virtual ~TcpServer();

public:
    virtual Type getType() const
    { return Type::TCP; }
};

} // namespace details
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DETAILS_TCPSERVER_HPP__

