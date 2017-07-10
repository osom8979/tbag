/**
 * @file   FunctionalNet.hpp
 * @brief  FunctionalNet class prototype.
 * @author zer0
 * @date   2017-05-23
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DETAILS_FUNCTIONALNET_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DETAILS_FUNCTIONALNET_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/Type.hpp>

#include <libtbag/network/details/NetCommon.hpp>
#include <libtbag/uvpp/Loop.hpp>

#include <functional>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace details {

/**
 * FunctionalClient class prototype.
 *
 * @author zer0
 * @date   2017-05-08
 */
template <typename BaseType>
struct FunctionalClient : public BaseType
{
    // @formatter:off
    using Parent = BaseType;
    using Loop   = uvpp::Loop;

    STATIC_ASSERT_CHECK_IS_BASE_OF(ClientInterface, Parent);

    using OnConnect  = std::function<void(Err)>;
    using OnShutdown = std::function<void(Err)>;
    using OnWrite    = std::function<void(Err)>;
    using OnRead     = std::function<void(Err, ReadPacket const &)>;
    using OnClose    = std::function<void(void)>;
    using OnTimer    = std::function<void(void)>;

    OnConnect   connect_cb;
    OnShutdown  shutdown_cb;
    OnWrite     write_cb;
    OnRead      read_cb;
    OnClose     close_cb;
    OnTimer     timer_cb;

    FunctionalClient(Loop & loop) : Parent(loop)
    { /* EMPTY */ }
    virtual ~FunctionalClient()
    { /* EMPTY */ }

    inline void setOnConnect (OnConnect  const & cb) { connect_cb  = cb; }
    inline void setOnShutdown(OnShutdown const & cb) { shutdown_cb = cb; }
    inline void setOnWrite   (OnWrite    const & cb) { write_cb    = cb; }
    inline void setOnRead    (OnRead     const & cb) { read_cb     = cb; }
    inline void setOnClose   (OnClose    const & cb) { close_cb    = cb; }
    inline void setOnTimer   (OnTimer    const & cb) { timer_cb    = cb; }

    virtual void onConnect(Err code) override
    { if (connect_cb) { connect_cb(code); } }
    virtual void onShutdown(Err code) override
    { if (shutdown_cb) { shutdown_cb(code); } }
    virtual void onWrite(Err code) override
    { if (write_cb) { write_cb(code); } }
    virtual void onRead(Err code, ReadPacket const & packet) override
    { if (read_cb) { read_cb(code, packet); } }
    virtual void onClose() override
    { if (close_cb) { close_cb(); } }
    virtual void onTimer() override
    { if (timer_cb) { timer_cb(); } }
    // @formatter:on
};

/**
 * FunctionalServer class prototype.
 *
 * @author zer0
 * @date   2017-05-08
 */
template <typename BaseType>
struct FunctionalServer : public BaseType
{
    using Parent = BaseType;
    using Loop   = uvpp::Loop;

    STATIC_ASSERT_CHECK_IS_BASE_OF(ServerInterface, Parent);

    using SharedClient = ServerInterface::SharedClient;
    using   WeakClient = ServerInterface::WeakClient;

    using OnConnection     = std::function<void(Err)>;
    using OnClientShutdown = std::function<void(WeakClient, Err)>;
    using OnClientWrite    = std::function<void(WeakClient, Err)>;
    using OnClientRead     = std::function<void(WeakClient, Err, ReadPacket const &)>;
    using OnClientClose    = std::function<void(WeakClient)>;
    using OnServerClose    = std::function<void(void)>;

    using OnClientUdataAlloc   = std::function<void*(WeakClient)>;
    using OnClientUdataDealloc = std::function<void(WeakClient, void*)>;

    OnConnection     connection_cb;
    OnClientShutdown client_shutdown_cb;
    OnClientWrite    client_write_cb;
    OnClientRead     client_read_cb;
    OnClientClose    client_close_cb;
    OnServerClose    server_close_cb;

    OnClientUdataAlloc   client_udata_alloc_cb;
    OnClientUdataDealloc client_udata_dealloc_cb;

    FunctionalServer(Loop & loop) : Parent(loop)
    { /* EMPTY */ }
    virtual ~FunctionalServer()
    { /* EMPTY */ }

    inline void setOnConnection    (OnConnection     const & cb) { connection_cb      = cb; }
    inline void setOnClientShutdown(OnClientShutdown const & cb) { client_shutdown_cb = cb; }
    inline void setOnClientWrite   (OnClientWrite    const & cb) { client_write_cb    = cb; }
    inline void setOnClientRead    (OnClientRead     const & cb) { client_read_cb     = cb; }
    inline void setOnClientClose   (OnClientClose    const & cb) { client_close_cb    = cb; }
    inline void setOnServerClose   (OnServerClose    const & cb) { server_close_cb    = cb; }

    inline void setOnClientUdataAlloc  (OnClientUdataAlloc   const & cb) { client_udata_alloc_cb   = cb; }
    inline void setOnClientUdataDealloc(OnClientUdataDealloc const & cb) { client_udata_dealloc_cb = cb; }

    virtual void onConnection(Err code) override
    { if (connection_cb) { connection_cb(code); } }

    virtual void onClientShutdown(WeakClient node, Err code) override
    { if (client_shutdown_cb) { client_shutdown_cb(node, code); } }
    virtual void onClientWrite(WeakClient node, Err code) override
    { if (client_write_cb) { client_write_cb(node, code); } }
    virtual void onClientRead(WeakClient node, Err code, ReadPacket const & packet) override
    { if (client_read_cb) { client_read_cb(node, code, packet); } }
    virtual void onClientClose(WeakClient node) override
    { if (client_close_cb) { client_close_cb(node); } }
    virtual void onServerClose() override
    { if (server_close_cb) { server_close_cb(); } }

    virtual void * onClientUdataAlloc(WeakClient node) override
    { if (client_udata_alloc_cb) { return client_udata_alloc_cb(node); } return nullptr; }
    virtual void onClientUdataDealloc(WeakClient node, void * data) override
    { if (client_udata_dealloc_cb) { client_udata_dealloc_cb(node, data); } }
};

} // namespace details
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DETAILS_FUNCTIONALNET_HPP__

