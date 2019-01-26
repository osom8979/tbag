/**
 * @file   HttpServer.hpp
 * @brief  HttpServer class prototype.
 * @author zer0
 * @date   2019-01-11
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_HTTP_HTTPSERVER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_HTTP_HTTPSERVER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <libtbag/http/HttpCommon.hpp>
#include <libtbag/http/HttpReader.hpp>
#include <libtbag/http/WsFrame.hpp>

#include <libtbag/mq/details/MqCommon.hpp>
#include <libtbag/net/socket/NetStreamServer.hpp>
#include <libtbag/net/Ip.hpp>
#include <libtbag/uvpp/Loop.hpp>

#include <libtbag/crypto/Tls.hpp>
#include <libtbag/random/MaskingDevice.hpp>
#include <libtbag/Type.hpp>

#include <string>
#include <memory>
#include <unordered_map>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace http {

/**
 * HttpServer class prototype.
 *
 * @author zer0
 * @date   2019-01-11
 * @date   2019-01-15 (Merge with HttpClient::Impl class)
 */
class TBAG_API HttpServer TBAG_FINAL : private Noncopyable
{
public:
    using Loop         = libtbag::uvpp::Loop;
    using MqParams     = libtbag::mq::details::MqParams;
    using HttpRequest  = libtbag::http::HttpRequest;
    using HttpResponse = libtbag::http::HttpResponse;
    using Callbacks    = libtbag::http::HttpServerCallbacks;
    using Params       = libtbag::http::HttpServerParams;
    using Buffer       = libtbag::util::Buffer;
    using WsFrame      = libtbag::http::WsFrame;

    using HttpReaderInterface = libtbag::http::HttpReaderInterface;
    using NetStreamServer     = libtbag::net::socket::NetStreamServer;
    using UniqueServer        = std::unique_ptr<NetStreamServer>;

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
        HttpServer *  const PARENT;
        std::intptr_t const ID;

    private:
        Reader _reader;

    public:
        Node(HttpServer * parent, std::intptr_t id, std::string const & key, bool use_websocket)
                : PARENT(parent), ID(id), _reader(this, key, use_websocket)
        { assert(PARENT != nullptr); }

        virtual ~Node()
        { /* EMPTY. */ }

    public:
        Err parse(char const * buffer, std::size_t size)
        { return _reader.parse(buffer, size); }

    public:
        virtual void onContinue(void * arg) override
        { PARENT->onContinue(this); }

        virtual bool onSwitchingProtocol(HttpProperty const & property, void * arg) override
        { return PARENT->onSwitchingProtocol(this, property); }

        virtual void onWsMessage(WsOpCode opcode, Buffer const & payload, void * arg) override
        { PARENT->onWsMessage(this, opcode, payload); }

        virtual void onRegularHttp(HttpProperty const & property, void * arg) override
        { PARENT->onRegularHttp(this, property); }

        virtual void onParseError(Err code, void * arg) override
        { PARENT->onParseError(this, code); }
    };

public:
    using SharedNode = std::shared_ptr<Node>;
    using NodeMap    = std::unordered_map<std::intptr_t, SharedNode>;

public:
    HttpServerParams const PARAMS;

private:
    UniqueServer _server;
    NodeMap      _nodes;

public:
    HttpServer(std::string const & address, int port, Callbacks const & callbacks);
    HttpServer(Params const & params);
    ~HttpServer();

public:
    // @formatter:off
    Loop       & loop();
    Loop const & loop() const;
    // @formatter:on

private:
    void onBegin();
    void onEnd();

    bool onAccept(std::intptr_t id, std::string const & ip);
    void onRecv(std::intptr_t id, char const * buffer, std::size_t size);
    void onClose(std::intptr_t id);

private:
    void onContinue(Node * node);
    bool onSwitchingProtocol(Node * node, HttpProperty const & property);
    void onWsMessage(Node * node, WsOpCode opcode, Buffer const & payload);
    void onRegularHttp(Node * node, HttpProperty const & property);
    void onParseError(Node * node, Err code);

public:
    void join();

public:
    Err exit();
    Err close(std::intptr_t id);

public:
    Err writeWsFrame(std::intptr_t id, WsFrame const & frame);
    Err writeText   (std::intptr_t id, char const * buffer, std::size_t size, bool finish = true);
    Err writeText   (std::intptr_t id, std::string const & text, bool finish = true);
    Err writeBinary (std::intptr_t id, char const * buffer, std::size_t size, bool finish = true);
    Err writeBinary (std::intptr_t id, Buffer const & buffer, bool finish = true);
    Err writeClose  (std::intptr_t id, WsStatus const & status);
    Err writeClose  (std::intptr_t id, WsStatusCode code);
    Err writeClose  (std::intptr_t id);
};

} // namespace http

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_HTTP_HTTPSERVER_HPP__

