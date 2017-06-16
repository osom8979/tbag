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

enum class PacketType
{
    PT_STREAM,
    PT_DATAGRAM,
};

/**
 * Read event parameter.
 *
 * @author zer0
 * @date   2017-06-16
 */
struct ReadPacket
{
    PacketType        type;
    char const *      buffer;
    std::size_t       size;
    sockaddr const *  addr;
    unsigned int      flags;

    explicit ReadPacket(PacketType t, char const * b, std::size_t s, sockaddr const * a, unsigned int f)
            : type(t), buffer(b), size(s), addr(a), flags(f)
    { /* EMPTY. */ }
    explicit ReadPacket(char const * b, std::size_t s, sockaddr const * a, unsigned int f)
            : ReadPacket(PacketType::PT_DATAGRAM, b, s, a, f)
    { /* EMPTY. */ }
    explicit ReadPacket(char const * b, std::size_t s)
            : ReadPacket(PacketType::PT_STREAM, b, s, nullptr, 0)
    { /* EMPTY. */ }

    virtual ~ReadPacket()
    { /* EMPTY. */ }
};

TBAG_API bool isIpv4(std::string const & ip);
TBAG_API bool isIpv6(std::string const & ip);

// ------------------------
// Miscellaneous utilities.
// ------------------------

inline bool isWellKnownPort(int port) TBAG_NOEXCEPT
{
    return 0 <= COMPARE_AND(port) <= 1023;
}

inline bool isRegisteredPort(int port) TBAG_NOEXCEPT
{
    return 1024 <= COMPARE_AND(port) <= 49151;
}

inline bool isDynamicPort(int port) TBAG_NOEXCEPT
{
    return 49152 <= COMPARE_AND(port) <= 65535;
}

// ----------------
// Interface class.
// ----------------

/**
 * Client interface.
 *
 * @author zer0
 * @date   2017-05-02
 * @date   2017-05-23 (Rename: Client -> ClientInterface)
 */
struct ClientInterface
{
    using Id   = id::Id;
    using binf = uvpp::binf;

    ClientInterface() { /* EMPTY.*/ }
    virtual ~ClientInterface() { /* EMPTY.*/ }

    virtual Id          id   () const { return reinterpret_cast<Id>(this); }
    virtual std::string dest () const { return std::string(); }
    virtual int         port () const { return 0; }
    virtual void *      udata() { return nullptr; }

    // -------------
    // Pure virtual.
    // -------------

    virtual Err  init  (char const * destination, int port = 0, uint64_t millisec = 0U) = 0;
    virtual Err  start () = 0;
    virtual Err  stop  () = 0;
    virtual void close () = 0;
    virtual void cancel() = 0;
    virtual Err  write (binf const * buffer, std::size_t size, uint64_t millisec = 0U) = 0;
    virtual Err  write (char const * buffer, std::size_t size, uint64_t millisec = 0U) = 0;

    // ---------------
    // Backend helper.
    // ---------------

    virtual void backConnect (Err code) = 0;
    virtual void backShutdown(Err code) = 0;
    virtual void backWrite   (Err code) = 0;
    virtual void backRead    (Err code, ReadPacket const & packet) = 0;
    virtual void backClose   () = 0;

    // ---------------
    // Event callback.
    // ---------------

    virtual void onConnect (Err code) { /* EMPTY. */ }
    virtual void onShutdown(Err code) { /* EMPTY. */ }
    virtual void onWrite   (Err code) { /* EMPTY. */ }
    virtual void onRead    (Err code, ReadPacket const & packet) { /* EMPTY. */ }
    virtual void onClose   () { /* EMPTY. */ }
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

    ServerInterface() { /* EMPTY.*/ }
    virtual ~ServerInterface() { /* EMPTY.*/ }

    virtual std::string dest () const { return std::string(); }
    virtual int         port () const { return 0; }

    // -------------
    // Pure virtual.
    // -------------

    virtual Err        init  (char const * destination, int port = 0) = 0;
    virtual void       close () = 0;
    virtual WeakClient accept() = 0;
    virtual WeakClient get   (Id id) = 0;
    virtual Err        remove(Id id) = 0;

    // ---------------
    // Backend helper.
    // ---------------

    virtual void backConnection(Err code) = 0;
    virtual void backClose() = 0;

    // ---------------
    // Event callback.
    // ---------------

    virtual void onConnection    (Err code) { /* EMPTY. */ }
    virtual void onClientShutdown(WeakClient node, Err code) { /* EMPTY. */ }
    virtual void onClientWrite   (WeakClient node, Err code) { /* EMPTY. */ }
    virtual void onClientRead    (WeakClient node, Err code, ReadPacket const & packet) { /* EMPTY. */ }
    virtual void onClientClose   (WeakClient node) { /* EMPTY. */ }
    virtual void onClose         () { /* EMPTY. */ }

    virtual void * onClientUdataAlloc  (WeakClient node) { return nullptr; }
    virtual void   onClientUdataDealloc(WeakClient node, void * data) { /* EMPTY. */ }
};

} // namespace details
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DETAILS_NETCOMMON_HPP__

