/**
 * @file   HttpServer.cpp
 * @brief  HttpServer class implementation.
 * @author zer0
 * @date   2017-05-22
 */

#include <libtbag/network/http/HttpServer.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/log/Log.hpp>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {

// ------------------------
// HttpNode implementation.
// ------------------------

HttpServer::HttpNode::HttpNode(Loop & loop, StreamType type, HttpServer * parent)
        : stream::StreamNode(loop, type, parent), _reader(this)
{
    // EMPTY.
}

HttpServer::HttpNode::~HttpNode()
{
    // EMPTY.
}

Err HttpServer::HttpNode::writeResponse()
{
    return writeResponse(HttpResponse());
}

Err HttpServer::HttpNode::writeResponse(HttpResponse const & response)
{
    HttpResponse update_response = response;
    libtbag::http::updateDefaultResponse(update_response);
    std::string const & RESPONSE_STRING = libtbag::http::toResponseString(update_response);
    return write(RESPONSE_STRING.data(), RESPONSE_STRING.size());
}

Err HttpServer::HttpNode::writeWsResponse(HttpRequest const & request)
{
    return writeWsResponse(request, HttpResponse());
}

Err HttpServer::HttpNode::writeWsResponse(HttpRequest const & request, HttpResponse const & response)
{
    HttpResponse ws_response = response;
    libtbag::http::updateDefaultWsResponse(ws_response, request);
    std::string const & RESPONSE_STRING = libtbag::http::toResponseString(ws_response);
    return write(RESPONSE_STRING.data(), RESPONSE_STRING.size());
}

Err HttpServer::HttpNode::writeWsFrame(WsFrame const & frame)
{
    if (_reader.isEnableWebsocket() == false) {
        return Err::E_ILLSTATE;
    }

    util::Buffer buffer;
    std::size_t const SIZE = frame.copyTo(buffer);
    if (SIZE == 0) {
        tDLogE("HttpServer::HttpNode::writeWsFrame() WsFrame -> Buffer copy error");
        return Err::E_ECOPY;
    }
    return write(buffer.data(), buffer.size());
}

Err HttpServer::HttpNode::writeText(char const * buffer, std::size_t size, bool finish)
{
    WsFrame frame;
    frame.text(buffer, size, finish);
    return writeWsFrame(frame);
}

Err HttpServer::HttpNode::writeText(std::string const & text, bool finish)
{
    return writeText(text.data(), text.size(), finish);
}

Err HttpServer::HttpNode::writeBinary(char const * buffer, std::size_t size, bool finish)
{
    WsFrame frame;
    frame.binary(buffer, size, finish);
    return writeWsFrame(frame);
}

Err HttpServer::HttpNode::writeBinary(util::Buffer const & binary, bool finish)
{
    return writeBinary(binary.data(), binary.size(), finish);
}

Err HttpServer::HttpNode::writeClose(WsStatus const & status)
{
    WsFrame frame;
    frame.close(status);
    return writeWsFrame(frame);
}

Err HttpServer::HttpNode::writeClose(WsStatusCode code)
{
    return writeClose(WsStatus(code));
}

Err HttpServer::HttpNode::writeClose()
{
    return writeClose(WsStatusCode::WSSC_NORMAL_CLOSURE);
}

void HttpServer::HttpNode::onRead(Err code, ReadPacket const & packet)
{
    auto * server = getHttpServerPtr();
    assert(server != nullptr);

    if (code == Err::E_EOF) {
        server->onClientEof(getWeakClient());
    } else if (code != Err::E_SUCCESS) {
        server->onClientReadError(getWeakClient(), code);
    } else {
        _reader.parse(packet.buffer, packet.size);
    }
}

void HttpServer::HttpNode::onContinue(void * arg)
{
    auto * server = getHttpServerPtr();
    assert(server != nullptr);
    server->onClientContinue(getWeakClient());
}

bool HttpServer::HttpNode::onSwitchingProtocol(HttpProperty const & property, void * arg)
{
    auto * server = getHttpServerPtr();
    assert(server != nullptr);
    return server->onClientSwitchingProtocol(getWeakClient(), property);
}

void HttpServer::HttpNode::onWsMessage(WsOpCode opcode, util::Buffer const & payload, void * arg)
{
    auto * server = getHttpServerPtr();
    assert(server != nullptr);
    return server->onClientWsMessage(getWeakClient(), opcode, payload);
}

void HttpServer::HttpNode::onRegularHttp(HttpProperty const & property, void * arg)
{
    auto * server = getHttpServerPtr();
    assert(server != nullptr);
    server->findAndExecute(getWeakClient(), property);
}

void HttpServer::HttpNode::onParseError(Err code, void * arg)
{
    auto * server = getHttpServerPtr();
    assert(server != nullptr);
    return server->onClientParseError(getWeakClient(), code);
}

// --------------------------
// HttpServer implementation.
// --------------------------

HttpServer::HttpServer(Loop & loop, StreamType type) : Parent(loop, type)
{
    // EMPTY.
}

HttpServer::~HttpServer()
{
    // EMPTY.
}

void HttpServer::setRequest(std::string const & method, std::string const & regex_path, OnRequest const & cb, int priority)
{
    setRequest(HttpFilter::SharedFilter(new (std::nothrow) HttpBaseFilter(method, regex_path)), cb);
}

void HttpServer::setRequest(std::regex const & regex_path, OnRequest const & cb, int priority)
{
    setRequest(HttpFilter::SharedFilter(new (std::nothrow) HttpBaseFilter(regex_path)), cb);
}

void HttpServer::setRequest(HttpFilter::SharedFilter const & filter, OnRequest const & cb, int priority)
{
    setRequest(HttpFilter(filter, cb), priority);
}

void HttpServer::setRequest(HttpFilter const & filter, int priority)
{
    WriteGuard const LOCK_GUARD(_filters_mutex);
    _filters.insert(HttpFilterMap::value_type(priority, filter));
}

Err HttpServer::writeResponse(WeakClient & node)
{
    if (auto shared = toSharedNode(node)) {
        return shared->writeResponse();
    }
    return Err::E_EXPIRED;
}

Err HttpServer::writeResponse(WeakClient & node, HttpResponse const & response)
{
    if (auto shared = toSharedNode(node)) {
        return shared->writeResponse(response);
    }
    return Err::E_EXPIRED;
}

Err HttpServer::writeWsResponse(WeakClient & node, HttpRequest const & request)
{
    if (auto shared = toSharedNode(node)) {
        return shared->writeWsResponse(request);
    }
    return Err::E_EXPIRED;
}

Err HttpServer::writeWsResponse(WeakClient & node, HttpRequest const & request, HttpResponse const & response)
{
    if (auto shared = toSharedNode(node)) {
        return shared->writeWsResponse(request, response);
    }
    return Err::E_EXPIRED;
}

Err HttpServer::writeWsFrame(WeakClient & node, WsFrame const & frame)
{
    if (auto shared = toSharedNode(node)) {
        return shared->writeWsFrame(frame);
    }
    return Err::E_EXPIRED;
}

Err HttpServer::writeText(WeakClient & node, char const * buffer, std::size_t size, bool finish)
{
    if (auto shared = toSharedNode(node)) {
        return shared->writeText(buffer, size, finish);
    }
    return Err::E_EXPIRED;
}

Err HttpServer::writeText(WeakClient & node, std::string const & text, bool finish)
{
    if (auto shared = toSharedNode(node)) {
        return shared->writeText(text, finish);
    }
    return Err::E_EXPIRED;
}

Err HttpServer::writeBinary(WeakClient & node, char const * buffer, std::size_t size, bool finish)
{
    if (auto shared = toSharedNode(node)) {
        return shared->writeBinary(buffer, size, finish);
    }
    return Err::E_EXPIRED;
}

Err HttpServer::writeBinary(WeakClient & node, util::Buffer const & buffer, bool finish)
{
    if (auto shared = toSharedNode(node)) {
        return shared->writeBinary(buffer, finish);
    }
    return Err::E_EXPIRED;
}

Err HttpServer::writeClose(WeakClient & node, WsStatus const & status)
{
    if (auto shared = toSharedNode(node)) {
        return shared->writeClose(status);
    }
    return Err::E_EXPIRED;
}

Err HttpServer::writeClose(WeakClient & node, WsStatusCode code)
{
    if (auto shared = toSharedNode(node)) {
        return shared->writeClose(code);
    }
    return Err::E_EXPIRED;
}

Err HttpServer::writeClose(WeakClient & node)
{
    if (auto shared = toSharedNode(node)) {
        return shared->writeClose();
    }
    return Err::E_EXPIRED;
}

HttpServer::HttpFilter HttpServer::findFilter(HttpRequest const & request)
{
    ReadGuard const LOCK_GUARD(_filters_mutex);
    for (auto & cursor : _filters) {
        auto & filter = cursor.second;
        if (static_cast<bool>(filter.filter) == false) {
            continue;
        }
        if (filter.filter->filter(request) == false) {
            continue;
        }
        return filter;
    }
    return HttpFilter();
}

void HttpServer::findAndExecute(WeakClient node, HttpRequest const & request)
{
    auto filter = findFilter(request);
    if (static_cast<bool>(filter.request_cb)) {
        filter.request_cb(node, request);
    } else {
        onClientRequest(node, request); // Default request callback.
    }
}

// ---------------
// Event handling.
// ---------------

void HttpServer::onClientRead(WeakClient node, Err code, ReadPacket const & packet)
{
    TBAG_INACCESSIBLE_BLOCK_ASSERT();
}

void HttpServer::onConnection(Err code)
{
    if (code != Err::E_SUCCESS) {
        tDLogE("HttpServer::onConnection() {} error.", getErrName(code));
        return;
    }

    auto node = accept();
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

    onClientOpen(node);
}

HttpServer::SharedStreamNode HttpServer::createClient(StreamType type, Loop & loop, SharedStream & server)
{
    assert(static_cast<bool>(server));
    tDLogSD("HttpServer::createClient() Create HttpNode");
    return SharedStreamNode(new (std::nothrow) HttpNode(loop, type, this));
}

void HttpServer::onClientEof(WeakClient node)
{
    if (auto shared = toSharedNode(node)) {
        shared->close();
    }
}

void HttpServer::onClientReadError(WeakClient node, Err code)
{
    if (auto shared = toSharedNode(node)) {
        shared->close();
    }
}

void HttpServer::onClientParseError(WeakClient node, Err code)
{
    if (auto shared = toSharedNode(node)) {
        shared->close();
    }
}

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

