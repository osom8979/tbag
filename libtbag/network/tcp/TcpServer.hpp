/**
 * @file   TcpServer.hpp
 * @brief  TcpServer class prototype.
 * @author zer0
 * @date   2017-05-10
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_TCP_TCPSERVER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_TCP_TCPSERVER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/network/stream/StreamServer.hpp>
#include <libtbag/network/tcp/TcpClient.hpp>
#include <libtbag/uvpp/Tcp.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace tcp     {

/**
 * TcpServer class prototype.
 *
 * @author zer0
 * @date   2017-05-05
 */
struct TcpServer : public stream::StreamServer
{
    TcpServer(Loop & loop) : stream::StreamServer(loop, StreamType::TCP)
    { /* EMPTY. */ }
};

/**
 * FunctionalTcpServer typedef.
 *
 * @author zer0
 * @date   2017-05-08
 */
using FunctionalTcpServer = FunctionalServer<TcpServer>;

} // namespace tcp
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_TCP_TCPSERVER_HPP__

