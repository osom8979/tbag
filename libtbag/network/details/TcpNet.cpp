/**
 * @file   TcpNet.cpp
 * @brief  TcpNet class implementation.
 * @author zer0
 * @date   2017-05-03
 */

#include <libtbag/network/details/TcpNet.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/uvpp/Loop.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace details {

using Loop = uvpp::Loop;

// ----------------------------
// TcpNetClient implementation.
// ----------------------------

TcpNetClient::TcpNetClient(Loop & loop) : Client(loop)
{
}

TcpNetClient::~TcpNetClient()
{
}

TcpNetClient::Type TcpNetClient::getType() const
{
    return Type::TCP;
}

// ----------------------------
// TcpNetServer implementation.
// ----------------------------

TcpNetServer::TcpNetServer(Loop & loop) : Server(loop)
{
}

TcpNetServer::~TcpNetServer()
{
}

TcpNetServer::Type TcpNetServer::getType() const
{
    return Type::TCP;
}

} // namespace details
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

