/**
 * @file   HttpServer.cpp
 * @brief  HttpServer class implementation.
 * @author zer0
 * @date   2019-01-11
 */

#include <libtbag/http/HttpServer.hpp>
#include <libtbag/log/Log.hpp>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace http {

HttpServer::HttpServer(std::string const & uri, Callbacks const & callbacks)
        : HttpServer(Params(uri, callbacks))
{
    assert(static_cast<bool>(_server));
}

HttpServer::HttpServer(std::string const & address, int port, Callbacks const & callbacks)
        : HttpServer(Params(address, port, callbacks))
{
    assert(static_cast<bool>(_server));
}

HttpServer::HttpServer(Params const & params)
        : PARAMS(params)
{
    NetStreamServer::Callbacks server_cbs;
    server_cbs.begin_cb = [&](){
        onBegin();
    };
    server_cbs.end_cb = [&](){
        onEnd();
    };
    server_cbs.accept_cb = [&](std::intptr_t id, std::string const & ip) -> bool {
        return onAccept(id, ip);
    };
    server_cbs.recv_cb = [&](std::intptr_t id, char const * b, std::size_t l){
        onRecv(id, b, l);
    };
    server_cbs.close_cb = [&](std::intptr_t id){
        onClose(id);
    };
    _server = std::make_unique<NetStreamServer>(params, server_cbs);
    assert(static_cast<bool>(_server));
}

HttpServer::~HttpServer()
{
    assert(static_cast<bool>(_server));
}

HttpServer::Loop & HttpServer::loop()
{
    assert(static_cast<bool>(_server));
    return _server->loop();
}

HttpServer::Loop const & HttpServer::loop() const
{
    assert(static_cast<bool>(_server));
    return _server->loop();
}

void HttpServer::onBegin()
{
    if (PARAMS.begin_cb) {
        PARAMS.begin_cb();
    }
}

void HttpServer::onEnd()
{
    if (PARAMS.end_cb) {
        PARAMS.end_cb();
    }
}

bool HttpServer::onAccept(std::intptr_t id, std::string const & ip)
{
    if (PARAMS.accept_cb) {
        auto const IS_ACCEPT = PARAMS.accept_cb(id, ip);
        if (!IS_ACCEPT) {
            return false;
        }
    }

    SharedNode node;
    if (PARAMS.enable_websocket) {
        node = std::make_shared<Node>(this, id, PARAMS.getWebSocketKey(), true);
    } else {
        node = std::make_shared<Node>(this, id, std::string(), false);
    }
    assert(static_cast<bool>(node));

    bool const INSERT_RESULT = _nodes.insert(std::make_pair(id, node)).second;
    assert(INSERT_RESULT);

    return true;
}

void HttpServer::onRecv(std::intptr_t id, char const * buffer, std::size_t size)
{
    auto itr = _nodes.find(id);
    assert(itr != _nodes.end());

    auto & node = itr->second;
    assert(static_cast<bool>(node));

    node->parse(buffer, size);
}

void HttpServer::onClose(std::intptr_t id)
{
    if (PARAMS.close_cb) {
        PARAMS.close_cb(id);
    }

    auto const ERASE_RESULT = _nodes.erase(id);
    assert(ERASE_RESULT == 1);
}

void HttpServer::onContinue(Node * node)
{
    assert(node != nullptr);
    if (PARAMS.continue_cb) {
        PARAMS.continue_cb(node->ID);
    }
}

bool HttpServer::onSwitchingProtocol(Node * node, HttpProperty const & property)
{
    assert(node != nullptr);
    if (PARAMS.switch_cb) {
        return PARAMS.switch_cb(node->ID, static_cast<HttpRequest>(property));
    } else {
        return PARAMS.enable_websocket;
    }
}

void HttpServer::onWsMessage(Node * node, WsOpCode opcode, Buffer const & payload)
{
    assert(node != nullptr);
    if (PARAMS.message_cb) {
        PARAMS.message_cb(node->ID, opcode, payload);
    }
}

void HttpServer::onRegularHttp(Node * node, HttpProperty const & property)
{
    assert(node != nullptr);

    HttpResponse response;
    if (PARAMS.http_cb) {
        response = PARAMS.http_cb(node->ID, static_cast<HttpRequest>(property));
    }
    libtbag::http::updateDefaultResponse(response);

    auto const CODE = _server->send(libtbag::http::toResponseString(response), node->ID);
    if (isFailure(CODE)) {
        onParseError(node, CODE);
    }
}

void HttpServer::onParseError(Node * node, Err code)
{
    assert(node != nullptr);
    if (PARAMS.error_cb) {
        PARAMS.error_cb(node->ID, code);
    } else {
        auto const CLOSE_CODE = close(node->ID);
        tDLogE("HttpServer::onError({}) Close node({}) request: {}", code, node->ID, CLOSE_CODE);
    }
}

void HttpServer::join()
{
    assert(static_cast<bool>(_server));
    _server->join();
}

Err HttpServer::exit()
{
    assert(static_cast<bool>(_server));
    return _server->exit();
}

Err HttpServer::close(std::intptr_t id)
{
    assert(static_cast<bool>(_server));
    return _server->sendClose(id);
}

Err HttpServer::writeWsResponse(std::intptr_t id, HttpRequest const & request)
{
    HttpResponse response;
    libtbag::http::updateDefaultWsResponse(response, request);
    auto const RESPONSE_STRING = libtbag::http::toResponseString(response);
    return _server->send(RESPONSE_STRING.data(), RESPONSE_STRING.size(), id);
}

Err HttpServer::writeWsFrame(std::intptr_t id, WsFrame const & frame)
{
    if (!PARAMS.enable_websocket) {
        return E_ILLSTATE;
    }

    Buffer buffer;
    auto const SIZE = frame.copyTo(buffer);

    if (SIZE == 0) {
        tDLogE("HttpServer::writeWsFrame() WsFrame -> Buffer copy error");
        return E_ECOPY;
    }
    return _server->send(buffer.data(), buffer.size(), id);
}

Err HttpServer::writeText(std::intptr_t id, char const * buffer, std::size_t size, bool finish)
{
    WsFrame frame;
    frame.text(buffer, size, finish);
    return writeWsFrame(id, frame);
}

Err HttpServer::writeText(std::intptr_t id, std::string const & text, bool finish)
{
    WsFrame frame;
    frame.text(text, finish);
    return writeWsFrame(id, frame);
}

Err HttpServer::writeBinary(std::intptr_t id, char const * buffer, std::size_t size, bool finish)
{
    WsFrame frame;
    frame.binary(buffer, size, finish);
    return writeWsFrame(id, frame);
}

Err HttpServer::writeBinary(std::intptr_t id, Buffer const & buffer, bool finish)
{
    WsFrame frame;
    frame.binary(buffer, finish);
    return writeWsFrame(id, frame);
}

Err HttpServer::writeClose(std::intptr_t id, WsStatus const & status)
{
    WsFrame frame;
    frame.close(status);
    return writeWsFrame(id, frame);
}

Err HttpServer::writeClose(std::intptr_t id, WsStatusCode code)
{
    WsFrame frame;
    frame.close(code);
    return writeWsFrame(id, frame);
}

Err HttpServer::writeClose(std::intptr_t id)
{
    return writeClose(id, WsStatusCode::WSSC_NORMAL_CLOSURE);
}

} // namespace http

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

