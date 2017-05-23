/**
 * @file   StreamServer.hpp
 * @brief  StreamServer class prototype.
 * @author zer0
 * @date   2017-05-10
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_STREAM_STREAMSERVER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_STREAM_STREAMSERVER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/Type.hpp>
#include <libtbag/log/Log.hpp>

#include <libtbag/uvpp/Stream.hpp>
#include <libtbag/network/Server.hpp>
#include <libtbag/network/stream/StreamClient.hpp>
#include <libtbag/network/stream/StreamServerNode.hpp>
#include <libtbag/network/stream/StreamServerBackend.hpp>

#include <cassert>
#include <atomic>
#include <unordered_map>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace stream  {

/**
 * StreamServer class prototype.
 *
 * @author zer0
 * @date   2017-05-10
 */
template <typename BaseStreamType, typename BaseStreamClientType>
class StreamServer : public Server
{
public:
    using BaseStream       = BaseStreamType;
    using BaseStreamClient = BaseStreamClientType;

    STATIC_ASSERT_CHECK_IS_BASE_OF(uvpp::Stream, BaseStream);

public:
    using ClientNode = StreamServerNode<BaseStreamClient>;
    using ServerBackend = StreamServerBackend<BaseStream>;

public:
    using AtomicBool = std::atomic_bool;

    AtomicBool _on_connection;

    inline bool isOnConnection() const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_on_connection.load()))
    { return _on_connection.load(); }

    virtual void runBackendConnection(Err code) override
    {
        _on_connection.store(true);
        onConnection(code);
        _on_connection.store(false);
    }

    virtual void runBackendClose() override
    {
        onServerClose();

        _mutex.lock();
        closeAll();
        _mutex.unlock();
    }

public:
    using SharedServerBackend = std::shared_ptr<ServerBackend>;
    using   WeakServerBackend =   std::weak_ptr<ServerBackend>;

public:
    using ClientKey  = Id;
    using ClientMap  = std::unordered_map<ClientKey, SharedClient>;
    using ClientPair = ClientMap::value_type;

public:
    using Mutex = std::mutex;
    using Guard = std::lock_guard<Mutex>;

private:
    mutable Mutex _mutex;

    SharedServerBackend     _server;
    SharedSafetyWriteAsync  _async;
    ClientMap               _clients;

public:
    StreamServer(Loop & loop)
    {
        _on_connection.store(false);

        _server = loop.newHandle<ServerBackend>(loop, this);
        _async  = loop.newHandle<SafetyWriteAsync>(loop);
        assert(static_cast<bool>(_server));
        assert(static_cast<bool>(_async));
    }

    virtual ~StreamServer()
    {
        _server.reset();
        _async.reset();
    }

public:
    // @formatter:off
    inline WeakServerBackend      getServer() { Guard g(_mutex); return WeakServerBackend     (_server); }
    inline SharedSafetyWriteAsync getAsync () { Guard g(_mutex); return SharedSafetyWriteAsync(_async);  }

    inline bool emptyClients() const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_clients.empty()))
    { Guard g(_mutex); return _clients.empty(); }
    inline Size sizeClients() const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_clients.size()))
    { Guard g(_mutex); return _clients.size(); }

    inline void     lock() TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(    _mutex.lock())) {   _mutex.lock(); }
    inline void   unlock() TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(  _mutex.unlock())) { _mutex.unlock(); }
    inline bool try_lock() TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_mutex.try_lock())) { return _mutex.try_lock(); }
    // @formatter:on

private:
    static SharedClient createClient(Loop & loop, StreamServer * server)
    {
        SharedClient client(new (std::nothrow) ClientNode(loop, server));
        if (static_cast<bool>(client)) {
            return client;
        }
        return SharedClient();
    }

    SharedClient createClient()
    {
        assert(static_cast<bool>(_server));

        Loop * loop = _server->getLoop();
        assert(loop != nullptr);

        return createClient(*loop, this);
    }

private:
    SharedClient getSharedClient(ClientKey key)
    {
        auto itr = _clients.find(key);
        if (itr != _clients.end()) {
            return itr->second;
        }
        return SharedClient();
    }

    bool insertClient(SharedClient client)
    {
        return _clients.insert(ClientPair(client->getId(), client)).second;
    }

    bool removeClient(ClientKey key)
    {
        return _clients.erase(key) == 1U;
    }

public:
    template <typename Predicated>
    void foreach(Predicated predicated)
    {
        Guard guard(_mutex);
        for (auto & cursor : _clients) {
            predicated(cursor);
        }
    }

    template <typename Predicated>
    void updateClients(Predicated predicated)
    {
        Guard guard(_mutex);
        predicated(_clients);
    }

private:
    void closeAll()
    {
        {   // Close all clients.
            for (auto & cursor : _clients) {
                if (static_cast<bool>(cursor.second)) {
                    cursor.second->close();
                }
            }
        }

        assert(static_cast<bool>(_server));
        assert(static_cast<bool>(_async));

        if (_async->isClosing() == false) {
            _async->close();
        }

        if (_server->isClosing() == false) {
            _server->close();
        }
    }

public:
    /** Obtain the Server handle id. */
    virtual Id getId() const override
    {
        return _server->id();
    }

public:
    /** Initialize this class. */
    virtual bool init(String const & destination, int port = 0) override
    {
        assert(static_cast<bool>(_server));
        Guard guard(_mutex);
        return realInitialize(*_server, destination, port);
    }

    /**
     * Safety close() operation.
     *
     * @remarks
     *  Select sync/async operations automatically according to Thread ID.
     */
    virtual void close() override
    {
        assert(static_cast<bool>(_server));
        assert(static_cast<bool>(_async));

        Loop * loop = _server->getLoop();
        assert(loop != nullptr);

        if (loop->isAliveAndThisThread()) {
            tDLogD("StreamServer::close() sync request.");
            Guard guard(_mutex);
            closeAll();
        } else {
            tDLogD("StreamServer::close() async request.");
            _async->newSendFunc([&](SafetyAsync * UNUSED_PARAM(async)) {
                Guard guard(_mutex);
                closeAll();
            });
        }
    }

    /**
     * Accept client.
     *
     * @warning
     *  This operation can only used with the onConnection() method.
     */
    virtual WeakClient accept() override
    {
        assert(static_cast<bool>(_server));
        if (isOnConnection() == false) {
            tDLogE("StreamServer::accept() server is not a connection state.");
            return WeakClient();
        }

        Guard guard(_mutex);
        auto client = std::static_pointer_cast<BaseStreamClient, Client>(createClient());
        if (auto shared = client->getClient().lock()) {
            Err const CODE = _server->accept(*shared);
            if (CODE == Err::E_SUCCESS) {
                tDLogD("StreamServer::accept() client connect.");
                bool const INSERT_RESULT = insertClient(client);
                assert(INSERT_RESULT);

                auto weak_client = WeakClient(client);
                shared->setUserData(this->onClientUserDataAlloc(weak_client));
                return weak_client;
            } else {
                tDLogE("StreamServer::accept() {} error.", getErrName(CODE));
            }
        } else {
            tDLogE("StreamServer::accept() client is nullptr.");
        }
        return WeakClient();
    }

    virtual WeakClient getClient(Id id) override
    {
        return WeakClient(getSharedClient(id));
    }


public:
    virtual bool realInitialize(ServerBackend & backend, String const & destination, int port) = 0;
};

} // namespace stream
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_STREAM_STREAMSERVER_HPP__

