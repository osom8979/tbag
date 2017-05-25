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

void HttpServer::setOnRequest(std::string const & method, std::string const & regex_path, OnRequest const & cb, int priority)
{
    setOnRequest(new (std::nothrow) HttpDefaultFilter(method, regex_path), cb);
}

void HttpServer::setOnRequest(std::string const & regex_path, OnRequest const & cb, int priority)
{
    setOnRequest(new (std::nothrow) HttpDefaultFilter(regex_path), cb);
}

void HttpServer::setOnRequest(HttpFilterInterface * filter, OnRequest const & cb, int priority)
{
    _filters.insert(FilterPair(priority, FilterContainer(filter, cb)));
}

void HttpServer::onConnection(Err code)
{
    if (code != Err::E_SUCCESS) {
        tDLogE("HttpServer::onConnection() {} error.", getErrName(code));
        return;
    }

    auto node = this->accept();
    auto shared = node.lock();

    if (static_cast<bool>(shared) == false) {
        tDLogE("HttpServer::onConnection() Expired client.");
        return;
    }

    if (shared->start() == false) {
        tDLogE("HttpServer::onConnection() Start client error.");
        shared->close();
        return;
    }

    if (static_cast<bool>(_open_cb)) {
        _open_cb(node);
    }
}

void HttpServer::onClientRead(WeakClient node, Err code, char const * buffer, std::size_t size)
{
    auto shared = node.lock();
    if (static_cast<bool>(shared) == false) {
        tDLogE("HttpServer::onClientRead() Expired client.");
        return;
    }

    if (code == Err::E_EOF) {
        tDLogD("HttpServer::onClientRead() EOF.");
        shared->close();
        return;
    }

    ClientData * client_data = static_cast<ClientData*>(shared->getUserData());
    if (client_data == nullptr) {
        tDLogE("HttpServer::onClientRead() User data is nullptr.");
        shared->close();
        return;
    }

    if (code != Err::E_SUCCESS) {
        tDLogE("HttpServer::onClientRead() {} error", getErrName(code));
        shared->close();
        return;
    }

    HttpParser  & request  = client_data->parser;
    HttpBuilder & response = client_data->builder;

    request.execute(buffer, size);
    if (request.isComplete() == false) {
        tDLogD("HttpServer::onClientRead() Not complete.");
        return;
    }

    // -----------------
    // Response process.
    // -----------------

    uint64_t timeout = 25000U;
    bool called = false;

    for (auto & filter : _filters) {
        if (static_cast<bool>(filter.second.filter) && filter.second.filter->filter(request)) {
            if (static_cast<bool>(filter.second.request_cb)) {
                filter.second.request_cb(code, request, response, timeout);
            }
            called = true;
            break;
        }
    }

    if (called == false && static_cast<bool>(_request_cb)) {
        _request_cb(code, request, response, timeout);
        called = true;
    }

    if (called == false) {
        tDLogW("HttpServer::onClientRead() Not found request callback.");
    }

    auto const RESPONSE = response.toDefaultResponseString();
    if (shared->write(RESPONSE.data(), RESPONSE.size(), timeout) == false) {
        tDLogW("HttpServer::onClientRead() Write error.");
    }

    request.clear();
    request.clearCache();
    response.clear();
}

void HttpServer::onClientClose(WeakClient node)
{
    auto shared = node.lock();
    if (static_cast<bool>(shared) == false) {
        tDLogE("HttpServer::onClientClose() Expired client.");
        return;
    }

    if (static_cast<bool>(_close_cb)) {
        _close_cb(node);
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

