/**
 * @file   HttpServer.cpp
 * @brief  HttpServer class implementation.
 * @author zer0
 * @date   2019-01-11
 */

#include <libtbag/http/HttpServer.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/net/socket/NetStreamServer.hpp>
#include <libtbag/http/HttpReader.hpp>
#include <libtbag/net/Ip.hpp>
#include <libtbag/Type.hpp>

#include <cassert>
#include <unordered_map>
#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace http {

/**
 * HttpServer::Impl class implementation.
 *
 * @author zer0
 * @date   2019-01-11
 */
struct HttpServer::Impl : public libtbag::net::socket::NetStreamServer
{
public:
    using NetStreamServer     = libtbag::net::socket::NetStreamServer;
    using HttpReaderInterface = libtbag::http::HttpReaderInterface;

public:
    STATIC_ASSERT_CHECK_IS_SAME(Buffer, HttpReaderInterface::Buffer);

public:
    /**
     * Client node properties.
     *
     * @author zer0
     * @date   2019-01-11
     */
    struct Node : public HttpReaderInterface
    {
    public:
        using Reader = libtbag::http::HttpReaderForCallback<Node>;

    public:
        Impl * const IMPL;
        std::intptr_t const ID;

    private:
        Reader _reader;

    public:
        Node(Impl * impl, std::intptr_t id, std::string const & key, bool use_websocket)
                : IMPL(impl), ID(id), _reader(this, key, use_websocket)
        { assert(IMPL != nullptr); }

        ~Node()
        { /* EMPTY. */ }

    public:
        Err parse(char const * buffer, std::size_t size)
        { return _reader.parse(buffer, size); }

    public:
        virtual void onContinue(void * arg) override
        { return IMPL->onContinue(this); }

        virtual bool onSwitchingProtocol(HttpProperty const & property, void * arg) override
        { return IMPL->onSwitchingProtocol(this, property); }

        virtual void onWsMessage(WsOpCode opcode, Buffer const & payload, void * arg) override
        { return IMPL->onWsMessage(this, opcode, payload); }

        virtual void onRegularHttp(HttpProperty const & property, void * arg) override
        { return IMPL->onRegularHttp(this, property); }

        virtual void onParseError(Err code, void * arg) override
        { return IMPL->onParseError(this, code); }
    };

public:
    using SharedNode = std::shared_ptr<Node>;
    using NodeMap    = std::unordered_map<std::intptr_t, SharedNode>;

public:
    HttpServer * const PARENT;
    std::string  const KEY;
    bool         const ENABLE_WEBSOCKET;

private:
    NodeMap _nodes;

public:
    Impl(HttpServer * parent, MqParams const & params, std::string const & key, bool use_websocket)
            : NetStreamServer(params), PARENT(parent), KEY(key), ENABLE_WEBSOCKET(use_websocket)
    {
        assert(PARENT != nullptr);
    }

    ~Impl()
    {
        // EMPTY.
    }

private:
    virtual void onBegin() override
    {
        PARENT->onBegin();
    }

    virtual void onEnd() override
    {
        PARENT->onEnd();
    }

    virtual bool onAccept(std::intptr_t id, std::string const & ip) override
    {
        if (!PARENT->onAccept(id, ip)) {
            return false;
        }

        SharedNode node;
        if (ENABLE_WEBSOCKET) {
            if (KEY.empty()) {
                node = std::make_shared<Node>(this, id, libtbag::http::generateRandomWebSocketKey(), true);
            } else {
                node = std::make_shared<Node>(this, id, KEY, true);
            }
        } else {
            node = std::make_shared<Node>(this, id, std::string(), false);
        }
        assert(static_cast<bool>(node));

        bool const INSERT_RESULT = _nodes.insert(std::make_pair(id, node)).second;
        assert(INSERT_RESULT);

        return true;
    }

    virtual void onRecv(std::intptr_t id, char const * buffer, std::size_t size) override
    {
        auto itr = _nodes.find(id);
        assert(itr != _nodes.end());

        auto & node = itr->second;
        assert(static_cast<bool>(node));

        node->parse(buffer, size);
    }

    virtual void onClose(std::intptr_t id) override
    {
        auto const ERASE_RESULT = _nodes.erase(id);
        assert(ERASE_RESULT == 1);
    }

private:
    void onContinue(Node * node)
    {
        assert(node != nullptr);
    }

    bool onSwitchingProtocol(Node * node, HttpProperty const & property)
    {
        assert(node != nullptr);
        return PARENT->onSwitchingProtocol(node->ID, static_cast<HttpRequest>(property));
    }

    void onWsMessage(Node * node, WsOpCode opcode, Buffer const & payload)
    {
        assert(node != nullptr);
        PARENT->onWsMessage(node->ID, opcode, payload);
    }

    void onRegularHttp(Node * node, HttpProperty const & property)
    {
        assert(node != nullptr);

        auto response = PARENT->onRegularHttp(node->ID, property);
        libtbag::http::updateDefaultResponse(response);

        auto const CODE = send(libtbag::http::toResponseString(response), node->ID);
        if (isFailure(CODE)) {
            onParseError(node, CODE);
        }
    }

    void onParseError(Node * node, Err code)
    {
        assert(node != nullptr);
        PARENT->onError(node->ID, code);
    }
};

// --------------------------
// HttpServer implementation.
// --------------------------

HttpServer::HttpServer(MqParams const & params, bool use_websocket)
        : HttpServer(params, std::string(), use_websocket)
{
    assert(static_cast<bool>(_impl));
}

HttpServer::HttpServer(MqParams const & params, std::string const & key, bool use_websocket)
        : _impl(std::make_unique<Impl>(this, params, key, use_websocket))
{
    assert(static_cast<bool>(_impl));
}

HttpServer::~HttpServer()
{
    // EMPTY.
}

HttpServer::Loop & HttpServer::loop()
{
    assert(static_cast<bool>(_impl));
    return _impl->loop();
}

HttpServer::Loop const & HttpServer::loop() const
{
    assert(static_cast<bool>(_impl));
    return _impl->loop();
}

void HttpServer::onBegin()
{
    // EMPTY.
}

void HttpServer::onEnd()
{
    // EMPTY.
}

bool HttpServer::onAccept(std::intptr_t id, std::string const & ip)
{
    return true;
}

void HttpServer::onClose(std::intptr_t id)
{
    // EMPTY.
}

HttpServer::HttpResponse HttpServer::onRegularHttp(std::intptr_t id, HttpRequest const & request)
{
    return HttpResponse{};
}

bool HttpServer::onSwitchingProtocol(std::intptr_t id, HttpRequest const & request)
{
    return true;
}

void HttpServer::onWsMessage(std::intptr_t id, WsOpCode opcode, Buffer const & payload)
{
    // EMPTY.
}

void HttpServer::onError(std::intptr_t id, Err code)
{
    assert(static_cast<bool>(_impl));
    auto const SEND_CODE = _impl->sendClose(id);
    tDLogE("HttpServer::onError({}) Close node({}) request: {}", code, id, SEND_CODE);
}

void HttpServer::join()
{
    assert(static_cast<bool>(_impl));
    _impl->join();
}

Err HttpServer::exit()
{
    assert(static_cast<bool>(_impl));
    return _impl->exit();
}

Err HttpServer::close(std::intptr_t id)
{
    assert(static_cast<bool>(_impl));
    return _impl->sendClose(id);
}

Err HttpServer::writeWsFrame(std::intptr_t id, WsFrame const & frame)
{
    assert(static_cast<bool>(_impl));
    if (!_impl->ENABLE_WEBSOCKET) {
        return Err::E_ILLSTATE;
    }

    Buffer buffer;
    auto const SIZE = frame.copyTo(buffer);

    if (SIZE == 0) {
        tDLogE("HttpServer::writeWsFrame() WsFrame -> Buffer copy error");
        return Err::E_ECOPY;
    }
    return _impl->send(buffer.data(), buffer.size(), id);
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

HttpServer::MqParams HttpServer::getDefaultParams(std::string const & host, int port)
{
    MqParams params;
    params.type = libtbag::mq::details::MqType::MT_TCP;
    params.address = host;
    params.port = port;
    params.send_queue_size = 1024;
    params.recv_queue_size = 1024;
    params.wait_closing_millisec = 1000;
    params.continuous_read_error_count = 4;
    params.connect_timeout_millisec = 4 * 1000;
    params.reconnect_count = 1;
    params.reconnect_delay_millisec = 0;
    params.wait_on_activation_timeout_millisec = libtbag::mq::details::WAIT_ON_ACTIVATION_INFINITY;
    params.wait_next_opcode_nanosec = 1000;
    params.verbose = false;
    params.user = nullptr;
    params.on_create_loop = nullptr;
    return params;
}

} // namespace http

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

