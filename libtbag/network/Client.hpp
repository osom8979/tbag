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
#include <libtbag/network/details/NetCommon.hpp>
#include <libtbag/Type.hpp>

#include <functional>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

// Forward declaration.
namespace uvpp { class Loop; }

namespace network {

/**
 * Client class prototype.
 *
 * @author zer0
 * @date   2017-05-02
 */
struct Client : public details::NetCommon
{
    // @formatter:off
    virtual Type getType() const
    { return Type::UNKNOWN; }
    virtual Id getId() const
    { return id::UNKNOWN_ID; }

    virtual bool init(String const & destination, int port = 0, uint64_t millisec = 0)
    { return false; }

    virtual bool  start() { return false; }
    virtual bool   stop() { return false; }
    virtual void  close() { /* EMPTY. */  }
    virtual void cancel() { /* EMPTY. */  }

    virtual bool write(binf const * buffer, Size size, uint64_t millisec = 0) { return false; }
    virtual bool write(char const * buffer, Size size, uint64_t millisec = 0) { return false; }

    virtual void onConnect (uerr code) { /* EMPTY. */ }
    virtual void onShutdown(uerr code) { /* EMPTY. */ }
    virtual void onWrite   (uerr code) { /* EMPTY. */ }
    virtual void onRead    (uerr code, char const * buffer, Size size) { /* EMPTY. */ }
    virtual void onClose   ()          { /* EMPTY. */ }

    virtual void * onUserDataAlloc  ()            { return nullptr; }
    virtual void   onUserDataDealloc(void * data) { /* EMPTY. */    }
    // @formatter:on
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

    STATIC_ASSERT_CHECK_IS_BASE_OF(Client, Parent);

    using Loop = uvpp::Loop;
    using uerr = details::NetCommon::uerr;
    using Size = details::NetCommon::Size;

    using OnConnect  = std::function<void(uerr)>;
    using OnShutdown = std::function<void(uerr)>;
    using OnWrite    = std::function<void(uerr)>;
    using OnRead     = std::function<void(uerr, char const *, Size)>;
    using OnClose    = std::function<void(void)>;

    using OnUserDataAlloc   = std::function<void*(void)>;
    using OnUserDataDealloc = std::function<void(void*)>;

    OnConnect  connect_cb;
    OnShutdown shutdown_cb;
    OnWrite    write_cb;
    OnRead     read_cb;
    OnClose    close_cb;

    OnUserDataAlloc   userdata_alloc_cb;
    OnUserDataDealloc userdata_dealloc_cb;

    FunctionalClient(Loop & loop) : Parent(loop)
    { /* EMPTY */ }
    virtual ~FunctionalClient()
    { /* EMPTY */ }

    inline void setOnConnect (OnConnect  const & cb) { connect_cb  = cb; }
    inline void setOnShutdown(OnShutdown const & cb) { shutdown_cb = cb; }
    inline void setOnWrite   (OnWrite    const & cb) { write_cb    = cb; }
    inline void setOnRead    (OnRead     const & cb) { read_cb     = cb; }
    inline void setOnClose   (OnClose    const & cb) { close_cb    = cb; }

    inline void setOnUserDataAlloc  (OnUserDataAlloc   const & cb) { userdata_alloc_cb   = cb; }
    inline void setOnUserDataDealloc(OnUserDataDealloc const & cb) { userdata_dealloc_cb = cb; }

    virtual void onConnect(uerr code) override
    { if (connect_cb) { connect_cb(code); } }
    virtual void onShutdown(uerr code) override
    { if (shutdown_cb) { shutdown_cb(code); } }
    virtual void onWrite(uerr code) override
    { if (write_cb) { write_cb(code); } }
    virtual void onRead(uerr code, char const * buffer, Size size) override
    { if (read_cb) { read_cb(code, buffer, size); } }
    virtual void onClose() override
    { if (close_cb) { close_cb(); } }

    virtual void * onUserDataAlloc() override
    { if (userdata_alloc_cb) { return userdata_alloc_cb(); } return nullptr; }
    virtual void onUserDataDealloc(void * data) override
    { if (userdata_dealloc_cb) { userdata_dealloc_cb(data); } }
    // @formatter:on
};

} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_CLIENT_HPP__

