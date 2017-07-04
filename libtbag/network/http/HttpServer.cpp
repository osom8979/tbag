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

bool HttpServer::createCacheData(Id id)
{
    return _cache_map.insert(CacheDataPair(id, SharedCacheData(new (std::nothrow) CacheData))).second;
}

bool HttpServer::removeCacheData(Id id)
{
    return _cache_map.erase(id) == 1U;
}

HttpServer::WeakCacheData HttpServer::getCacheData(Id id)
{
    auto itr = _cache_map.find(id);
    if (itr == _cache_map.end()) {
        return WeakCacheData();
    }
    return WeakCacheData(itr->second);
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

bool HttpServer::isUpgradeWebSocket(CacheData const & cache) const TBAG_NOEXCEPT
{
    return _use_websocket && cache.upgrade;
}

void HttpServer::runWebSocketOpen(SharedClient node, Err code, ReadPacket const & packet, CacheData & cache)
{
    cache.upgrade = true;

    auto & request  = cache.parser;
    auto & response = cache.builder;

    uint64_t timeout = DEFAULT_WRITE_TIMEOUT_MILLISECOND;
    if (_callback != nullptr) {
        HP hp(request, response, timeout);
        _callback->onWsOpen(node, code, hp);
    }

    auto const RESPONSE = response.buildDefaultResponseString();
    Err const WRITE_CODE = node->write(RESPONSE.data(), RESPONSE.size(), timeout);
    if (WRITE_CODE != Err::E_SUCCESS) {
        tDLogW("HttpServer::runWebSocketOpen() WebSocket response write {} error.", getErrName(WRITE_CODE));
    }

    request.clear();
    request.clearCache();
    response.clear();
}

void HttpServer::runWebSocketRead(SharedClient node, Err code, ReadPacket const & packet, CacheData & cache)
{
    auto & rframe  = cache.recv_frame;
    auto & wframe  = cache.send_frame;
    auto & fbuffer = cache.buffer;

    Err const EXECUTE_CODE = rframe.execute((uint8_t*)packet.buffer, packet.size);
    if (EXECUTE_CODE != Err::E_SUCCESS) {
        tDLogE("HttpServer::runWebSocketRead() WebSocket frame {} error", getErrName(EXECUTE_CODE));
        return;
    }

    if (rframe.fin == false) {
        // Waiting next frame ...
        return;
    }

    assert(rframe.fin);
    if (rframe.opcode == OpCode::OC_TEXT_FRAME || rframe.opcode == OpCode::OC_BINARY_FRAME) {
        if (_callback != nullptr) {
            char const * data = (char const *)rframe.getPayloadData();
            std::size_t size = rframe.getPayloadSize();
            WP wp(rframe.opcode, data, size);
            _callback->onWsMessage(node, code, wp);
        }
        return;
    }

    // Processing control frames.
    if (rframe.opcode == OpCode::OC_CONNECTION_CLOSE) {
        wframe.closeResponse();
        wframe.copyTo(fbuffer);
    } else if (rframe.opcode == OpCode::OC_DENOTES_PING) {
        wframe.pongResponse(rframe.getPayloadData(), rframe.getPayloadSize());
        wframe.copyTo(fbuffer);
    } else if (rframe.opcode == OpCode::OC_DENOTES_PONG) {
        // Ping & Pong checker.
    } else {
        tDLogW("HttpServer::runWebSocketRead() Unsupported control frame: {}", getOpCodeName(rframe.opcode));
    }
}

void HttpServer::runHttpRead(SharedClient node, Err code, ReadPacket const & packet, CacheData & cache)
{
    auto & request  = cache.parser;
    auto & response = cache.builder;

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

    auto const RESPONSE = response.buildDefaultResponseString();
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

    if (createCacheData(shared->id()) == false) {
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

    SharedCacheData dataset = getCacheData(shared->id()).lock();
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
        if (updateResponseWebSocket(request, response) == Err::E_SUCCESS) {
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

    if (removeCacheData(shared->id()) == false) {
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
        return _callback->onUdataAlloc(node);
    }
    return nullptr;
}

void HttpServer::onClientUdataDealloc(WeakClient node, void * data)
{
    if (_callback != nullptr) {
        return _callback->onUdataDealloc(node, data);
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

