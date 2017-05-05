/**
 * @file   TcpNetClient.hpp
 * @brief  TcpNetClient class prototype.
 * @author zer0
 * @date   2017-05-05
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DETAILS_TCPNETCLIENT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DETAILS_TCPNETCLIENT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/network/Client.hpp>

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
class ClientImpl;
class TcpNetClient;

/**
 * ClientImpl class prototype.
 *
 * @author zer0
 * @date   2017-05-05
 */
class TBAG_API ClientImpl : public uvpp::Tcp, public details::NetTypes
{
public:
    friend class TcpNetClient;

private:
    TcpNetClient & _parent;

    ConnectRequest _connect_req;
    WriteRequest   _write_req;

    Buffer _buffer;

public:
    ClientImpl(Loop & loop, TcpNetClient & parent);
    virtual ~ClientImpl();

public:
    virtual void onConnect(ConnectRequest & request, uerr code) override;
    virtual void onWrite(WriteRequest & request, uerr code) override;
    virtual binf onAlloc(std::size_t suggested_size) override;
    virtual void onRead(uerr code, char const * buffer, std::size_t size) override;
    virtual void onClose() override;
};

/**
 * TcpNetClient class prototype.
 *
 * @author zer0
 * @date   2017-05-05
 */
class TBAG_API TcpNetClient : public Client
{
public:
    TcpNetClient(Loop & loop);
    virtual ~TcpNetClient();

public:
    virtual Type getType() const override;
};

} // namespace details
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DETAILS_TCPNETCLIENT_HPP__

