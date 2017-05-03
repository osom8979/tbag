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
#include <libtbag/Noncopyable.hpp>
#include <libtbag/network/details/NetCommon.hpp>

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
class TBAG_API Client : public details::ClientInterface, public Noncopyable
{
public:
    using Loop    = uvpp::Loop;
    using NetType = details::NetType;

    using SharedClient = std::shared_ptr<Client>;
    using WeakClient   = std::weak_ptr<Client>;

protected:
    Client(Loop & loop);
public:
    virtual ~Client();

public:
    // @formatter:off
    virtual uerr start() override { return uerr::UVPP_ENOSYS; }
    virtual uerr  stop() override { return uerr::UVPP_ENOSYS; }
    virtual uerr close() override { return uerr::UVPP_ENOSYS; }
    // @formatter:on

public:
    // @formatter:off
    virtual uerr  syncWrite(char const * buffer, Size size, Size * wsize = nullptr) override
    { return uerr::UVPP_ENOSYS; }
    virtual uerr asyncWrite(char const * buffer, Size size, Size * wsize = nullptr) override
    { return uerr::UVPP_ENOSYS; }
    virtual uerr   tryWrite(char const * buffer, Size size, Size * wsize = nullptr) override
    { return uerr::UVPP_ENOSYS; }
    virtual void onConnect(uerr code) override
    { /* EMPTY. */ }
    virtual binf onAlloc(Size suggested_size) override
    { return binf(); }
    virtual void onWrite(uerr code) override
    { /* EMPTY. */ }
    virtual void onRead(uerr code, char const * buffer, Size size) override
    { /* EMPTY. */ }
    virtual void onClose() override
    { /* EMPTY. */ }
    virtual void onAsync(int type, uerr code) override
    { /* EMPTY. */ }
    // @formatter:on

public:
    static SharedClient create(Loop & loop, NetType type);
};

} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_CLIENT_HPP__

