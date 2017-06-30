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

bool HttpServer::isUpgradeWebSocket(ClientData const & client_data) const TBAG_NOEXCEPT
{
    return _use_websocket && client_data.websocket.upgrade;
}

void HttpServer::runWebSocketOpen(SharedClient node, Err code, ReadPacket const & packet, ClientData & client_data)
{
    client_data.websocket.upgrade = true;

    auto & request  = client_data.parser;
    auto & response = client_data.builder;

    uint64_t timeout = DEFAULT_WRITE_TIMEOUT_MILLISECOND;
    if (_callback != nullptr) {
        HP hp(request, response, timeout);
        _callback->onWebSocketOpen(node, code, hp);
    }

    auto const RESPONSE = response.toDefaultResponseString();
    Err const WRITE_CODE = node->write(RESPONSE.data(), RESPONSE.size(), timeout);
    if (WRITE_CODE != Err::E_SUCCESS) {
        tDLogW("HttpServer::runWebSocketOpen() WebSocket response write {} error.", getErrName(WRITE_CODE));
    }

    request.clear();
    request.clearCache();
    response.clear();
}

void HttpServer::runWebSocketRead(SharedClient node, Err code, ReadPacket const & packet, ClientData & client_data)
{
    auto & recv_frame   = client_data.websocket.recv_frame;
    auto & write_frame  = client_data.websocket.write_frame;
    auto & frame_buffer = client_data.websocket.frame_buffer;

    Err const EXECUTE_CODE = recv_frame.execute((uint8_t*)packet.buffer, packet.size);
    if (EXECUTE_CODE != Err::E_SUCCESS) {
        tDLogE("HttpServer::runWebSocketRead() WebSocket frame {} error", getErrName(EXECUTE_CODE));
        return;
    }

    uint64_t timeout = DEFAULT_WRITE_TIMEOUT_MILLISECOND;
    if (_callback != nullptr) {
        WP wp(recv_frame, write_frame, timeout);
        _callback->onWebSocketMessage(node, code, wp);
    }

    std::size_t const RESERVE_SIZE = write_frame.calculateWriteBufferSize();
    if (frame_buffer.size() < write_frame.calculateWriteBufferSize()) {
        frame_buffer.resize(RESERVE_SIZE);
    }

    if (write_frame.write(frame_buffer.data(), frame_buffer.size()) == 0) {
        tDLogE("HttpServer::runWebSocketRead() WebSocket frame write error.");
        return;
    }

    Err write_code = node->write((char const *)frame_buffer.data(), frame_buffer.size(), timeout);
    if (write_code != Err::E_SUCCESS) {
        tDLogW("HttpServer::runWebSocketRead() WebSocket response write {} error.", getErrName(write_code));
        return;
    }
}

void HttpServer::runHttpRead(SharedClient node, Err code, ReadPacket const & packet, ClientData & client_data)
{
    auto & request  = client_data.parser;
    auto & response = client_data.builder;

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
            HP hp(request, response, timeout);
            shared->request_cb(node, code, hp);
        }

        called = true;
        break;
    }

    if (called == false) {
        if (_callback != nullptr) {
            // Default request callback.
            HP hp(request, response, timeout);
            _callback->onRequest(node, code, hp);
        } else {
            tDLogW("HttpServer::runHttpRead() Not found request callback.");
        }
    }

    auto const RESPONSE = response.toDefaultResponseString();
    Err const WRITE_CODE = node->write(RESPONSE.data(), RESPONSE.size(), timeout);
    if (WRITE_CODE != Err::E_SUCCESS) {
        tDLogW("HttpServer::onClientRead() Write {} error.", getErrName(WRITE_CODE));
    }

    request.clear();
    request.clearCache();
    response.clear();
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

    if (_callback != nullptr) {
        _callback->onOpen(node);
    }
}

void HttpServer::onClientRead(WeakClient node, Err code, ReadPacket const & packet)
{
    auto shared = node.lock();
    if (static_cast<bool>(shared) == false) {
        tDLogC("HttpServer::onClientRead() Expired client.");
        return;
    }

    if (code == Err::E_EOF) {
        tDLogD("HttpServer::onClientRead() EOF.");
        shared->close();
        return;
    }

    SharedClientData dataset = getClientData(shared->id()).lock();
    if (static_cast<bool>(dataset) == false) {
        tDLogC("HttpServer::onClientRead() Expired client data.");
        shared->close();
        return;
    }

    if (code != Err::E_SUCCESS) {
        tDLogE("HttpServer::onClientRead() {} error", getErrName(code));
        shared->close();
        return;
    }

    if (isUpgradeWebSocket(*dataset)) {
        // WebSocket interrupt process (WebSocket Frame).
        runWebSocketRead(shared, code, packet, *dataset);
        return;
    }

    HttpParser  & request  = dataset->parser;
    HttpBuilder & response = dataset->builder;

    request.execute(packet.buffer, packet.size);
    if (request.isComplete() == false) {
        tDLogD("HttpServer::onClientRead() Not complete.");
        return;
    }

    // WebSocket checker.
    if (_use_websocket && request.isUpgrade()) {
        // WebSocket interrupt process (HTTP Request).
        if (getResponseWebSocket(request, response) == Err::E_SUCCESS) {
            tDLogI("HttpServer::onClientRead() Request WebSocket header.");
            runWebSocketOpen(shared, code, packet, *dataset);
            return;
        } else {
            tDLogW("HttpServer::onClientRead() Unknown WebSocket request. Switches to the regular HTTP protocol.");
        }
    }

    // Regular HTTP process.
    runHttpRead(shared, code, packet, *dataset);
}

void HttpServer::onClientClose(WeakClient node)
{
    auto shared = node.lock();
    if (static_cast<bool>(shared) == false) {
        tDLogE("HttpServer::onClientClose() Expired client.");
        return;
    }

    if (_callback != nullptr) {
        _callback->onClose(node);
    }

    if (removeClientData(shared->id()) == false) {
        tDLogW("HttpServer::onClientClose() Client-data removal failed.");
    }
}

void HttpServer::onClientShutdown(WeakClient node, Err code)
{
    if (_callback != nullptr) {
        _callback->onShutdown(node, code);
    }
}

void HttpServer::onClientWrite(WeakClient node, Err code)
{
    if (_callback != nullptr) {
        _callback->onWrite(node, code);
    }
}

void * HttpServer::onClientUdataAlloc(WeakClient node)
{
    if (_callback != nullptr) {
        return _callback->onUserDataAlloc(node);
    }
    return nullptr;
}

void HttpServer::onClientUdataDealloc(WeakClient node, void * data)
{
    if (_callback != nullptr) {
        return _callback->onUserDataDealloc(node, data);
    }
}

void HttpServer::onClose()
{
    if (_callback != nullptr) {
        return _callback->onServerClose();
    }
}

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

