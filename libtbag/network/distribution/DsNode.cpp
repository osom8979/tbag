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
    struct NodeInterface;
    struct ServerNode;
    struct ClientNode;
    struct Server;
    struct Idle;

    friend struct NodeInterface;
    friend struct ServerNode;
    friend struct ClientNode;
    friend struct Server;
    friend struct Idle;

    using SharedServerNode = std::shared_ptr<ServerNode>;
    using SharedClientNode = std::shared_ptr<ClientNode>;
    using SharedServer     = std::shared_ptr<Server>;
    using SharedIdle       = std::shared_ptr<Idle>;
    using SharedAsync      = std::shared_ptr<SafetyAsync>;
    using SharedPrepare    = std::shared_ptr<FuncPrepare>;
    using SharedThread     = std::shared_ptr<ThreadPool>;
    using SharedNode       = std::shared_ptr<NodeInterface>;
    using NodeMap          = std::unordered_map<std::string, SharedNode>;

public:
    TBAG_CONSTEXPR static char const * const HEADER_HOST   = libtbag::network::http::base::HEADER_HOST;
    TBAG_CONSTEXPR static char const * const HEADER_ORIGIN = libtbag::network::http::base::HEADER_ORIGIN;
    TBAG_CONSTEXPR static char const * const PIPE_SCHEMA   = "pipe";
    TBAG_CONSTEXPR static unsigned int WAIT_OPERATIONS_MILLISEC = 16 * 1000;

public:
    enum class NodeType
    {
        NT_SERVER,
        NT_CLIENT,
    };

public:
    struct Params
    {
        std::string uri;

        TBAG_CONSTEXPR static char const * const QUERY_NAME = "name";
        std::string name; ///< If it does not exist, replace it with a uri.

        TBAG_CONSTEXPR static char const * const QUERY_VERBOSE = "verbose";
        bool verbose = false;   ///< Verbose logging.
    };

    /**
     * DsNode::Impl::NodeInterface class implementation.
     *
     * @author zer0
     * @date   2018-10-26
     */
    struct NodeInterface
    {
    private:
        NodeType const NODE_TYPE;

    protected:
        std::string      _name;
        std::atomic_bool _connected;

    public:
        NodeInterface(NodeType type) : NODE_TYPE(type), _name(), _connected(false)
        { /* EMPTY. */ }
        NodeInterface(NodeType type, std::string const & name) : NODE_TYPE(type), _name(name), _connected(false)
        { /* EMPTY. */ }
        virtual ~NodeInterface()
        { /* EMPTY. */ }

    public:
        inline NodeType getNodeType() const TBAG_NOEXCEPT
        { return NODE_TYPE; }

        std::string getName() const
        { return _name; }

        void setName(char const * name)
        { _name = (name != nullptr ? name : std::string()); }
        void setName(std::string const & name)
        { _name = name; }

        bool isConnected() const TBAG_NOEXCEPT
        { return _connected; }

        void setConnected(bool value = true) TBAG_NOEXCEPT
        { _connected = value; }
        void setDisconnected() TBAG_NOEXCEPT
        { _connected = false; }

    public:
        Err castedClose()
        {
            if (NODE_TYPE == NodeType::NT_SERVER) {
                return ((ServerNode*)(this))->close();
            }
            assert(NODE_TYPE == NodeType::NT_CLIENT);
            return ((ClientNode*)(this))->close();
        }

        Err castedWrite(char const * buffer, std::size_t size)
        {
            if (NODE_TYPE == NodeType::NT_SERVER) {
                return ((ServerNode*)(this))->writeBinary(buffer, size);
            }
            assert(NODE_TYPE == NodeType::NT_CLIENT);
            return ((ClientNode*)(this))->writeBinary(buffer, size);
        }
    };

public:
    /**
     * DsNode::Impl::ClientNode class implementation.
     *
     * @author zer0
     * @date   2018-10-26
     * @date   2018-11-01 (Rename: Client -> ClientNode)
     */
    struct ClientNode : public HttpClient, public NodeInterface
    {
    public:
        using Base = HttpClient;

    private:
        Impl * _impl = nullptr;

    public:
        ClientNode(Impl * i, Loop & loop, StreamType type, std::string const & name)
                : Base(loop, type), NodeInterface(NodeType::NT_CLIENT, name), _impl(i)
        { assert(_impl != nullptr); }

        virtual ~ClientNode()
        { /* EMPTY. */ }

        // -----------------------
        // ClientInterface events.
        // -----------------------

        virtual void onConnect(Err code) override
        { Base::onConnect(code); }

        virtual void onShutdown(Err code) override
        { /* EMPTY. */ }

        virtual void onWrite(Err code) override
        { /* EMPTY. */ }

        virtual void onRead(Err code, ReadPacket const & packet) override
        { Base::onRead(code, packet); }

        virtual void onClose() override
        {
            if (_name.empty()) {
                return;
            }

            assert(_impl != nullptr);
            assert(_impl->_parent != nullptr);
            auto & lock  = _impl->_nodes_lock;
            auto & nodes = _impl->_nodes;
            auto & event = _impl->_parent->event();

            if (event) {
                event->onDisconnect(_name);
            }

            WriteGuard const GUARD(lock);
            auto itr = nodes.find(_name);
            if (itr != nodes.end()) {
                nodes.erase(itr);
                tDLogI("DsNode::I::C::onClose() Erase node: {}", _name);
            } else {
                // Not found node. Do not print log message.
            }
        }

        virtual void onTimer() override
        { /* EMPTY. */ }

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
        { Base::onEof(); }

        virtual void onError(EventType from, Err code) override
        { Base::onError(from, code); }

        // ---------------------------
        // HttpReaderInterface events.
        // ---------------------------

        virtual void onContinue(void * arg) override
        { /* EMPTY. */ }

        virtual bool onSwitchingProtocol(HttpProperty const & property, void * arg) override
        {
            assert(_impl != nullptr);
            assert(_impl->_parent != nullptr);
            auto & event = _impl->_parent->event();
            if (event) {
                event->onConnect(_name);
            }
            return Base::onSwitchingProtocol(property, arg);
        }

        virtual void onWsMessage(WsOpCode opcode, util::Buffer const & payload, void * arg) override
        {
            assert(_impl != nullptr);
            assert(_impl->_parent != nullptr);
            auto & event = _impl->_parent->event();
            if (event) {
                event->onRecv(_name, payload.data(), payload.size());
            }
        }

        virtual void onRegularHttp(HttpProperty const & property, void * arg) override
        { /* EMPTY. */ }

        virtual void onParseError(Err code, void * arg) override
        { Base::onParseError(code, arg); }
    };

    /**
     * DsNode::Impl::ServerNode class implementation.
     *
     * @author zer0
     * @date   2018-10-31
     * @date   2018-11-01 (Rename: ClientNode -> ServerNode)
     */
    struct ServerNode : public HttpNode, public NodeInterface
    {
    public:
        using Base = HttpNode;

    public:
        ServerNode(Loop & loop, StreamType type, HttpServer * parent)
                : Base(loop, type, parent), NodeInterface(NodeType::NT_SERVER)
        { /* EMPTY. */ }
        virtual ~ServerNode()
        { /* EMPTY. */ }
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
        { assert(_impl != nullptr); }

        virtual ~Server()
        { /* EMPTY. */ }

        // -----------------------
        // ServerInterface events.
        // -----------------------

        virtual void onConnection(Err code) override
        { Base::onConnection(code); }

        virtual void onClientShutdown(WeakClient node, Err code) override
        { /* EMPTY. */ }

        virtual void onClientWrite(WeakClient node, Err code) override
        { /* EMPTY. */ }

        // Don't implement this method.
        // virtual void onClientRead(WeakClient node, Err code, ReadPacket const & packet) override
        // { /* EMPTY. */ }

        virtual void onClientClose(WeakClient node) override
        {
            auto shared = std::static_pointer_cast<ServerNode>(node.lock());
            assert(static_cast<bool>(shared));
            assert(_impl != nullptr);
            assert(_impl->_parent != nullptr);

            auto & lock  = _impl->_nodes_lock;
            auto & nodes = _impl->_nodes;
            auto & event = _impl->_parent->event();
            auto const NAME = shared->getName();

            if (event) {
                event->onDisconnect(NAME);
            }

            WriteGuard const GUARD(lock);
            auto itr = nodes.find(NAME);
            if (itr != nodes.end()) {
                itr->second->setDisconnected();
                nodes.erase(itr);
                tDLogI("DsNode::I::S::onClientClose() Erase node: {}", NAME);
            } else {
                // Not found node. Do not print log message.
            }
        }

        virtual void onClientTimer(WeakClient node) override
        { /* EMPTY. */ }

        virtual void onServerClose() override
        { /* EMPTY. */ }

        // ------------------
        // HttpServer events.
        // ------------------

        SharedStreamNode createClient(StreamType type, Loop & loop, SharedStream & server) override
        { return std::make_shared<ServerNode>(loop, type, this); }

        virtual void onClientEof(WeakClient node) override
        { Base::onClientEof(node); }

        virtual void onClientReadError(WeakClient node, Err code) override
        { Base::onClientReadError(node, code); }

        virtual void onClientParseError(WeakClient node, Err code) override
        { Base::onClientParseError(node, code); }

        virtual bool onClientSwitchingProtocol(WeakClient node, HttpRequest const & request) override
        {
            bool result = false;
            auto shared = std::static_pointer_cast<ServerNode>(node.lock());
            assert(static_cast<bool>(shared));

            auto itr = request.find(HEADER_HOST);
            std::string host_name;

            if (itr == request.end()) {
                tDLogE("DsNode::I::S::oCSP() Not found hostname");
            } else {
                host_name = itr->second;
                if (host_name.empty()) {
                    tDLogE("DsNode::I::S::oCSP() Empty hostname");
                } else {
                    assert(_impl != nullptr);
                    auto & lock  = _impl->_nodes_lock;
                    auto & nodes = _impl->_nodes;

                    WriteGuard const GUARD(lock);
                    if (nodes.find(host_name) != nodes.end()) {
                        tDLogE("DsNode::I::S::oCSP() Exists hostname: {}", host_name);
                    } else {
                        result = nodes.insert(std::make_pair(host_name, shared)).second;
                        if (result) {
                            shared->setConnected();
                            shared->setName(host_name);
                        }
                    }
                }
            }

            if (result) {
                assert(_impl != nullptr);
                assert(_impl->_parent != nullptr);
                auto & event = _impl->_parent->event();
                if (event) {
                    event->onConnect(host_name);
                }
                tDLogI("DsNode::I::S::oCSP() Create new node: {}", host_name);
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
        { /* EMPTY. */ }

        virtual void onClientContinue(WeakClient node) override
        { /* EMPTY. */ }

        virtual void onClientWsMessage(WeakClient node, WsOpCode opcode, Buffer const & payload) override
        {
            auto shared = std::static_pointer_cast<ServerNode>(node.lock());
            assert(static_cast<bool>(shared));
            assert(_impl != nullptr);
            assert(_impl->_parent != nullptr);
            auto & event = _impl->_parent->event();
            if (event) {
                event->onRecv(shared->getName(), payload.data(), payload.size());
            }
        }

        virtual void onClientRequest(WeakClient node, HttpRequest const & request) override
        { /* EMPTY. */ }
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
        { assert(_impl != nullptr); }
        virtual ~Idle()
        { /* EMPTY. */ }

    public:
        virtual void onIdle() override
        { /* EMPTY. */ }
    };

private:
    DsNode * _parent;
    Params   _params;

private:
    Loop          _loop;
    SharedIdle    _idle;
    SharedPrepare _prepare;
    SharedAsync   _async;
    SharedServer  _server;

private:
    RwLock mutable _nodes_lock;
    NodeMap _nodes;

public:
    std::atomic<State> _state;
    std::atomic_int _write_counter;

private:
    SharedThread _pool;

public:
    Impl(DsNode * parent, std::string const & uri)
            : _parent(parent), _state(State::S_OPENING), _write_counter(0)
    {
        assert(_parent != nullptr);

        StreamType  type;
        std::string host;
        int         port;

        STEP(01, "URI parsing");
        libtbag::network::Uri uri_parser;
        if (!uri_parser.parse(uri)) {
            throw ErrException(Err::E_PARSING);
        }

        STEP(02, "Update Server information");
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

        STEP(03, "Acquire parameters") {
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

        STEP(04, "Create uv handles");
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

        STEP(05, "Server initialize");
        Err const INIT_CODE = _server->init(host.c_str(), port);
        if (isFailure(INIT_CODE)) {
            _server.reset();
            throw ErrException(INIT_CODE);
        }

        STEP(06, "Update callback");
        _prepare->setOnPrepare([this](){
            _prepare->close();
            _prepare.reset();

            auto server = _server->getServer().lock();
            assert(static_cast<bool>(server));
            if (server->isActive()) {
                if (_params.verbose) {
                    loggingSelfInformation();
                }
                _state = State::S_OPENED; // ==[[ OPENING DONE ]]==
            } else {
                tDLogE("DsNode::Impl::Impl()@prepare() Server is not active!");
                close();
            }
        });
        _prepare->start();

        STEP(07, "Create thread");
        _pool = std::make_shared<ThreadPool>(_POOL_SIZE);
        assert(static_cast<bool>(_pool));
        assert(_pool->sizeOfThreads() == _POOL_SIZE);
        if (!_pool->push([this](){ runner(); })) {
            throw ErrException(Err::E_EPUSH);
        }

        STEP(08, "Update member variables");
        _params = params;
    }

    ~Impl()
    {
        close();
    }

private:
    void close()
    {
        STEP(01, "Update CLOSING state");
        _state = State::S_CLOSING;

        using namespace std::chrono;
        auto const CLOSE_BEGIN = system_clock::now();
        auto const CLOSE_TIMEOUT = milliseconds(WAIT_OPERATIONS_MILLISEC);

        STEP(02, "Wait operations");
        while (_write_counter == 0) {
            if ((system_clock::now() - CLOSE_BEGIN) >= CLOSE_TIMEOUT) {
                tDLogW("DsNode::Impl::close() Close timeout!");
                break;
            }
        }

        STEP(03, "Close uv handles");
        // {{~~
        _nodes_lock.readLock();
        for (auto & node : _nodes) {
            if (node.second) {
                node.second->castedClose();
            }
        }
        _nodes_lock.readUnlock();
        // ~~}}
        if (_server) {
            _server->close();
        }
        if (_idle && !_idle->isClosing()) {
            _idle->close();
        }
        if (_prepare && !_prepare->isClosing()) {
            _prepare->close();
        }
        if (_async && !_async->isClosing()) {
            _async->close();
        }

        STEP(04, "Join background thread");
        assert(static_cast<bool>(_pool));
        if (!_pool->waitPush([this](){ _pool->exit(); })) {
            tDLogW("DsNode::Impl::close() Task push error.");
        }
        _pool.reset();

        STEP(05, "Clear memory");
        _idle.reset();
        _prepare.reset();
        _async.reset();
        _server.reset();
        // {{~~
        _nodes_lock.writeLock();
        _nodes.clear();
        _nodes_lock.writeUnlock();
        // ~~}}
    }

    void runner()
    {
        tDLogI("DsNode::Impl::runner() Start.");
        Err const CODE = _loop.run();
        if (isSuccess(CODE)) {
            tDLogI("DsNode::Impl::runner() Done.");
        } else {
            tDLogE("DsNode::Impl::runner() Done, error: {}", CODE);
        }
    }

public:
    void loggingSelfInformation() const
    {
        using namespace libtbag::util;
        tDLogI("===== DsNoD INFORMATION =====");
        tDLogI("* Tbag version: {}", getTbagVersion().toString());
        tDLogI("* Packet version: {}", getTbagPacketVersion().toString());
        tDLogI("* Release version: {}", getTbagReleaseVersion().toString());

        assert(static_cast<bool>(_server));
        tDLogI("* Server: {}:{})", _server->dest(), _server->port());
    }

    std::vector<std::string> nodes() const
    {
        std::vector<std::string> result;
        ReadGuard const GUARD(_nodes_lock);
        for (auto & node : _nodes) {
            result.push_back(node.first);
        }
        return result;
    }

    Err isConnected(std::string const & name, bool & result) const
    {
        ReadGuard const GUARD(_nodes_lock);
        auto itr = _nodes.find(name);
        if (itr == _nodes.end()) {
            return Err::E_ENFOUND;
        }
        if (!static_cast<bool>(itr->second)) {
            return Err::E_EXPIRED;
        }
        result = itr->second->isConnected();
        return Err::E_SUCCESS;
    }

public:
    Err connect(std::string const & uri)
    {
        return connect(uri, uri);
    }

    Err connect(std::string const & name, std::string const & uri)
    {
        if (_state != State::S_OPENED) {
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
        if (_state != State::S_OPENED) {
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
        if (_state != State::S_OPENED) {
            return Err::E_EBUSY;
        }
        Err code;
        ++_write_counter;
        code = writeMain(name, buffer, size);
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

        SharedClientNode client;
        try {
            client = std::make_shared<ClientNode>(this, _loop, type, name);
        } catch (...) {
            tDLogE("DsNode::Impl::connectMain() Bad allocation.");
            return Err::E_BADALLOC;
        }

        assert(static_cast<bool>(client));
        bool insert_result = false;
        COMMENT("INSERT CLIENT") {
            WriteGuard const GUARD(_nodes_lock);
            if (_nodes.find(name) == _nodes.end()) {
                if (!_nodes.insert(std::make_pair(name, client)).second) {
                    return Err::E_INSERT;
                }
            } else {
                return Err::E_EEXIST;
            }
        }

        Err const INIT_CODE = client->init(host.c_str(), port);
        if (isFailure(INIT_CODE)) {
            tDLogE("DsNode::Impl::connectMain() Server init error: {}", INIT_CODE);
            auto const ERASE_RESULT = _nodes.erase(name);
            assert(ERASE_RESULT == 1);
            return INIT_CODE;
        }
        return Err::E_SUCCESS;
    }

    Err disconnectMain(std::string const & name)
    {
        SharedNode node;
        COMMENT("NODES READ LOCK") {
            ReadGuard const GUARD(_nodes_lock);
            auto itr = _nodes.find(name);
            if (itr == _nodes.end()) {
                return Err::E_ENFOUND;
            }
            if (!static_cast<bool>(itr->second)) {
                return Err::E_EXPIRED;
            }
            node = itr->second;
        }
        return node->castedClose();
    }

    Err writeMain(std::string const & name, char const * buffer, std::size_t size)
    {
        SharedNode node;
        COMMENT("NODES READ LOCK") {
            ReadGuard const GUARD(_nodes_lock);
            auto itr = _nodes.find(name);
            if (itr == _nodes.end()) {
                return Err::E_ENFOUND;
            }
            if (!static_cast<bool>(itr->second)) {
                return Err::E_EXPIRED;
            }
            node = itr->second;
        }

        if (node->isConnected()) {
            return node->castedWrite(buffer, size);
        } else {
            return Err::E_NREADY;
        }
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

std::vector<std::string> DsNode::nodes() const
{
    if (!exists()) {
        return std::vector<std::string>();
    }
    return _impl->nodes();
}

bool DsNode::isConnected(std::string const & name) const
{
    if (!exists()) {
        return false;
    }
    bool result;
    if (isSuccess(_impl->isConnected(name, result))) {
        return result;
    }
    return false;
}

Err DsNode::connect(std::string const & client_name, std::string const & server_uri)
{
    if (!exists()) {
        return Err::E_EOPEN;
    }
    return _impl->connect(client_name, server_uri);
}

Err DsNode::disconnect(std::string const & client_name)
{
    if (!exists()) {
        return Err::E_EOPEN;
    }
    return _impl->disconnect(client_name);
}

Err DsNode::write(std::string const & name, char const * buffer, std::size_t size)
{
    if (!exists()) {
        return Err::E_EOPEN;
    }
    return _impl->write(name, buffer, size);
}

} // namespace distribution
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

