/**
 * @file   Client.cpp
 * @brief  Client class implementation.
 * @author zer0
 * @date   2016-10-14
 */

#include <libtbag/network/socket/Client.hpp>
#include <cstring>
#include <vector>
#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace socket  {

Client::Client()
{
    // EMPTY.
}

Client::~Client()
{
    // EMPTY.
}

bool Client::run(std::string const & ip, int port)
{
    if (Tcp::isIpv4(ip)) {
        return runIpv4(ip, port);
    } else {
        return runIpv6(ip, port);
    }
}

bool Client::runIpv4(std::string const & ip, int port)
{
    if (_tcp.init(_loop) && _tcp.connectIpv4(ip, port)) {
        return _loop.runDefault();
    }
    return false;
}

bool Client::runIpv6(std::string const & ip, int port)
{
    if (_tcp.init(_loop) && _tcp.connectIpv6(ip, port)) {
        return _loop.runDefault();
    }
    return false;
}

bool Client::read()
{
    return _tcp.read();
}

bool Client::write(char const * buffer, std::size_t length)
{
    return _tcp.write(buffer, length);
}

void Client::close()
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

