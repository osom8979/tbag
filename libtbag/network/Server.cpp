/**
 * @file   Server.cpp
 * @brief  Server class implementation.
 * @author zer0
 * @date   2017-05-02
 */

#include <libtbag/network/Server.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/uvpp/Loop.hpp>

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
    return SharedServer();
}

} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

