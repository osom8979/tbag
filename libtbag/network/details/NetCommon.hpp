/**
 * @file   NetCommon.hpp
 * @brief  NetCommon class prototype.
 * @author zer0
 * @date   2017-05-03
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DETAILS_NETCOMMON_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DETAILS_NETCOMMON_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/uvpp/UvCommon.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace details {

enum class NetType
{
    TCP,
    UDP,
    PIPE,
    POLL,
};

struct NetInterface
{
    using uerr = uvpp::uerr;
    using binf = uvpp::binf;

    using Type = NetType;
    using Size = std::size_t;

    // ACCESSOR.
    virtual Type getType() const = 0;

    // OPERATORS.
    virtual uerr start() = 0;
    virtual uerr  stop() = 0;
    virtual uerr close() = 0;
};

struct ClientInterface : public NetInterface
{
    // OPERATORS.
    virtual uerr  syncWrite(char const * buffer, Size size, Size * wsize = nullptr) = 0;
    virtual uerr asyncWrite(char const * buffer, Size size, Size * wsize = nullptr) = 0;
    virtual uerr   tryWrite(char const * buffer, Size size, Size * wsize = nullptr) = 0;

    // EVENT METHODS.
    virtual void onConnect(uerr code) = 0;
    virtual binf onAlloc  (Size suggested_size) = 0;
    virtual void onWrite  (uerr code) = 0;
    virtual void onRead   (uerr code, char const * buffer, Size size) = 0;
    virtual void onClose  () = 0;
    virtual void onAsync  (int type, uerr code) = 0;
};

struct ServerInterface : public NetInterface
{
    using ClientInterface = details::ClientInterface;

    // EVENT METHODS.
    virtual void onClose() = 0;

    virtual void onClientConnect(ClientInterface * client, uerr code) = 0;
    virtual binf onClientAlloc  (ClientInterface * client, Size suggested_size) = 0;
    virtual void onClientWrite  (ClientInterface * client, uerr code) = 0;
    virtual void onClientRead   (ClientInterface * client, uerr code, char const * buffer, Size size) = 0;
    virtual void onClientClose  (ClientInterface * client) = 0;
    virtual void onClientAsync  (ClientInterface * client, int type, uerr code) = 0;
};

} // namespace details
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DETAILS_NETCOMMON_HPP__

