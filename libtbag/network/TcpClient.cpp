/**
 * @file   TcpClient.cpp
 * @brief  TcpClient class implementation.
 * @author zer0
 * @date   2016-12-29
 */

#include <libtbag/network/TcpClient.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {

TcpClient::TcpClient()
{
    // EMPTY.
}

TcpClient::~TcpClient()
{
    // EMPTY.
}

bool TcpClient::initIpv4(std::string const & ip, int port)
{
    if (atTcp().isInit() == false) {
        return false;
    }

    sockaddr_in addr;
    if (atTcp().initAddress(ip, port, &addr) == false) {
        return false;
    }

    if (atTcp().connect(_connector, (sockaddr const *)&addr) == false) {
        return false;
    }

    return true;
}

bool TcpClient::run(std::string const & ip, int port)
{
    if (Tcp::isIpv4(ip) && initIpv4(ip, port)) {
        return _loop.run();
    }
    return false;
}

} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

