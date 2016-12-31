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

bool TcpClient::asyncWrite(binf * infos, std::size_t infos_size)
{
    return Parent::asyncWrite(atTcp(), obtainWriteRequest(atTcp()), infos, infos_size);
}

bool TcpClient::asyncWrite(char const * buffer, std::size_t size)
{
    return Parent::asyncWrite(atTcp(), obtainWriteRequest(atTcp()), buffer, size);
}

bool TcpClient::run(std::string const & ip, int port)
{
    if (uv::Tcp::isIpv4(ip) && initIpv4(ip, port)) {
        return _loop.run();
    }
    return false;
}

TcpClient::binf TcpClient::onAlloc(std::size_t suggested_size)
{
    // Realloc with read buffer.
    if (_read_buffer.size() < suggested_size) {
        _read_buffer.resize(suggested_size);
    }

    binf info;
    info.buffer = &_read_buffer[0];
    info.size   =  _read_buffer.size();
    return info;
}


} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

