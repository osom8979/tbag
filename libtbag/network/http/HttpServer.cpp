/**
 * @file   HttpServer.cpp
 * @brief  HttpServer class implementation.
 * @author zer0
 * @date   2017-05-22
 */

#include <libtbag/network/http/HttpServer.hpp>
#include <libtbag/log/Log.hpp>

#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {

HttpServer::HttpServer(Loop & loop, StreamType type) : Parent(loop, type)
{
    // EMPTY.
}

HttpServer::~HttpServer()
{
    // EMPTY.
}

void HttpServer::onConnection(Err code)
{
    if (code != Err::E_SUCCESS) {
        tDLogE("HttpServer::onConnection() {} error.", getErrName(code));
        return;
    }

    auto node = this->accept();
    if (auto shared = node.lock()) {
        if (shared->start()) {
            if (static_cast<bool>(_open_cb)) {
                _open_cb(node);
            }
        } else {
            tDLogE("HttpServer::onConnection() Start client error.");
        }
    } else {
        tDLogE("HttpServer::onConnection() Expired client.");
    }
}

void HttpServer::onClientRead(WeakClient node, Err code, char const * buffer, std::size_t size)
{
    auto shared = node.lock();
    if (static_cast<bool>(shared) == false) {
        tDLogE("HttpServer::onClientRead() Expired client.");
        return;
    }

    ClientData * client_data = static_cast<ClientData*>(shared->getUserData());
    if (client_data == nullptr) {
        tDLogE("HttpServer::onClientRead() User data is nullptr.");
        return;
    }

    if (code != Err::E_SUCCESS /* with Err::E_EOF*/) {
        shared->stop();
        shared->close();
        return;
    }

    HttpParser  & request  = client_data->parser;
    HttpBuilder & response = client_data->builder;

    request.execute(buffer, size);
    if (request.isComplete()) {
        if (static_cast<bool>(_request_cb)) {
            uint64_t timeout = 25000U;
            _request_cb(code, request, response, timeout);

            auto buffer = response.toResponseDefaultString();
            if (shared->write(buffer.data(), buffer.size(), timeout) == false) {
                tDLogW("HttpServer::onClientRead() Write error.");
            }
        }
    }
}

void HttpServer::onClientClose(WeakClient node)
{
    if (auto shared = node.lock()) {
        //shared->stop();
        if (static_cast<bool>(_close_cb)) {
            _close_cb(node);
        }
    } else {
        tDLogE("HttpServer::onClientClose() Expired client.");
    }
}

void HttpServer::onClientShutdown(WeakClient node, Err code)
{
    // EMPTY.
}

void HttpServer::onClientWrite(WeakClient node, Err code)
{
    // EMPTY.
}

void HttpServer::onServerClose()
{
    // EMPTY.
}

void * HttpServer::onClientUserDataAlloc(WeakClient node)
{
    auto * client_data = new (std::nothrow) ClientData;
    assert(client_data != nullptr);
    return client_data;
}

void HttpServer::onClientUserDataDealloc(WeakClient node, void * data)
{
    auto * client_data = static_cast<ClientData*>(data);
    assert(client_data != nullptr);
    delete client_data;
}

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

