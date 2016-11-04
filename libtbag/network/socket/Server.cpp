/**
 * @file   Server.cpp
 * @brief  Server class implementation.
 * @author zer0
 * @date   2016-10-14
 */

#include <libtbag/network/socket/Server.hpp>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace socket  {

// --------------------------------------
// Server::AcceptedClient implementation.
// --------------------------------------

Server::AcceptedClient::AcceptedClient(Server & parent) : Tcp(this), _parent(parent)
{
    // EMPTY.
}

Server::AcceptedClient::~AcceptedClient()
{
    // EMPTY.
}

void Server::AcceptedClient::onClose()
{
    _parent.onClientClose(WeakClient(shared_from_this()));
}

void Server::AcceptedClient::onRead(ReadErrorCode code, char * buffer, std::size_t length)
{
    _parent.onClientRead(WeakClient(shared_from_this()), code, buffer, length);
}

void Server::AcceptedClient::onWrite(WriteErrorCode code)
{
    _parent.onClientWrite(WeakClient(shared_from_this()), code);
}

// ----------------------
// Server implementation.
// ----------------------

Server::Server() : _tcp(this)
{
    // EMPTY.
}

Server::~Server()
{
    // EMPTY.
}

bool Server::run(std::string const & ip, int port)
{
    if (Tcp::isIpv4(ip)) {
        return runIpv4(ip, port);
    } else {
        return runIpv6(ip, port);
    }
}

bool Server::runIpv4(std::string const & ip, int port)
{
    if (_tcp.init(_loop) && _tcp.bindIpv4(ip, port) && _tcp.listen()) {
        return _loop.runDefault();
    }
    return false;
}

bool Server::runIpv6(std::string const & ip, int port)
{
    if (_tcp.init(_loop) && _tcp.bindIpv6(ip, port) && _tcp.listen()) {
        return _loop.runDefault();
    }
    return false;
}

void Server::close()
{
    _tcp.close();
}

Server::WeakClient Server::accept()
{
    SharedClient client(new AcceptedClient(*this));
    client->init(_loop);
    if (_tcp.accept(*client.get())) {
        if (_clients.insert(ClientMap::value_type(client->getId(), client)).second) {
            return WeakClient(client);
        }
    }
    return WeakClient();
}

} // namespace socket
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

