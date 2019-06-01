/**
 * @file   NngSocket.hpp
 * @brief  NngSocket class prototype.
 * @author zer0
 * @date   2019-05-29
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MQ_NNGSOCKET_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MQ_NNGSOCKET_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/mq/NngBypass.hpp>

#include <string>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace mq {

/**
 * NngSocket class prototype.
 *
 * @author zer0
 * @date   2019-05-29
 */
class TBAG_API NngSocket
{
public:
    enum class SocketType
    {
        ST_NONE = 0,
        ST_BUS0,
        ST_PAIR0,
        ST_PAIR1,
        ST_PULL0,
        ST_PUSH0,
        ST_PUB0,
        ST_SUB0,
        ST_REP0,
        ST_REQ0,
        ST_RESPONDENT0,
        ST_SURVEYOR0,
    };

public:
    static char const * const getSocketTypeName(SocketType type) TBAG_NOEXCEPT;

public:
    TBAG_CONSTEXPR static const nng_duration DURATION_INFINITE = NNG_DURATION_INFINITE;
    TBAG_CONSTEXPR static const nng_duration DURATION_DEFAULT  = NNG_DURATION_DEFAULT;
    TBAG_CONSTEXPR static const nng_duration DURATION_ZERO     = NNG_DURATION_ZERO;

    TBAG_CONSTEXPR static int const SOCKNAME_MAX_LENGTH = 64; // including the terminating NULL byte.

public:
    struct Impl;
    using SharedImpl = std::shared_ptr<Impl>;

private:
    SharedImpl _impl;

public:
    NngSocket();
    NngSocket(NngSocket const & obj) TBAG_NOEXCEPT;
    NngSocket(NngSocket && obj) TBAG_NOEXCEPT;
    ~NngSocket();

public:
    NngSocket & operator =(NngSocket const & obj) TBAG_NOEXCEPT;
    NngSocket & operator =(NngSocket && obj) TBAG_NOEXCEPT;

public:
    void copy(NngSocket const & obj) TBAG_NOEXCEPT;
    void swap(NngSocket & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(NngSocket & lh, NngSocket & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }

public:
    inline bool exists() const TBAG_NOEXCEPT
    { return static_cast<bool>(_impl); }

    inline operator bool() const TBAG_NOEXCEPT
    { return exists(); }

public:
    inline Impl       * get()       TBAG_NOEXCEPT { return _impl.get(); }
    inline Impl const * get() const TBAG_NOEXCEPT { return _impl.get(); }

    inline Impl       * operator ->()       TBAG_NOEXCEPT { return get(); }
    inline Impl const * operator ->() const TBAG_NOEXCEPT { return get(); }

    inline Impl       & ref()       TBAG_NOEXCEPT { return *get(); }
    inline Impl const & ref() const TBAG_NOEXCEPT { return *get(); }

    inline Impl       & operator *()       TBAG_NOEXCEPT { return ref(); }
    inline Impl const & operator *() const TBAG_NOEXCEPT { return ref(); }

public:
    void reset();

public:
    Err open(SocketType type, bool raw = false);
    Err close();

public:
    SocketType getType() const;
    char const * const getTypeName() const;

public:
    bool isOpened() const;

public:
    nng_socket getSocket() const;

public:
    Err listen(std::string const & url, nng_listener * lp = nullptr, int flags = 0);
    Err dial(std::string const & url, nng_dialer * lp = nullptr, int flags = 0);

public:
    Err send(void * data, size_t size, int flags = 0);
    Err recv(void * data, size_t * size, int flags = 0);

public:
    Err setopt(std::string const & key, bool value);
    Err setopt(std::string const & key, int value);
    Err setopt(std::string const & key, size_t value);
    Err setopt(std::string const & key, std::string const & value);
    Err setopt(std::string const & key, void * value);

    Err getopt(std::string const & key, bool * value) const;
    Err getopt(std::string const & key, int * value) const;
    Err getopt(std::string const & key, size_t * value) const;
    Err getopt(std::string const & key, std::string & value) const;
    Err getopt(std::string const & key, void ** value) const;

    Err setopt_uint64(std::string const & key, uint64_t value);
    Err getopt_uint64(std::string const & key, uint64_t * value) const;

    Err setopt_duration(std::string const & key, nng_duration value = DURATION_DEFAULT);
    Err getopt_duration(std::string const & key, nng_duration * value) const;

public:
    // Socket options:
    // https://nanomsg.github.io/nng/man/tip/nng_options.5.html

    /**
     * This is the socket receive timeout in milliseconds.
     * When no message is available for receiving at the socket for this period of time,
     * receive operations will fail with a return value of NNG_ETIMEDOUT.
     */
    Err setRecvTimeout(nng_duration ms);
    Err getRecvTimeout(nng_duration * ms) const;

    /**
     * This is the socket send timeout in milliseconds.
     * When a message cannot be queued for delivery by the socket for this period of time
     * (such as if send buffers are full),
     * the operation will fail with a return value of NNG_ETIMEDOUT.
     */
    Err setSendTimeout(nng_duration ms);
    Err getSendTimeout(nng_duration * ms) const;

    /**
     * This is the depth of the socket’s receive buffer as a number of messages.
     * Messages received by a transport may be buffered until the application has accepted them for delivery.
     */
    Err setRecvNumberOfMessages(int size);
    Err getRecvNumberOfMessages(int * size) const;

    /**
     * This is the depth of the socket send buffer as a number of messages.
     * Messages sent by an application may be buffered by the socket
     * until a transport is ready to accept them for delivery.
     * This value must be an integer between 0 and 8192, inclusive.
     */
    Err setSendNumberOfMessages(int size);
    Err getSendNumberOfMessages(int * size) const;

    /**
     * This is the maximum message size that the will be accepted from a remote peer.
     * If a peer attempts to send a message larger than this, then the message will be discarded.
     * If the value of this is zero, then no limit on message sizes is enforced.
     * This option exists to prevent certain kinds of denial-of-service attacks,
     * where a malicious agent can claim to want to send an extraordinarily large message,
     * without sending any data. This option can be set for the socket,
     * but may be overridden for on a per-dialer or per-listener basis.
     */
    Err setRecvMaxSize(std::size_t size);
    Err getRecvMaxSize(std::size_t * size) const;

    /**
     * This is the minimum amount of time (milliseconds) to wait before attempting to establish
     * a connection after a previous attempt has failed.
     * This can be set on a socket, but it can also be overridden on an individual dialer.
     * The option is irrelevant for listeners.
     */
    Err setReconnectTimeMin(nng_duration ms);
    Err getReconnectTimeMin(nng_duration * ms) const;

    /**
     * This is the maximum amount of time (milliseconds) to wait before attempting to establish
     * a connection after a previous attempt has failed.
     * If this is non-zero, then the time between successive connection attempts will start at
     * the value of NNG_OPT_RECONNMINT, and grow exponentially, until it reaches this value.
     * If this value is zero, then no exponential back-off between connection attempts is done,
     * and each attempt will wait the time specified by NNG_OPT_RECONNMINT.
     * This can be set on a socket, but it can also be overridden on an individual dialer.
     * The option is irrelevant for listeners.
     */
    Err setReconnectTimeMax(nng_duration ms);
    Err getReconnectTimeMax(nng_duration * ms) const;

    /**
     * This the socket name.
     * By default this is a string corresponding to the value of the socket.
     * The string must fit within 64-bytes, including the terminating NUL byte.
     * The value is intended for application use, and is not used for anything in the library itself.
     */
    Err setSocketName(std::string const & name);
    Err getSocketName(std::string & name) const;

    /**
     * This is the maximum number of "hops" a message may traverse across a nng_device() forwarders.
     * The intention here is to prevent forwarding loops in device chains.
     * When this is supported, it can have a value between 1 and 255, inclusive.
     */
    Err setMaxTTL(int size);
    Err getMaxTTL(int * size) const;

    /**
     * This read-only option indicates whether the socket is in "raw" mode.
     * If true, the socket is in "raw" mode, and if false the socket is in "cooked" mode.
     *
     * Raw mode sockets generally do not have any protocol-specific semantics applied to them;
     * instead the application is expected to perform such semantics itself.
     *
     * (For example, in "cooked" mode a rep socket would automatically copy message headers
     * from a received message to the corresponding reply, whereas in "raw" mode this is not done.)
     *
     * @see <https://nanomsg.github.io/nng/man/tip/nng.7#raw_mode>
     */
    Err getRaw(bool * mode) const;

    /**
     * This read-only option is used to obtain the URL with which a listener or dialer was configured.
     * Accordingly it can only be used with dialers, listeners, and pipes.
     */
    Err getUrl(std::string & url) const;

    /**
     * This read-only option is used to obtain the 16-bit number for the socket’s protocol.
     */
    Err getProtocolNumber(int * number) const;

    /**
     * This read-only option is used to obtain the 16-bit number of the peer protocol for the socket.
     */
    Err getPeerProtocolNumber(int * number) const;

    /**
     * This read-only option is used to obtain the name of the socket’s protocol.
     */
    Err getProtocolName(std::string & name) const;

    /**
     * This read-only option is used to obtain the name of the peer protocol for the socket.
     */
    Err getPeerProtocolName(std::string & name) const;
};

} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MQ_NNGSOCKET_HPP__

