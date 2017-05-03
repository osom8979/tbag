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
 * Server class prototype.
 *
 * @author zer0
 * @date   2017-05-02
 */
class TBAG_API Server : public details::ServerInterface, public Noncopyable
{
public:
    using Loop    = uvpp::Loop;
    using NetType = details::NetType;

    using SharedServer = std::shared_ptr<Server>;
    using WeakServer   = std::weak_ptr<Server>;

protected:
    Server(Loop & loop);
public:
    virtual ~Server();

public:
    // @formatter:off
    virtual uerr start() override { return uerr::UVPP_ENOSYS; }
    virtual uerr  stop() override { return uerr::UVPP_ENOSYS; }
    virtual uerr close() override { return uerr::UVPP_ENOSYS; }
    // @formatter:on

public:
    // @formatter:off
    virtual void onClose() override
    { /* EMPTY. */ }
    virtual void onClientConnect(ClientInterface * client, uerr code) override
    { /* EMPTY. */ }
    virtual binf onClientAlloc(ClientInterface * client, Size suggested_size) override
    { return binf(); }
    virtual void onClientWrite(ClientInterface * client, uerr code) override
    { /* EMPTY. */ }
    virtual void onClientRead(ClientInterface * client, uerr code, char const * buffer, Size size) override
    { /* EMPTY. */ }
    virtual void onClientClose(ClientInterface * client) override
    { /* EMPTY. */ }
    virtual void onClientAsync(ClientInterface * client, int type, uerr code) override
    { /* EMPTY. */ }
    // @formatter:on

public:
    static SharedServer create(Loop & loop, NetType type);
};

} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_SERVER_HPP__

