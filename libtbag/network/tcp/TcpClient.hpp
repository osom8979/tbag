/**
 * @file   TcpClient.hpp
 * @brief  TcpClient class prototype.
 * @author zer0
 * @date   2017-05-10
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_TCP_TCPCLIENT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_TCP_TCPCLIENT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/network/stream/StreamClient.hpp>
#include <libtbag/uvpp/Tcp.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace tcp     {

/**
 * TcpClient class prototype.
 *
 * @author zer0
 * @date   2017-05-05
 */
class TcpClient : public stream::StreamClient<uvpp::Tcp>
{
public:
    using Parent = stream::StreamClient<uvpp::Tcp>;

public:
    TcpClient(Loop & loop) : Parent(loop)
    { /* EMPTY. */ }

    virtual ~TcpClient()
    { /* EMPTY. */ }

public:
    virtual Type getType() const override
    { return Type::TCP; }

    virtual bool realInitialize(ClientBackend & backend, String const & ip, int port) override
    { return uvpp::initCommonClient(backend, backend.atConnectReq(), ip, port); }
};

/**
 * FunctionalTcpClient typedef.
 *
 * @author zer0
 * @date   2017-05-08
 */
using FunctionalTcpClient = FunctionalClient<TcpClient>;

} // namespace tcp
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_TCP_TCPCLIENT_HPP__

