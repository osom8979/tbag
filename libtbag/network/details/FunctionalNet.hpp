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

#include <libtbag/functional/CallbackHelper.hpp>
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
    using Parent = BaseType;
    using Loop   = uvpp::Loop;

    STATIC_ASSERT_CHECK_IS_BASE_OF(ClientInterface, Parent);

    FunctionalClient(Loop & loop) : Parent(loop)
    { /* EMPTY */ }
    virtual ~FunctionalClient()
    { /* EMPTY */ }

    TBAG_VOID_CALLBACK_HELPER(onConnect,  Err);
    TBAG_VOID_CALLBACK_HELPER(onShutdown, Err);
    TBAG_VOID_CALLBACK_HELPER(onWrite,    Err);
    TBAG_VOID_CALLBACK_HELPER(onRead,     Err, ReadPacket const &);

    TBAG_VOID_NOPARAM_CALLBACK_HELPER(onClose);
    TBAG_VOID_NOPARAM_CALLBACK_HELPER(onTimer);
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

    using SharedClient = ServerInterface::SharedClient;
    using   WeakClient = ServerInterface::WeakClient;

    STATIC_ASSERT_CHECK_IS_BASE_OF(ServerInterface, Parent);

    FunctionalServer(Loop & loop) : Parent(loop)
    { /* EMPTY */ }
    virtual ~FunctionalServer()
    { /* EMPTY */ }

    TBAG_VOID_CALLBACK_HELPER(onConnection,     Err);
    TBAG_VOID_CALLBACK_HELPER(onClientShutdown, WeakClient, Err);
    TBAG_VOID_CALLBACK_HELPER(onClientWrite,    WeakClient, Err);
    TBAG_VOID_CALLBACK_HELPER(onClientRead,     WeakClient, Err, ReadPacket const &);
    TBAG_VOID_CALLBACK_HELPER(onClientClose,    WeakClient);
    TBAG_VOID_NOPARAM_CALLBACK_HELPER(onServerClose);

    TBAG_CALLBACK_HELPER(onClientUdataAlloc, void*, nullptr, WeakClient);
    TBAG_VOID_CALLBACK_HELPER(onClientUdataDealloc, WeakClient, void*);
};

} // namespace details
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DETAILS_FUNCTIONALNET_HPP__

