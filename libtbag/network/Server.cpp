/**
 * @file   Server.cpp
 * @brief  Server class implementation.
 * @author zer0
 * @date   2017-05-02
 */

#include <libtbag/network/Server.hpp>
#include <libtbag/network/details/TcpNet.hpp>
#include <libtbag/network/details/UdpNet.hpp>
#include <libtbag/network/details/PipeNet.hpp>
#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {

Server::Server(Loop & loop)
{
    // EMPTY.
}

Server::~Server()
{
    // EMPTY.
}

Server::SharedServer Server::create(Loop & loop, NetType type)
{
    // @formatter:off
    switch (type) {
    case NetType::TCP:  return SharedServer(new (std::nothrow) details::TcpNetServer(loop));
    case NetType::UDP:  return SharedServer(new (std::nothrow) details::UdpNetServer(loop));
    case NetType::PIPE: return SharedServer(new (std::nothrow) details::PipeNetServer(loop));
    default:            return SharedServer();
    }
    // @formatter:on
}

} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

