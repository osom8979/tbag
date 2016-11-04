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

Server::Server()
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

bool Server::read()
{
    return _tcp.read();
}

bool Server::write(ClientKey id, char const * buffer, std::size_t length)
{
    auto find_itr = _clients.find(id);
    if (find_itr != _clients.end()) {
        if (static_cast<bool>(find_itr->second)) {
            find_itr->second->write(buffer, length);
        }
    }
    return false;
}

bool Server::close(ClientKey id)
{
    auto find_itr = _clients.find(id);
    if (find_itr != _clients.end()) {
        if (static_cast<bool>(find_itr->second)) {
            find_itr->second->closeWrite();
            find_itr->second->closeConnect();
            find_itr->second->closeTcp();
        }
        _clients.erase(find_itr);
        return true;
    }
    return false;
}

Server::ClientKey Server::accept()
{
    SharedClient client(new Tcp());
    if (_clients.insert(ClientMap::value_type(client->getId(), client)).second) {
        return client->getId();
    }
    return getErrorKey();
}

void Server::close()
{
    _tcp.closeWrite();
    _tcp.closeConnect();
    _tcp.closeTcp();
}

} // namespace socket
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

