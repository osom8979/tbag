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

#include <libtbag/id/Id.hpp>
#include <libtbag/uvpp/Tcp.hpp>
#include <libtbag/uvpp/Pipe.hpp>

#include <cstdint>
#include <type_traits>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace details {

TBAG_CONSTEXPR char const * const ANY_IPV4 = "0.0.0.0";
TBAG_CONSTEXPR char const * const BROADCAST_SUBNET_IPV4 = "255.255.255.255";

TBAG_CONSTEXPR char const * const ANY_SUBNET_MASK_IPV4 = "0.0.0.0";

TBAG_CONSTEXPR char const * const LOOPBACK_NAME = "localhost";
TBAG_CONSTEXPR char const * const LOOPBACK_IPV4 = "127.0.0.1";
TBAG_CONSTEXPR char const * const LOOPBACK_IPV6 = "::1";

TBAG_CONSTEXPR int const OS_RANDOMLY_ASSIGNS_PORT = 0;

enum class StreamType : int
{
    UNKNOWN,
    TCP,
    PIPE,
};

template <typename T> struct IsNetworkType : public std::false_type
{
    TBAG_CONSTEXPR static StreamType const STREAM_TYPE = StreamType::UNKNOWN;
};

template <> struct IsNetworkType<uvpp::Tcp> : public std::true_type
{
    TBAG_CONSTEXPR static StreamType const STREAM_TYPE = StreamType::TCP;
};

template <> struct IsNetworkType<uvpp::Pipe> : public std::true_type
{
    TBAG_CONSTEXPR static StreamType const STREAM_TYPE = StreamType::PIPE;
};

TBAG_API bool isIpv4(std::string const & ip);
TBAG_API bool isIpv6(std::string const & ip);

/**
 * Client interface.
 *
 * @author zer0
 * @date   2017-05-02
 * @date   2017-05-23 (Rename: Client -> ClientInterface)
 */
struct ClientInterface
{
    using binf = uvpp::binf;
    using Id   = id::Id;

    virtual Id getId() const = 0;

    virtual bool init(char const * destination, int port = 0, uint64_t millisec = 0) = 0;

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
 * Server interface.
 *
 * @author zer0
 * @date   2017-05-02
 * @date   2017-05-23 (Rename: Server -> ServerInterface)
 */
struct ServerInterface
{
    using Id = id::Id;

    using SharedClient = std::shared_ptr<ClientInterface>;
    using   WeakClient =   std::weak_ptr<ClientInterface>;

    virtual bool init(char const * destination, int port = 0) = 0;
    virtual void close() = 0;

    virtual WeakClient accept() = 0;
    virtual WeakClient getClient(Id id) = 0;

    virtual char const * getDestination() const = 0;
    virtual int getPort() const = 0;

    virtual void runBackendConnection(Err code) = 0;
    virtual void runBackendClose() = 0;

    // ---------------
    // Event callback.
    // ---------------

    virtual void onConnection(Err code) { /* EMPTY. */ }
    virtual void onClientShutdown(WeakClient node, Err code) { /* EMPTY. */ }
    virtual void onClientWrite(WeakClient node, Err code) { /* EMPTY. */ }
    virtual void onClientRead(WeakClient node, Err code, char const * buffer, std::size_t size) { /* EMPTY. */ }
    virtual void onClientClose(WeakClient node) { /* EMPTY. */ }
    virtual void onServerClose() { /* EMPTY. */ }

    virtual void * onClientUserDataAlloc(WeakClient node) { return nullptr; }
    virtual void onClientUserDataDealloc(WeakClient node, void * data) { /* EMPTY. */ }
};

} // namespace details
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DETAILS_NETCOMMON_HPP__

