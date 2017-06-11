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

bool HttpServer::createClientData(Id id)
{
    return _dataset.insert(ClientDataPair(id, SharedClientData(new (std::nothrow) ClientData))).second;
}

bool HttpServer::removeClientData(Id id)
{
    return _dataset.erase(id) == 1U;
}

HttpServer::WeakClientData HttpServer::getClientData(Id id)
{
    auto itr = _dataset.find(id);
    if (itr == _dataset.end()) {
        return WeakClientData();
    }
    return WeakClientData(itr->second);
}

void HttpServer::setOnRequest(std::string const & method, std::string const & regex_path, OnRequest const & cb, Order priority)
{
    setOnRequest(new (std::nothrow) HttpDefaultFilter(method, regex_path), cb);
}

void HttpServer::setOnRequest(std::string const & regex_path, OnRequest const & cb, Order priority)
{
    setOnRequest(new (std::nothrow) HttpDefaultFilter(regex_path), cb);
}

void HttpServer::setOnRequest(HttpFilterInterface * filter, OnRequest const & cb, Order priority)
{
    setOnRequest(SharedFilter(new (std::nothrow) Filter(filter, cb)), priority);
}

void HttpServer::setOnRequest(SharedFilter filter, Order priority)
{
    _filters.insert(FilterPair(priority, filter));
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

    if (createClientData(shared->getId()) == false) {
        tDLogE("HttpServer::onConnection() Bad allocated client-data.");
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

    auto dataset = getClientData(shared->getId()).lock();
    if (static_cast<bool>(dataset) == false) {
        tDLogE("HttpServer::onClientRead() Expired client data.");
        shared->close();
        return;
    }

    if (code != Err::E_SUCCESS) {
        tDLogE("HttpServer::onClientRead() {} error", getErrName(code));
        shared->close();
        return;
    }

    HttpParser  & request  = dataset->parser;
    HttpBuilder & response = dataset->builder;

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

    for (auto & f : _filters) {
        Order order = f.first;
        SharedFilter shared = f.second;

        if (static_cast<bool>(shared) == false || static_cast<bool>(shared->http_filter) == false) {
            continue;
        }
        if (shared->http_filter->filter(request) == false) {
            continue;
        }
        if (static_cast<bool>(shared->request_cb)) {
            shared->request_cb(code, node, request, response, timeout);
        }
        called = true;
        break;
    }

    if (called == false && static_cast<bool>(_request_cb)) {
        _request_cb(code, node, request, response, timeout); // Default request callback.
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

    if (removeClientData(shared->getId()) == false) {
        tDLogW("HttpServer::onClientClose() Client-data removal failed.");
    }
}

void HttpServer::onClientShutdown(WeakClient node, Err code)
{
    if (static_cast<bool>(_shutdown_cb)) {
        _shutdown_cb(node, code);
    }
}

void HttpServer::onClientWrite(WeakClient node, Err code)
{
    if (static_cast<bool>(_write_cb)) {
        _write_cb(node, code);
    }
}

void * HttpServer::onClientUserDataAlloc(WeakClient node)
{
    if (static_cast<bool>(_user_data_alloc_cb)) {
        return _user_data_alloc_cb(node);
    }
    return nullptr;
}

void HttpServer::onClientUserDataDealloc(WeakClient node, void * data)
{
    if (static_cast<bool>(_user_data_dealloc_cb)) {
        _user_data_dealloc_cb(node, data);
    }
}

void HttpServer::onServerClose()
{
    if (static_cast<bool>(_server_close_cb)) {
        _server_close_cb();
    }
}

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

