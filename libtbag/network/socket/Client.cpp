/**
 * @file   Client.cpp
 * @brief  Client class implementation.
 * @author zer0
 * @date   2016-10-14
 */

#include <libtbag/network/socket/Client.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/loop/UvEventDispatcher.hpp>

#include <cstring>
#include <vector>

#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace socket  {

Client::Client(Loop & loop, EventCallback * callback)
        : Tcp(loop)
        , _writer(uv::UvRequestType::WRITE, this)
        , _read_buffer()
        , _callback(callback)
{
    // EMPTY.
}

Client::Client(Loop & loop) : Client(loop, nullptr)
{
    // EMPTY.
}

Client::~Client()
{
    // EMPTY.
}

bool Client::init(std::string const & ip, int port)
{
    if (isIpv4(ip)) {
        return initIpv4(ip, port);
    }
    return false;
}

bool Client::initIpv4(std::string const & ip, int port)
{
    if (isInit() == false) {
        return false;
    }

    sockaddr_in addr;
    if (initAddress(ip, port, &addr) == false) {
        return false;
    }

    if (connect(_connector, (sockaddr const *)&addr) == false) {
        return false;
    }

    return true;
}

bool Client::autoWrite(char const * buffer, std::size_t size)
{
    auto request = _writer.create().lock();
    return write(*static_cast<uv::WriteRequest*>(request.get()), buffer, size);
}

// ------------------
// Tcp Event methods.
// ------------------

void Client::onConnect(Err code)
{
    if (_callback != nullptr) {
        _callback->onConnect(code);
    }
}

Client::BufferInfo Client::onAlloc(std::size_t suggested_size)
{
    // Realloc with read buffer.
    if (_read_buffer.size() < suggested_size) {
        _read_buffer.resize(suggested_size);
    }

    BufferInfo info;
    info.buffer = &_read_buffer[0];
    info.size   =  _read_buffer.size();
    return info;
}

void Client::onRead(Err code, char const * buffer, std::size_t size)
{
    if (_callback != nullptr) {
        _callback->onRead(code, buffer, size);
    }
}

void Client::onWrite(Err code)
{
    if (_callback != nullptr) {
        _callback->onWrite(code);
    }
}

void Client::onClose()
{
    if (_callback != nullptr) {
        _callback->onClose();
    }
}

} // namespace socket
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

