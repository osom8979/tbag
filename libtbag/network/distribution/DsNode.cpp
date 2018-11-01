/**
 * @file   DsNode.cpp
 * @brief  DsNode class implementation.
 * @author zer0
 * @date   2018-10-26
 * @date   2018-11-01 (Rename: Node -> DsNode)
 */

#include <libtbag/network/distribution/DsNode.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/thread/ThreadPool.hpp>
#include <libtbag/signal/SignalHandler.hpp>

#include <libtbag/network/http/HttpServer.hpp>
#include <libtbag/network/http/HttpClient.hpp>

#include <libtbag/uvpp/ex/SafetyAsync.hpp>
#include <libtbag/uvpp/func/FunctionalPrepare.hpp>
#include <libtbag/uvpp/Idle.hpp>
#include <libtbag/uvpp/Signal.hpp>
#include <libtbag/uvpp/Loop.hpp>

#include <libtbag/string/StringUtils.hpp>
#include <libtbag/string/Format.hpp>
#include <libtbag/util/BufferInfo.hpp>
#include <libtbag/util/Version.hpp>
#include <libtbag/lock/RwLock.hpp>

#include <cassert>
#include <unordered_map>
#include <algorithm>
#include <utility>
#include <chrono>
#include <atomic>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network      {
namespace distribution {

/** @warning Don't change this variable. */
TBAG_CONSTEXPR static unsigned int const _POOL_SIZE = 1U;

/**
 * DsNode::Impl class implementation.
 *
 * @author zer0
 * @date   2018-10-26
 */
struct DsNode::Impl : private Noncopyable
{
public:
    using ThreadPool    = libtbag::thread::ThreadPool;
    using HttpServer    = libtbag::network::http::HttpServer;
    using HttpClient    = libtbag::network::http::HttpClient;
    using StreamType    = libtbag::network::details::StreamType;
    using HttpMethod    = libtbag::network::http::base::HttpMethod;
    using HttpRequest   = libtbag::network::http::HttpRequest;
    using HttpResponse  = libtbag::network::http::HttpResponse;
    using HttpProperty  = libtbag::network::http::HttpProperty;
    using WsOpCode      = libtbag::network::http::ws::WsOpCode;
    using EventType     = libtbag::network::http::EventType;
    using Loop          = libtbag::uvpp::Loop;
    using SafetyAsync   = libtbag::uvpp::ex::SafetyAsync;
    using FuncPrepare   = libtbag::uvpp::func::FuncPrepare;
    using RwLock        = libtbag::lock::RwLock;
    using ReadGuard     = libtbag::lock::ReadLockGuard;
    using WriteGuard    = libtbag::lock::WriteLockGuard;
    using HttpNode      = HttpServer::HttpNode;

public:
    struct Server;
    struct Client;

    friend struct Server;
    friend struct Client;

public:
    TBAG_CONSTEXPR static char const * const HEADER_HOST   = libtbag::network::http::base::HEADER_HOST;
    TBAG_CONSTEXPR static char const * const HEADER_ORIGIN = libtbag::network::http::base::HEADER_ORIGIN;

public:
    /**
     * DsNode::Impl::Client class implementation.
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

    private:
        std::string _name;

    public:
        Client(Impl * i, Loop & loop, StreamType type, std::string const & name)
                : Base(loop, type), _impl(i), _name(name)
        {
            assert(_impl != nullptr);
        }

        virtual ~Client()
        {
            assert(_impl != nullptr);
            WriteGuard const GUARD(_impl->_clients_lock);
            auto itr = _impl->_clients.find(_name);
            if (itr != _impl->_clients.end()) {
                _impl->_clients.erase(itr);
                tDLogI("DsNode::Impl::Client::~Client() Erase client: {}", _name);
            }
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
            HttpRequest request;
            request.setHttpMethod(HttpMethod::M_GET);
            request.path = "ws://tbag/node/";
            request.insert(HEADER_HOST, _impl->_params.name);
            request.insert(HEADER_ORIGIN, _impl->_params.name);
            writeWsRequest(request);
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
     * DsNode::Impl::ClientNode class implementation.
     *
     * @author zer0
     * @date   2018-10-31
     */
    struct ClientNode : public HttpNode
    {
    public:
        using Base = HttpNode;

    public:
        std::string name;

    public:
        ClientNode(Loop & loop, StreamType type, HttpServer * parent) : Base(loop, type, parent)
        {
            // EMPTY.
        }

        virtual ~ClientNode()
        {
            // EMPTY.
        }
    };

    /**
     * DsNode::Impl::Server class implementation.
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
        Server(Impl * i, Loop & loop, StreamType type) : Base(loop, type), _impl(i)
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
            auto shared = std::static_pointer_cast<ClientNode>(node.lock());
            assert(static_cast<bool>(shared));
            assert(_impl != nullptr);

            WriteGuard const GUARD(_impl->_nodes_lock);
            auto itr = _impl->_nodes.find(shared->name);
            if (itr != _impl->_nodes.end()) {
                _impl->_nodes.erase(itr);
                tDLogI("DsNode::Impl::Server::onClientClose() Erase node: {}", shared->name);
            }
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
            return SharedStreamNode(new (std::nothrow) ClientNode(loop, type, this));
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
            auto shared = std::static_pointer_cast<ClientNode>(node.lock());
            assert(static_cast<bool>(shared));
            assert(_impl != nullptr);

            bool result = false;
            auto itr = request.find(HEADER_HOST);
            if (itr != request.end()) {
                if (itr->second.empty()) {
                    tDLogE("DsNode::Impl::Server::onClientSwitchingProtocol() Empty hostname");
                } else {
                    WriteGuard const GUARD(_impl->_nodes_lock);
                    if (_impl->_nodes.find(itr->second) != _impl->_nodes.end()) {
                        tDLogE("DsNode::Impl::Server::onClientSwitchingProtocol() Exists hostname: {}", itr->second);
                    } else {
                        result = _impl->_nodes.insert(std::make_pair(itr->second, shared)).second;
                        if (result) {
                            shared->name = itr->second;
                        }
                    }
                }
            } else {
                tDLogE("DsNode::Impl::Server::onClientSwitchingProtocol() Not found hostname");
            }

            if (result) {
                tDLogI("DsNode::Impl::Server::onClientSwitchingProtocol() Create new node: {}", shared->name);
                return Base::onClientSwitchingProtocol(node, request); // Write WebSocket Response.
            } else {
                shared->close();
                return false;
            }
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
     * DsNode::Impl::Idle class implementation.
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

    using SharedNode    = std::shared_ptr<ClientNode>;
    using SharedServer  = std::shared_ptr<Server>;
    using SharedClient  = std::shared_ptr<Client>;
    using SharedIdle    = std::shared_ptr<Idle>;
    using SharedAsync   = std::shared_ptr<SafetyAsync>;
    using SharedPrepare = std::shared_ptr<FuncPrepare>;
    using SharedThread  = std::shared_ptr<ThreadPool>;

    using NodeMap   = std::unordered_map<std::string, SharedNode>;
    using ClientMap = std::unordered_map<std::string, SharedClient>;

public:
    TBAG_CONSTEXPR static unsigned int WAIT_OPERATIONS_MILLISEC = 16 * 1000;
    TBAG_CONSTEXPR static char const * const PIPE_SCHEMA = "pipe";

public:
    struct Params
    {
        std::string uri;

        TBAG_CONSTEXPR static char const * const QUERY_NAME = "name";
        std::string name; ///< If it does not exist, replace it with a uri.

        TBAG_CONSTEXPR static char const * const QUERY_VERBOSE = "verbose";
        bool verbose = false;   ///< Verbose logging.
    };

private:
    DsNode * _parent;
    Params   _params;

private:
    Loop         _loop;
    SharedIdle   _idle;
    SharedAsync  _async;
    SharedServer _server;

private:
    SharedPrepare _prepare;

private:
    RwLock    _clients_lock;
    ClientMap _clients;

private:
    RwLock  _nodes_lock;
    NodeMap _nodes;

public:
    std::atomic<NodeState> _state;
    std::atomic_int _write_counter;

private:
    SharedThread _pool;

public:
    Impl(DsNode * parent, std::string const & uri)
            : _parent(parent), _state(NodeState::NS_OPENING), _write_counter(0)
    {
        assert(_parent != nullptr);

        StreamType  type;
        std::string host;
        int         port;

        // STEP 01. URI parsing.
        libtbag::network::Uri uri_parser;
        if (!uri_parser.parse(uri)) {
            throw ErrException(Err::E_PARSING);
        }

        // STEP 02. Update Server information.
        auto const SCHEMA = libtbag::string::lower(uri_parser.getSchema());
        if (SCHEMA == PIPE_SCHEMA) {
            type = StreamType::PIPE;
            host = uri_parser.getHost();
            port = 0;
        } else {
            type = StreamType::TCP;
            Err const ADDR_CODE = uri_parser.requestAddrInfo(host, port, Uri::AddrFlags::MOST_IPV4);
            if (isFailure(ADDR_CODE)) {
                throw ErrException(ADDR_CODE);
            }
        }

        Params params;
        params.uri = uri;

        COMMENT("STEP 03. Update Params information") {
            auto const QUERYS = uri_parser.getQueryMap();

            auto name_itr = QUERYS.find(Params::QUERY_NAME);
            if (name_itr != QUERYS.end()) {
                params.name = name_itr->second;
            } else {
                params.name = uri;
            }

            auto verbose_itr = QUERYS.find(Params::QUERY_VERBOSE);
            if (verbose_itr != QUERYS.end()) {
                params.verbose = libtbag::string::toValue(verbose_itr->second, false);
            } else {
                params.verbose = false;
            }
        }

        // STEP 04. Create uv handles.
        try {
            _prepare = _loop.newHandle<FuncPrepare>(_loop);
            _async = _loop.newHandle<SafetyAsync>(_loop);
            _idle = _loop.newHandle<Idle>(this, _loop);
            _server = std::make_shared<Server>(this, _loop, type);
        } catch (...) {
            _server.reset();
            throw ErrException(Err::E_BADALLOC);
        }
        assert(static_cast<bool>(_prepare));
        assert(static_cast<bool>(_async));
        assert(static_cast<bool>(_idle));
        assert(static_cast<bool>(_server));

        // STEP 05. Server initialize.
        Err const INIT_CODE = _server->init(host.c_str(), port);
        if (isFailure(INIT_CODE)) {
            _server.reset();
            throw ErrException(INIT_CODE);
        }

        // STEP 06. update callback.
        _prepare->setOnPrepare([this](){
            _prepare->close();
            _prepare.reset();
            _state = NodeState::NS_OPENED;
        });
        _prepare->start();

        // STEP 07. Create thread.
        _pool = std::make_shared<ThreadPool>(_POOL_SIZE);
        assert(static_cast<bool>(_pool));
        assert(_pool->sizeOfThreads() == _POOL_SIZE);
        if (!_pool->push([this](){ runMain(); })) {
            throw ErrException(Err::E_EPUSH);
        }

        // STEP 08. Update member variables.
        _params = params;
    }

    ~Impl()
    {
        // STEP 01. Update CLOSING state.
        _state = NodeState::NS_CLOSING;

        using namespace std::chrono;
        auto const CLOSE_BEGIN = system_clock::now();
        auto const CLOSE_TIMEOUT = milliseconds(WAIT_OPERATIONS_MILLISEC);

        // STEP 02. Wait c2s & s2c operations ...
        while (_write_counter == 0) {
            if (WAIT_OPERATIONS_MILLISEC != 0 && (system_clock::now() - CLOSE_BEGIN) >= CLOSE_TIMEOUT) {
                tDLogW("DsNode::Impl::~Impl() Close timeout!");
                break;
            }
        }

        // STEP 03. Close uv handles ...
        if (_idle && !_idle->isClosing()) {
            _idle->close();
        }
        if (_prepare && !_prepare->isClosing()) {
            _prepare->close();
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

        // STEP 04. Join thread.
        assert(static_cast<bool>(_pool));
        if (!_pool->waitPush([this](){ _pool->exit(); })) {
            tDLogW("DsNode::Impl::~Impl() Task push error.");
        }
        _pool.reset();

        // STEP 05. Memory clear
        _idle.reset();
        _prepare.reset();
        _async.reset();
        _server.reset();
        _clients.clear();
    }

private:
    void runMain()
    {
        Err const CODE = _loop.run();
        if (isFailure(CODE)) {
            tDLogE("DsNode::Impl::runMain() loop run error: {}", CODE);
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
    Err connect(std::string const & uri)
    {
        return connect(uri, uri);
    }

    Err connect(std::string const & name, std::string const & uri)
    {
        if (_state != NodeState::NS_OPENED) {
            return Err::E_EBUSY;
        }
        auto const JOB = _async->newSendFunc([this, name, uri](){
            auto const CODE = connectMain(name, uri);
            if (isFailure(CODE)) {
                if (name == uri) {
                    tDLogE("DsNode::Impl::connect({})@async() Open error: {}", name, CODE);
                } else {
                    tDLogE("DsNode::Impl::connect({}, {})@async() Open error: {}", name, uri, CODE);
                }
            }
        });
        return static_cast<bool>(JOB) ? Err::E_SUCCESS : Err::E_EPUSH;
    }

    Err disconnect(std::string const & name)
    {
        if (_state != NodeState::NS_OPENED) {
            return Err::E_EBUSY;
        }
        auto const JOB = _async->newSendFunc([this, name](){
            auto const CODE = disconnectMain(name);
            if (isFailure(CODE)) {
                tDLogE("DsNode::Impl::disconnect({})@async() Open error: {}", name, CODE);
            }
        });
        return static_cast<bool>(JOB) ? Err::E_SUCCESS : Err::E_EPUSH;
    }

    Err write(std::string const & name, char const * buffer, std::size_t size)
    {
        if (_state != NodeState::NS_OPENED) {
            return Err::E_EBUSY;
        }
        Err code;
        ++_write_counter;
        code = c2sMain(name, buffer, size);
        --_write_counter;
        return code;
    }

public:
    Err connectMain(std::string const & name, std::string const & uri)
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

        SharedClient client;
        try {
            client = std::make_shared<Client>(this, _loop, type, name);
        } catch (...) {
            tDLogE("DsNode::Impl::connectMain() Bad allocation.");
            return Err::E_BADALLOC;
        }

        assert(static_cast<bool>(client));
        bool insert_result = false;
        COMMENT("INSERT CLIENT") {
            WriteGuard const GUARD(_clients_lock);
            if (_clients.find(name) == _clients.end()) {
                if (!_clients.insert(std::make_pair(name, client)).second) {
                    return Err::E_INSERT;
                }
            } else {
                return Err::E_EEXIST;
            }
        }

        Err const INIT_CODE = client->init(host.c_str(), port);
        if (isFailure(INIT_CODE)) {
            tDLogE("DsNode::Impl::connectMain() Server init error: {}", INIT_CODE);
            return INIT_CODE;
        }
        return Err::E_SUCCESS;
    }

    Err disconnectMain(std::string const & name)
    {
        // Find first nodes.
        Err const CODE = disconnectNode(name);
        if (CODE != Err::E_ENFOUND) {
            return CODE;
        }
        // Find second clients.
        return disconnectClient(name);
    }

    Err disconnectNode(std::string const & name)
    {
        SharedNode node;
        COMMENT("CLIENTS WRITE LOCK") {
            WriteGuard const GUARD(_nodes_lock);
            auto itr = _nodes.find(name);
            if (itr == _nodes.end()) {
                return Err::E_ENFOUND;
            }
            node = itr->second;
            _nodes.erase(itr);
        }
        if (node) {
            return node->close();
        }
        return Err::E_EXPIRED;
    }

    Err disconnectClient(std::string const & name)
    {
        SharedClient client;
        COMMENT("CLIENTS WRITE LOCK") {
            WriteGuard const GUARD(_clients_lock);
            auto itr = _clients.find(name);
            if (itr == _clients.end()) {
                return Err::E_ENFOUND;
            }
            client = itr->second;
            _clients.erase(itr);
        }
        if (client) {
            return client->close();
        }
        return Err::E_EXPIRED;
    }

    Err c2sMain(std::string const & name, char const * buffer, std::size_t size)
    {
        SharedClient client;
        COMMENT("CLIENTS READ LOCK") {
            ReadGuard const GUARD(_clients_lock);
            auto itr = _clients.find(name);
            if (itr == _clients.end()) {
                return Err::E_ENFOUND;
            }
            client = itr->second;
        }

        if (client) {
            return client->writeBinary(buffer, size);
        }
        return Err::E_EXPIRED;
    }

    Err s2cMain(std::string const & name, char const * buffer, std::size_t size)
    {
        SharedNode node;
        COMMENT("NODES READ LOCK") {
            ReadGuard const GUARD(_nodes_lock);
            auto itr = _nodes.find(name);
            if (itr == _nodes.end()) {
                return Err::E_ENFOUND;
            }
            node = itr->second;
        }

        if (node) {
            return node->writeBinary(buffer, size);
        }
        return Err::E_EXPIRED;
    }
};

// ----------------------
// DsNode implementation.
// ----------------------

DsNode::DsNode() : _impl(nullptr), _event(nullptr)
{
    // EMPTY.
}

DsNode::DsNode(DsNode const & obj) TBAG_NOEXCEPT : DsNode()
{
    (*this) = obj;
}

DsNode::DsNode(DsNode && obj) TBAG_NOEXCEPT : DsNode()
{
    (*this) = std::move(obj);
}

DsNode::~DsNode()
{
    close();
}

DsNode & DsNode::operator =(DsNode const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

DsNode & DsNode::operator =(DsNode && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void DsNode::copy(DsNode const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl = obj._impl;
        _event = obj._event;
    }
}

void DsNode::swap(DsNode & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl.swap(obj._impl);
        _event.swap(obj._event);
    }
}

Err DsNode::open(std::string const & name, std::string const & schema,
                 std::string const & host, int port, bool verbose)
{
    using namespace libtbag::string;
    auto const URI = fformat("{}://{}:{}?{}={}&{}={}", schema, host, port,
                             std::string(Impl::Params::QUERY_NAME), name,
                             std::string(Impl::Params::QUERY_VERBOSE), verbose ? 1 : 0);
    return open(URI);
}

Err DsNode::open(std::string const & uri)
{
    try {
        _impl = std::make_shared<Impl>(this, uri);
    } catch (ErrException & e) {
        return e.CODE;
    } catch (std::exception & e) {
        return Err::E_UNKEXCP;
    } catch (...) {
        return Err::E_UNKNOWN;
    }
    return Err::E_SUCCESS;
}

void DsNode::close()
{
    _impl.reset();
}

Err DsNode::connect(std::string const & client_name, std::string const & server_uri)
{
    if (!exists()) {
        return Err::E_NREADY;
    }
    return _impl->connect(client_name, server_uri);
}

Err DsNode::disconnect(std::string const & client_name)
{
    if (!exists()) {
        return Err::E_NREADY;
    }
    return _impl->disconnect(client_name);
}

Err DsNode::write(std::string const & name, char const * buffer, std::size_t size)
{
    if (!exists()) {
        return Err::E_NREADY;
    }
    return _impl->write(name, buffer, size);
}

} // namespace distribution
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

