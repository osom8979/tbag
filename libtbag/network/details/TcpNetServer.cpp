/**
 * @file   TcpNetServer.cpp
 * @brief  TcpNetServer class implementation.
 * @author zer0
 * @date   2017-05-05
 */

#include <libtbag/network/details/TcpNetServer.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace details {

// ----------------------------
// TcpNetServer implementation.
// ----------------------------

TcpNetServer::TcpNetServer(Loop & loop) : Server(loop)
{
    // EMPTY.
}

TcpNetServer::~TcpNetServer()
{
    // EMPTY.
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

