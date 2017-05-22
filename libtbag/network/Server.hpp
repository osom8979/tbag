/**
 * @file   Server.hpp
 * @brief  Server class prototype.
 * @author zer0
 * @date   2017-05-02
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_SERVER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_SERVER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/Type.hpp>

#include <libtbag/network/details/NetCommon.hpp>
#include <libtbag/network/Client.hpp>

#include <functional>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

// Forward declaration.
namespace uvpp { class Loop; }

namespace network {

/**
 * Server class prototype.
 *
 * @author zer0
 * @date   2017-05-02
 */
struct Server : public details::NetCommon
{
    using SharedClient = std::shared_ptr<Client>;
    using   WeakClient =   std::weak_ptr<Client>;

    // @formatter:off
    virtual Type getType() const = 0;
    virtual Id getId() const = 0;

    virtual bool init(String const & destination, int port = 0) = 0;
    virtual void close() = 0;

    virtual WeakClient accept() = 0;
    // @formatter:on

    // ---------------
    // Event callback.
    // ---------------

    // @formatter:off
    virtual void onConnection    (Err code)       { /* EMPTY. */ }
    virtual void onClientShutdown(WeakClient node, Err code) { /* EMPTY. */ }
    virtual void onClientWrite   (WeakClient node, Err code) { /* EMPTY. */ }
    virtual void onClientRead    (WeakClient node, Err code, char const * buffer, Size size) { /* EMPTY. */ }
    virtual void onClientClose   (WeakClient node) { /* EMPTY. */ }
    virtual void onServerClose   ()                { /* EMPTY. */ }

    virtual void * onClientUserDataAlloc(WeakClient node) { return nullptr; }
    virtual void   onClientUserDataDealloc(WeakClient node, void * data) { /* EMPTY. */ }
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
    // @formatter:off
    using Parent = BaseType;

    STATIC_ASSERT_CHECK_IS_BASE_OF(Server, Parent);

    using Loop = uvpp::Loop;
    using Size = details::NetCommon::Size;
    using WeakClient = Server::WeakClient;

    using OnConnection     = std::function<void(Err)>;
    using OnClientShutdown = std::function<void(WeakClient, Err)>;
    using OnClientWrite    = std::function<void(WeakClient, Err)>;
    using OnClientRead     = std::function<void(WeakClient, Err, char const *, Size)>;
    using OnClientClose    = std::function<void(WeakClient)>;
    using OnServerClose    = std::function<void(void)>;

    using OnClientUserDataAlloc   = std::function<void*(WeakClient)>;
    using OnClientUserDataDealloc = std::function<void(WeakClient, void*)>;

    OnConnection     connection_cb;
    OnClientShutdown client_shutdown_cb;
    OnClientWrite    client_write_cb;
    OnClientRead     client_read_cb;
    OnClientClose    client_close_cb;
    OnServerClose    server_close_cb;

    OnClientUserDataAlloc   client_userdata_alloc_cb;
    OnClientUserDataDealloc client_userdata_dealloc_cb;

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

    inline void setOnClientUserDataAlloc  (OnClientUserDataAlloc   const & cb) { client_userdata_alloc_cb   = cb; }
    inline void setOnClientUserDataDealloc(OnClientUserDataDealloc const & cb) { client_userdata_dealloc_cb = cb; }

    virtual void onConnection(Err code) override
    { if (connection_cb) { connection_cb(code); } }
    virtual void onClientShutdown(WeakClient node, Err code) override
    { if (client_shutdown_cb) { client_shutdown_cb(node, code); } }
    virtual void onClientWrite(WeakClient node, Err code) override
    { if (client_write_cb) { client_write_cb(node, code); } }
    virtual void onClientRead(WeakClient node, Err code, char const * buffer, Size size) override
    { if (client_read_cb) { client_read_cb(node, code, buffer, size); } }
    virtual void onClientClose(WeakClient node) override
    { if (client_close_cb) { client_close_cb(node); } }
    virtual void onServerClose() override
    { if (server_close_cb) { server_close_cb(); } }

    virtual void * onClientUserDataAlloc(WeakClient node) override
    { if (client_userdata_alloc_cb) { return client_userdata_alloc_cb(node); } return nullptr; }
    virtual void onClientUserDataDealloc(WeakClient node, void * data) override
    { if (client_userdata_dealloc_cb) { client_userdata_dealloc_cb(node, data); } }
    // @formatter:on
};

} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_SERVER_HPP__

