/**
 * @file   NetInterface.hpp
 * @brief  NetInterface class prototype.
 * @author zer0
 * @date   2018-12-09
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DETAILS_NETINTERFACE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DETAILS_NETINTERFACE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>

#include <libtbag/id/Id.hpp>
#include <libtbag/uvpp/UvCommon.hpp>
#include <libtbag/uvpp/Tcp.hpp>
#include <libtbag/uvpp/Pipe.hpp>

#include <cstdint>
#include <string>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace details {

/** Maximum continuous failure count of write. */
TBAG_CONSTEXPR int const MAXIMUM_CONTINUOUS_FAILURE_COUNT_OF_WRITE = 8;

/** Maximum write queue size. */
TBAG_CONSTEXPR int const MAXIMUM_WRITE_QUEUE_SIZE = 256;

/** Maximum connection size. */
TBAG_CONSTEXPR int const MAXIMUM_CONNECTION_SIZE = 10000;

enum class StreamType : int
{
    UNKNOWN,
    TCP,
    PIPE,
};

inline char const * const getStreamName(StreamType type) TBAG_NOEXCEPT
{
    // @formatter:off
    switch (type) {
    case StreamType::TCP:     return "TCP";
    case StreamType::PIPE:    return "PIPE";
    case StreamType::UNKNOWN: return "UNKNOWN";
    default:                  return "UNKNOWN";
    }
    // @formatter:on
}

template <typename T> struct IsNetworkType : public std::false_type
{
    TBAG_CONSTEXPR static StreamType const STREAM_TYPE = StreamType::UNKNOWN;
};

template <> struct IsNetworkType<libtbag::uvpp::Tcp> : public std::true_type
{
    TBAG_CONSTEXPR static StreamType const STREAM_TYPE = StreamType::TCP;
};

template <> struct IsNetworkType<libtbag::uvpp::Pipe> : public std::true_type
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

/**
 * Client interface.
 *
 * @author zer0
 * @date   2017-05-02
 * @date   2017-05-23 (Rename: Client -> ClientInterface)
 */
struct ClientInterface
{
    using Id   = libtbag::id::Id;
    using binf = libtbag::uvpp::binf;

    ClientInterface() { /* EMPTY.*/ }
    virtual ~ClientInterface() { /* EMPTY.*/ }

    virtual Id          id   () const { return reinterpret_cast<Id>(this); }
    virtual std::string dest () const { return std::string(); }
    virtual int         port () const { return 0; }

    virtual void * udata(void * data = nullptr) { return nullptr; }

    // -------------
    // Pure virtual.
    // -------------

    virtual Err init  (char const * destination, int port = 0) = 0;
    virtual Err start () = 0;
    virtual Err stop  () = 0;
    virtual Err close () = 0;
    virtual Err cancel() = 0;
    virtual Err write (char const * buffer, std::size_t size) = 0;

    // ----------------
    // Timer utilities.
    // ----------------

    virtual void setWriteTimeout(uint64_t millisec) { /* EMPTY. */ }
    virtual bool isActiveTimer() { return false; }
    virtual Err startTimer(uint64_t millisec) { return Err::E_UNSUPOP; }
    virtual void stopTimer() { /* EMPTY. */ }

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
    virtual void onTimer   () { /* EMPTY. */ }
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
    using Id = libtbag::id::Id;

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
    virtual Err        close () = 0;
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
    virtual void onClientTimer   (WeakClient node) { /* EMPTY. */ }
    virtual void onServerClose   () { /* EMPTY. */ }
};

} // namespace details
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DETAILS_NETINTERFACE_HPP__
