/**
 * @file   HttpServer.cpp
 * @brief  HttpServer class implementation.
 * @author zer0
 * @date   2017-05-22
 */

#include <libtbag/network/http/HttpServer.hpp>
#include <libtbag/log/Log.hpp>

#include <cassert>
#include "HttpServer.hpp"

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {

TBAG_CONSTEXPR static uint64_t const DEFAULT_WRITE_TIMEOUT_MILLISECOND = 5 * 1000;

HttpServer::HttpServer(Loop & loop, StreamType type) : Parent(loop, type), _use_websocket(false)
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

    if (shared->start() != Err::E_SUCCESS) {
        tDLogE("HttpServer::onConnection() Start client error.");
        shared->close();
        return;
    }

    if (createClientData(shared->id()) == false) {
        tDLogE("HttpServer::onConnection() Bad allocated client-data.");
        shared->close();
        return;
    }

    if (static_cast<bool>(_open_cb)) {
        _open_cb(node);
    }
}

void HttpServer::onClientRead(WeakClient node, Err code, ReadPacket const & packet)
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

    auto dataset = getClientData(shared->id()).lock();
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

    if (_use_websocket && dataset->websocket.upgrade) {
        // WebSocket interrupt process (WebSocket Frame).
        WebSocketFrame & recv_frame = dataset->websocket.recv_frame;
        WebSocketFrame & write_frame = dataset->websocket.write_frame;
        WebSocketFrame::Buffer & frame_buffer = dataset->websocket.frame_buffer;

        if (recv_frame.execute((uint8_t*)packet.buffer, packet.size) != Err::E_SUCCESS) {
            tDLogE("HttpServer::onClientRead() WebSocket frame {} error", getErrName(code));
            return;
        }

        uint64_t timeout = DEFAULT_WRITE_TIMEOUT_MILLISECOND;
        if (static_cast<bool>(_on_web_socket_message)) {
            _on_web_socket_message(code, node, recv_frame, write_frame, timeout);
        }

        std::size_t const RESERVE_SIZE = write_frame.calculateWriteBufferSize();
        if (frame_buffer.size() < write_frame.calculateWriteBufferSize()) {
            frame_buffer.resize(RESERVE_SIZE);
        }

        if (write_frame.write(frame_buffer.data(), frame_buffer.size()) == 0) {
            tDLogE("HttpServer::onClientRead() WebSocket frame write error.");
            return;
        }

        if (shared->write((char const *)frame_buffer.data(), frame_buffer.size(), timeout) != Err::E_SUCCESS) {
            tDLogW("HttpServer::onClientRead() WebSocket response write error.");
        }
        return;
    }

    HttpParser  & request  = dataset->parser;
    HttpBuilder & response = dataset->builder;

    request.execute(packet.buffer, packet.size);
    if (request.isComplete() == false) {
        tDLogD("HttpServer::onClientRead() Not complete.");
        return;
    }

    // ------------------
    // WebSocket process.
    // ------------------

    if (_use_websocket && request.isUpgrade()) {
        // WebSocket interrupt process (HTTP Request).
        if (getResponseWebSocket(request, response) == Err::E_SUCCESS) {
            tDLogI("HttpServer::onClientRead() Request WebSocket header.");
            dataset->websocket.upgrade = true;

            uint64_t timeout = DEFAULT_WRITE_TIMEOUT_MILLISECOND;
            if (static_cast<bool>(_on_web_socket_open)) {
                _on_web_socket_open(code, node, request, response, timeout);
            }

            auto const RESPONSE = response.toDefaultResponseString();
            if (shared->write(RESPONSE.data(), RESPONSE.size(), timeout) != Err::E_SUCCESS) {
                tDLogW("HttpServer::onClientRead() WebSocket response write error.");
            }
            request.clear();
            request.clearCache();
            response.clear();
            return;
        } else {
            tDLogW("HttpServer::onClientRead() Unknown WebSocket request. Switches to the regular HTTP protocol.");
        }
    }

    // -----------------
    // Response process.
    // -----------------

    uint64_t timeout = DEFAULT_WRITE_TIMEOUT_MILLISECOND;
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
    if (shared->write(RESPONSE.data(), RESPONSE.size(), timeout) != Err::E_SUCCESS) {
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

    if (removeClientData(shared->id()) == false) {
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

void * HttpServer::onClientUdataAlloc(WeakClient node)
{
    if (static_cast<bool>(_user_data_alloc_cb)) {
        return _user_data_alloc_cb(node);
    }
    return nullptr;
}

void HttpServer::onClientUdataDealloc(WeakClient node, void * data)
{
    if (static_cast<bool>(_user_data_dealloc_cb)) {
        _user_data_dealloc_cb(node, data);
    }
}

void HttpServer::onClose()
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

