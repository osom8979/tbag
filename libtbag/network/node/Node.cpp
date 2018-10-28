/**
 * @file   Node.cpp
 * @brief  Node class implementation.
 * @author zer0
 * @date   2018-10-26
 */

#include <libtbag/network/node/Node.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/thread/ThreadPool.hpp>
#include <libtbag/signal/SignalHandler.hpp>

#include <libtbag/network/http/HttpServer.hpp>
#include <libtbag/network/http/HttpClient.hpp>

#include <libtbag/uvpp/ex/SafetyAsync.hpp>
#include <libtbag/uvpp/Idle.hpp>
#include <libtbag/uvpp/Signal.hpp>
#include <libtbag/uvpp/Loop.hpp>

#include <libtbag/string/StringUtils.hpp>
#include <libtbag/util/BufferInfo.hpp>
#include <libtbag/util/Version.hpp>

#include <cassert>
#include <atomic>
#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace node    {

/** @warning Don't change this variable. */
TBAG_CONSTEXPR static unsigned int const _POOL_SIZE = 1U;

/**
 * Node::Impl class implementation.
 *
 * @author zer0
 * @date   2018-10-26
 */
struct Node::Impl : private Noncopyable
{
public:
    using ThreadPool = libtbag::thread::ThreadPool;

public:
    using HttpServer   = libtbag::network::http::HttpServer;
    using HttpClient   = libtbag::network::http::HttpClient;
    using StreamType   = libtbag::network::details::StreamType;
    using HttpRequest  = libtbag::network::http::HttpRequest;
    using HttpResponse = libtbag::network::http::HttpResponse;
    using HttpProperty = libtbag::network::http::HttpProperty;
    using WsOpCode     = libtbag::network::http::ws::WsOpCode;
    using EventType    = libtbag::network::http::EventType;
    using Loop         = libtbag::uvpp::Loop;
    using SafetyAsync  = libtbag::uvpp::ex::SafetyAsync;

public:
    struct Server;
    struct Client;

    friend struct Server;
    friend struct Client;

public:
    /**
     * Node::Impl::Server class implementation.
     *
     * @author zer0
     * @date   2018-10-26
     */
    struct Server : public HttpServer
    {
    public:
        using Base = HttpServer;

    private:
        Impl * _impl = nullptr;

    public:
        Server(Impl * i, Loop & loop, StreamType type = StreamType::TCP) : Base(loop, type), _impl(i)
        {
            assert(_impl != nullptr);
        }

        virtual ~Server()
        {
            // EMPTY.
        }

        // -----------------------
        // ServerInterface events.
        // -----------------------

        virtual void onConnection(Err code) override
        {
            Base::onConnection(code);
        }

        virtual void onClientShutdown(WeakClient node, Err code) override
        {
            // EMPTY.
        }

        virtual void onClientWrite(WeakClient node, Err code) override
        {
            // EMPTY.
        }

        // Don't implement this method.
        // virtual void onClientRead(WeakClient node, Err code, ReadPacket const & packet) override
        // { /* EMPTY. */ }

        virtual void onClientClose(WeakClient node) override
        {
            // EMPTY.
        }

        virtual void onClientTimer(WeakClient node) override
        {
            // EMPTY.
        }

        virtual void onServerClose() override
        {
            // EMPTY.
        }

        // ------------------
        // HttpServer events.
        // ------------------

        SharedStreamNode createClient(StreamType type, Loop & loop, SharedStream & server) override
        {
            assert(static_cast<bool>(server));
            return SharedStreamNode(new (std::nothrow) HttpNode(loop, type, this));
        }

        virtual void onClientEof(WeakClient node) override
        {
            Base::onClientEof(node);
        }

        virtual void onClientReadError(WeakClient node, Err code) override
        {
            Base::onClientReadError(node, code);
        }

        virtual void onClientParseError(WeakClient node, Err code) override
        {
            Base::onClientParseError(node, code);
        }

        virtual bool onClientSwitchingProtocol(WeakClient node, HttpRequest const & request) override
        {
            return Base::onClientSwitchingProtocol(node, request);
        }

        // ----------------------------
        // HttpServer extension events.
        // ----------------------------

        virtual void onClientOpen(WeakClient node) override
        {
            // EMPTY.
        }

        virtual void onClientContinue(WeakClient node) override
        {
            // EMPTY.
        }

        virtual void onClientWsMessage(WeakClient node, WsOpCode opcode, Buffer const & payload) override
        {
            // EMPTY.
        }

        virtual void onClientRequest(WeakClient node, HttpRequest const & request) override
        {
            // EMPTY.
        }
    };

    /**
     * Node::Impl::Client class implementation.
     *
     * @author zer0
     * @date   2018-10-26
     */
    struct Client : public HttpClient
    {
    public:
        using Base = HttpClient;

    private:
        Impl * _impl = nullptr;

    public:
        Client(Impl * i, Loop & loop, StreamType type = StreamType::TCP) : Base(loop, type), _impl(i)
        {
            assert(_impl != nullptr);
        }

        virtual ~Client()
        {
            // EMPTY.
        }

        // -----------------------
        // ClientInterface events.
        // -----------------------

        virtual void onConnect(Err code) override
        {
            Base::onConnect(code);
        }

        virtual void onShutdown(Err code) override
        {
            // EMPTY.
        }

        virtual void onWrite(Err code) override
        {
            // EMPTY.
        }

        virtual void onRead(Err code, ReadPacket const & packet) override
        {
            Base::onRead(code, packet);
        }

        virtual void onClose() override
        {
            // EMPTY.
        }

        virtual void onTimer() override
        {
            // EMPTY.
        }

        // ----------------------------
        // HttpClient extension events.
        // ----------------------------

        virtual void onOpen() override
        {
            // EMPTY.
        }

        virtual void onEof() override
        {
            Base::onEof();
        }

        virtual void onError(EventType from, Err code) override
        {
            Base::onError(from, code);
        }

        // ---------------------------
        // HttpReaderInterface events.
        // ---------------------------

        virtual void onContinue(void * arg) override
        {
            // EMPTY.
        }

        virtual bool onSwitchingProtocol(HttpProperty const & property, void * arg) override
        {
            return Base::onSwitchingProtocol(property, arg);
        }

        virtual void onWsMessage(WsOpCode opcode, util::Buffer const & payload, void * arg) override
        {
            // EMPTY.
        }

        virtual void onRegularHttp(HttpProperty const & property, void * arg) override
        {
            // EMPTY.
        }

        virtual void onParseError(Err code, void * arg) override
        {
            Base::onParseError(code, arg);
        }
    };

    /**
     * Node::Impl::Idle class implementation.
     *
     * @author zer0
     * @date   2018-10-27
     */
    struct Idle : public libtbag::uvpp::Idle
    {
    public:
        using Base = libtbag::uvpp::Idle;

    private:
        Impl * _impl = nullptr;

    public:
        Idle(Impl * i, Loop & loop) : Base(loop), _impl(i)
        {
            assert(_impl != nullptr);
        }

        virtual ~Idle()
        {
            // EMPTY.
        }

        virtual void onIdle() override
        {
            // EMPTY.
        }
    };

    using SharedServer = std::shared_ptr<Server>;
    using SharedClient = std::shared_ptr<Client>;
    using SharedIdle   = std::shared_ptr<Idle>;
    using SharedAsync  = std::shared_ptr<SafetyAsync>;

    using ClientMap = std::unordered_map<std::string, SharedClient>;

private:
    Node * _parent;
    Param  _param;

private:
    ThreadPool _pool;
    Loop       _loop;

private:
    SharedIdle   _idle;
    SharedAsync  _async;

private:
    SharedServer _server;
    ClientMap    _clients;

public:
    std::atomic<NodeState> _state;
    std::atomic_int _c2s_counter;
    std::atomic_int _s2c_counter;

public:
    Impl(Node * parent) : _parent(parent), _pool(_POOL_SIZE),
                          _state(NodeState::NS_CLOSED), _c2s_counter(0), _s2c_counter(0)
    {
        assert(_parent != nullptr);
        assert(_pool.sizeOfThreads() == _POOL_SIZE);

        _async = _loop.newHandle<SafetyAsync>(_loop);
        _idle = _loop.newHandle<Idle>(this, _loop);
        assert(static_cast<bool>(_async));
        assert(static_cast<bool>(_idle));

        if (!_pool.push([this](){ runMain(); })) {
            throw ErrException(Err::E_EPUSH);
        }
    }

    ~Impl()
    {
        auto const CLOSE_CODE = close();
        if (isFailure(CLOSE_CODE)) {
            tDLogE("Node::Impl::~Impl() close error: {}", CLOSE_CODE);
        }
        auto const JOB = _async->newSendFunc([this](){
            _async->close();
        });
        if (!JOB) {
            tDLogE("Node::Impl::~Impl() async send error.");
        }
        if (!_pool.waitPush([this](){ _pool.exit(); })) {
            tDLogE("Node::Impl::~Impl() pool push error.");
        }
    }

private:
    void runMain()
    {
        Err const CODE = _loop.run();
        if (isFailure(CODE)) {
            tDLogE("Node::Impl::runMain() loop run error: {}", CODE);
        }
    }

public:
    void loggingSelfInformation() const
    {
        using namespace libtbag::util;
        tDLogI("===== NodE INFORMATION =====");
        tDLogI("* Tbag version: {}", getTbagVersion().toString());
        tDLogI("* Packet version: {}", getTbagPacketVersion().toString());
        tDLogI("* Release version: {}", getTbagReleaseVersion().toString());
        if (_server) {
            tDLogI("* Server: {}:{})", _server->dest(), _server->port());
        } else {
        }
    }

public:
    TBAG_CONSTEXPR static char const * const PIPE_SCHEMA = "pipe";

public:
    Err open(std::string const & uri)
    {
        switch (_state) {
        case NodeState::NS_CLOSING: return Err::E_EBUSY;
        case NodeState::NS_CLOSED:  break; // OK!
        case NodeState::NS_OPENING: return Err::E_EBUSY;
        case NodeState::NS_OPENED:  return Err::E_EALREADY;
        case NodeState::NS_WORKING: return Err::E_EBUSY;
        default:                    return Err::E_UNKNOWN;
        }

        auto const JOB = _async->newSendFunc([this, uri](){
            if (_state == NodeState::NS_OPENING) {
                auto const CODE = openMain(uri);
                if (isSuccess(CODE)) {
                    _state = NodeState::NS_OPENED;
                } else {
                    tDLogE("Node::Impl::open()@async() Open error: {}", CODE);
                    _state = NodeState::NS_CLOSED;
                }
            } else {
                tDLogE("Node::Impl::open()@async() Illegal state: {}", getNodeStateName(_state));
                // Do not change state.
            }
        });

        if (JOB) {
            NodeState exchange_state = NodeState::NS_OPENING;
            if (_state.compare_exchange_weak(exchange_state, NodeState::NS_CLOSED)) {
                return Err::E_SUCCESS;
            }
            tDLogE("Node::Impl::open() Exchange state error: {}", getNodeStateName(_state));
            return Err::E_ILLSTATE;
        }
        return Err::E_EPUSH;
    }

    Err close()
    {
        switch (_state) {
        case NodeState::NS_CLOSING: return Err::E_EBUSY;
        case NodeState::NS_CLOSED:  return Err::E_EALREADY;
        case NodeState::NS_OPENING: return Err::E_EBUSY;
        case NodeState::NS_OPENED:  break; // OK!
        case NodeState::NS_WORKING: return Err::E_EBUSY;
        default:                    return Err::E_UNKNOWN;
        }

        auto const JOB = _async->newSendFunc([this](){
            if (_state == NodeState::NS_CLOSING) {
                while (_c2s_counter == 0 && _s2c_counter == 0) {
                    // Wait c2s & s2c operations ...
                }
                auto const CODE = closeMain();
                if (isFailure(CODE)) {
                    tDLogE("Node::Impl::close()@async() Close error: {}", CODE);
                }
                _state = NodeState::NS_CLOSED;
            } else {
                tDLogE("Node::Impl::close()@async() Illegal state: {}", getNodeStateName(_state));
                // Do not change state.
            }
        });

        if (JOB) {
            NodeState exchange_state = NodeState::NS_CLOSING;
            if (_state.compare_exchange_weak(exchange_state, NodeState::NS_OPENED)) {
                return Err::E_SUCCESS;
            }
            tDLogE("Node::Impl::close() Exchange state error: {}", getNodeStateName(_state));
            return Err::E_ILLSTATE;
        }
        return Err::E_EPUSH;
    }

    Err connect(std::string const & client_name, std::string const & server_uri)
    {
        switch (_state) {
        case NodeState::NS_CLOSING: return Err::E_EBUSY;
        case NodeState::NS_CLOSED:  return Err::E_NREADY;
        case NodeState::NS_OPENING: return Err::E_EBUSY;
        case NodeState::NS_OPENED:  break; // OK!
        case NodeState::NS_WORKING: return Err::E_EBUSY;
        default:                    return Err::E_UNKNOWN;
        }

        auto const JOB = _async->newSendFunc([this, client_name, server_uri](){
            if (_state == NodeState::NS_WORKING) {
                auto const CODE = connectMain(client_name, server_uri);
                if (isFailure(CODE)) {
                    tDLogE("Node::Impl::connect()@async() Open error: {}", CODE);
                }
                _state = NodeState::NS_OPENED;
            } else {
                tDLogE("Node::Impl::connect()@async() Illegal state: {}", getNodeStateName(_state));
                // Do not change state.
            }
        });

        if (JOB) {
            NodeState exchange_state = NodeState::NS_WORKING;
            if (_state.compare_exchange_weak(exchange_state, NodeState::NS_OPENED)) {
                return Err::E_SUCCESS;
            }
            tDLogE("Node::Impl::connect() Exchange state error: {}", getNodeStateName(_state));
            return Err::E_ILLSTATE;
        }
        return Err::E_EPUSH;
    }

    Err disconnect(std::string const & client_name)
    {
        switch (_state) {
        case NodeState::NS_CLOSING: return Err::E_EBUSY;
        case NodeState::NS_CLOSED:  return Err::E_NREADY;
        case NodeState::NS_OPENING: return Err::E_EBUSY;
        case NodeState::NS_OPENED:  break; // OK!
        case NodeState::NS_WORKING: return Err::E_EBUSY;
        default:                    return Err::E_UNKNOWN;
        }

        auto const JOB = _async->newSendFunc([this, client_name](){
            if (_state == NodeState::NS_WORKING) {
                auto const CODE = disconnectMain(client_name);
                if (isFailure(CODE)) {
                    tDLogE("Node::Impl::disconnect()@async() Open error: {}", CODE);
                }
                _state = NodeState::NS_OPENED;
            } else {
                tDLogE("Node::Impl::disconnect()@async() Illegal state: {}", getNodeStateName(_state));
                // Do not change state.
            }
        });

        if (JOB) {
            NodeState exchange_state = NodeState::NS_WORKING;
            if (_state.compare_exchange_weak(exchange_state, NodeState::NS_OPENED)) {
                return Err::E_SUCCESS;
            }
            tDLogE("Node::Impl::disconnect() Exchange state error: {}", getNodeStateName(_state));
            return Err::E_ILLSTATE;
        }
        return Err::E_EPUSH;
    }

    Err c2s(std::string const & server_name, char const * buffer, std::size_t size)
    {
        switch (_state) {
        case NodeState::NS_CLOSING: return Err::E_EBUSY;
        case NodeState::NS_CLOSED:  return Err::E_NREADY;
        case NodeState::NS_OPENING: return Err::E_EBUSY;
        case NodeState::NS_OPENED:  break; // OK!
        case NodeState::NS_WORKING: break; // OK!
        default:                    return Err::E_UNKNOWN;
        }

        Err code;
        ++_c2s_counter;
        switch (_state) {
        case NodeState::NS_OPENED:
        case NodeState::NS_WORKING:
            code = c2sMain(server_name, buffer, size);
            break;
        default:
            code = Err::E_ILLSTATE;
            break;
        }
        --_c2s_counter;
        return code;
    }

    Err s2c(std::string const & client_name, char const * buffer, std::size_t size)
    {
        switch (_state) {
        case NodeState::NS_CLOSING: return Err::E_EBUSY;
        case NodeState::NS_CLOSED:  return Err::E_NREADY;
        case NodeState::NS_OPENING: return Err::E_EBUSY;
        case NodeState::NS_OPENED:  break; // OK!
        case NodeState::NS_WORKING: break; // OK!
        default:                    return Err::E_UNKNOWN;
        }

        Err code;
        ++_s2c_counter;
        switch (_state) {
        case NodeState::NS_OPENED:
        case NodeState::NS_WORKING:
            code = s2cMain(client_name, buffer, size);
            break;
        default:
            code = Err::E_ILLSTATE;
            break;
        }
        --_s2c_counter;
        return code;
    }

public:
    Err openMain(std::string const & uri)
    {
        StreamType  type;
        std::string host;
        int         port;

        libtbag::network::Uri uri_parser;
        if (!uri_parser.parse(uri)) {
            return Err::E_PARSING;
        }

        auto const SCHEMA = libtbag::string::lower(uri_parser.getSchema());
        if (SCHEMA == PIPE_SCHEMA) {
            type = StreamType::PIPE;
            host = uri_parser.getHost();
            port = 0;
        } else {
            type = StreamType::TCP;
            Err const ADDR_CODE = uri_parser.requestAddrInfo(host, port, Uri::AddrFlags::MOST_IPV4);
            if (isFailure(ADDR_CODE)) {
                return ADDR_CODE;
            }
        }

        try {
            _server = std::make_shared<Server>(this, _loop, type);
        } catch (...) {
            tDLogE("Node::Impl::openMain() Bad allocation.");
            _server.reset();
            return Err::E_BADALLOC;
        }

        assert(static_cast<bool>(_server));

        Err const INIT_CODE = _server->init(host.c_str(), port);
        if (isFailure(INIT_CODE)) {
            tDLogE("Node::Impl::openMain() Server init error: {}", INIT_CODE);
            _server.reset();
            return INIT_CODE;
        }

        if (_param.verbose) {
            loggingSelfInformation();
        }
        return Err::E_SUCCESS;
    }

    Err closeMain()
    {
        if (_idle && !_idle->isClosing()) {
            _idle->close();
        }
        if (_async && !_async->isClosing()) {
            _async->close();
        }
        if (_server) {
            _server->close();
        }
        for (auto & client : _clients) {
            if (client.second) {
                client.second->close();
            }
        }
        _clients.clear();
        return Err::E_SUCCESS;
    }

    Err connectMain(std::string const & client_name, std::string const & server_uri)
    {
        StreamType  type;
        std::string host;
        int         port;

        libtbag::network::Uri uri_parser;
        if (!uri_parser.parse(server_uri)) {
            return Err::E_PARSING;
        }

        auto const SCHEMA = libtbag::string::lower(uri_parser.getSchema());
        if (SCHEMA == PIPE_SCHEMA) {
            type = StreamType::PIPE;
            host = uri_parser.getHost();
            port = 0;
        } else {
            type = StreamType::TCP;
            Err const ADDR_CODE = uri_parser.requestAddrInfo(host, port, Uri::AddrFlags::MOST_IPV4);
            if (isFailure(ADDR_CODE)) {
                return ADDR_CODE;
            }
        }

        SharedClient client;
        try {
            client = std::make_shared<Client>(this, _loop, type);
        } catch (...) {
            tDLogE("Node::Impl::connectMain() Bad allocation.");
            return Err::E_BADALLOC;
        }

        assert(static_cast<bool>(client));

        Err const INIT_CODE = client->init(host.c_str(), port);
        if (isFailure(INIT_CODE)) {
            tDLogE("Node::Impl::connectMain() Server init error: {}", INIT_CODE);
            return INIT_CODE;
        }

        return _clients.insert(std::make_pair(client_name, client)).second ? Err::E_SUCCESS : Err::E_INSERT;
    }

    Err disconnectMain(std::string const & client_name)
    {
        auto itr = _clients.find(client_name);
        if (itr == _clients.end()) {
            return Err::E_ENFOUND;
        }
        auto client = itr->second;
        _clients.erase(itr);
        if (client) {
            client->close();
        }
        return Err::E_SUCCESS;
    }

    Err c2sMain(std::string const & server_name, char const * buffer, std::size_t size)
    {
        return Err::E_UNSUPOP;
    }

    Err s2cMain(std::string const & client_name, char const * buffer, std::size_t size)
    {
        return Err::E_UNSUPOP;
    }
};

// --------------------
// Node implementation.
// --------------------

Node::Node() : _impl(std::make_shared<Impl>(this)), _event(nullptr)
{
    // EMPTY.
}

Node::Node(Node const & obj) TBAG_NOEXCEPT : Node()
{
    (*this) = obj;
}

Node::Node(Node && obj) TBAG_NOEXCEPT : Node()
{
    (*this) = std::move(obj);
}

Node::~Node()
{
    // EMPTY.
}

Node & Node::operator =(Node const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

Node & Node::operator =(Node && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void Node::copy(Node const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl = obj._impl;
        _event = obj._event;
    }
}

void Node::swap(Node & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl.swap(obj._impl);
        _event.swap(obj._event);
    }
}

Node::Param Node::getDefaultParam()
{
    Param param;
    param.verbose = false;
    param.timeout = 0;
    return param;
}

Err Node::open(std::string const & uri)
{
    assert(exists());
    return _impl->open(uri);
}

Err Node::close()
{
    assert(exists());
    return _impl->close();
}

Err Node::connect(std::string const & client_name, std::string const & server_uri)
{
    assert(exists());
    return _impl->connect(client_name, server_uri);
}

Err Node::disconnect(std::string const & client_name)
{
    assert(exists());
    return _impl->disconnect(client_name);
}

Err Node::c2s(std::string const & server_name, char const * buffer, std::size_t size)
{
    assert(exists());
    return _impl->c2s(server_name, buffer, size);
}

Err Node::s2c(std::string const & client_name, char const * buffer, std::size_t size)
{
    assert(exists());
    return _impl->c2s(client_name, buffer, size);
}

} // namespace node
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

