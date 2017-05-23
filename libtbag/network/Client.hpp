/**
 * @file   Client.hpp
 * @brief  Client class prototype.
 * @author zer0
 * @date   2017-05-02
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_CLIENT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_CLIENT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/Type.hpp>

#include <libtbag/id/Id.hpp>
#include <libtbag/uvpp/UvCommon.hpp>
#include <libtbag/uvpp/Loop.hpp>

#include <cstdint>
#include <string>
#include <functional>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {

/**
 * Client class prototype.
 *
 * @author zer0
 * @date   2017-05-02
 */
struct Client
{
    using binf = uvpp::binf;
    using Id   = id::Id;

    virtual Id getId() const = 0;

    virtual bool init(std::string const & destination, int port = 0, uint64_t millisec = 0) = 0;

    virtual bool  start() = 0;
    virtual bool   stop() = 0;
    virtual void  close() = 0;
    virtual void cancel() = 0;

    virtual bool write(binf const * buffer, std::size_t size, uint64_t millisec = 0) = 0;
    virtual bool write(char const * buffer, std::size_t size, uint64_t millisec = 0) = 0;

    virtual void * getUserData() = 0;

    virtual void runBackendConnect(Err code) = 0;
    virtual void runBackendShutdown(Err code) = 0;
    virtual void runBackendWrite(Err code) = 0;
    virtual void runBackendRead(Err code, char const * buffer, std::size_t size) = 0;
    virtual void runBackendClose() = 0;

    // ---------------
    // Event callback.
    // ---------------

    virtual void onConnect(Err code) { /* EMPTY. */ }
    virtual void onShutdown(Err code) { /* EMPTY. */ }
    virtual void onWrite(Err code) { /* EMPTY. */ }
    virtual void onRead(Err code, char const * buffer, std::size_t size) { /* EMPTY. */ }
    virtual void onClose() { /* EMPTY. */ }
};

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
    using Loop = uvpp::Loop;

    STATIC_ASSERT_CHECK_IS_BASE_OF(Client, Parent);

    using OnConnect  = std::function<void(Err)>;
    using OnShutdown = std::function<void(Err)>;
    using OnWrite    = std::function<void(Err)>;
    using OnRead     = std::function<void(Err, char const *, std::size_t)>;
    using OnClose    = std::function<void(void)>;

    OnConnect   connect_cb;
    OnShutdown  shutdown_cb;
    OnWrite     write_cb;
    OnRead      read_cb;
    OnClose     close_cb;

    FunctionalClient(Loop & loop) : Parent(loop)
    { /* EMPTY */ }
    virtual ~FunctionalClient()
    { /* EMPTY */ }

    inline void setOnConnect (OnConnect  const & cb) { connect_cb  = cb; }
    inline void setOnShutdown(OnShutdown const & cb) { shutdown_cb = cb; }
    inline void setOnWrite   (OnWrite    const & cb) { write_cb    = cb; }
    inline void setOnRead    (OnRead     const & cb) { read_cb     = cb; }
    inline void setOnClose   (OnClose    const & cb) { close_cb    = cb; }

    virtual void onConnect(Err code) override
    { if (connect_cb) { connect_cb(code); } }
    virtual void onShutdown(Err code) override
    { if (shutdown_cb) { shutdown_cb(code); } }
    virtual void onWrite(Err code) override
    { if (write_cb) { write_cb(code); } }
    virtual void onRead(Err code, char const * buffer, std::size_t size) override
    { if (read_cb) { read_cb(code, buffer, size); } }
    virtual void onClose() override
    { if (close_cb) { close_cb(); } }
    // @formatter:on
};

} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_CLIENT_HPP__

