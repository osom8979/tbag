/**
 * @file   MqCommon.hpp
 * @brief  MqCommon class prototype.
 * @author zer0
 * @date   2018-11-10
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MQ_DETAILS_MQCOMMON_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MQ_DETAILS_MQCOMMON_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/util/BufferInfo.hpp>

#include <cassert>
#include <cstdint>
#include <cstdlib>

#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace mq      {
namespace details {

enum class MqEvent : int32_t
{
    ME_NONE,
    ME_MSG,
    ME_CLOSE,
};

inline char const * const getEventName(MqEvent event) TBAG_NOEXCEPT
{
    switch (event) {
    case MqEvent::ME_NONE:  return "NONE";
    case MqEvent::ME_MSG:   return "MSG";
    case MqEvent::ME_CLOSE: return "CLOSE";
    default:                return "UNKNOWN";
    }
}

enum class MqType : int
{
    MT_NONE,
    MT_PIPE,
    MT_UDP,
    MT_TCP,
};

TBAG_CONSTEXPR char const * const PIPE_UPPER_NAME = "PIPE";
TBAG_CONSTEXPR char const * const  UDP_UPPER_NAME = "UDP";
TBAG_CONSTEXPR char const * const  TCP_UPPER_NAME = "TCP";

inline char const * const getTypeName(MqType type) TBAG_NOEXCEPT
{
    switch (type) {
    case MqType::MT_NONE: return "NONE";
    case MqType::MT_PIPE: return PIPE_UPPER_NAME;
    case MqType::MT_UDP:  return UDP_UPPER_NAME;
    case MqType::MT_TCP:  return TCP_UPPER_NAME;
    default:              return "UNKNOWN";
    }
}

enum class MqMode : int
{
    MM_NONE,
    MM_BIND,
    MM_CONNECT,
};

inline char const * const getModeName(MqMode mode) TBAG_NOEXCEPT
{
    switch (mode) {
    case MqMode::MM_NONE:    return "NONE";
    case MqMode::MM_BIND:    return "BIND";
    case MqMode::MM_CONNECT: return "CONNECT";
    default:                 return "UNKNOWN";
    }
}

enum class MqRequestState
{
    MRS_WAITING,
    MRS_ASYNC,
    MRS_REQUESTING,
};

inline char const * const getRequestStateName(MqRequestState state) TBAG_NOEXCEPT
{
    switch (state) {
    case MqRequestState::MRS_WAITING:    return "WAITING";
    case MqRequestState::MRS_ASYNC:      return "ASYNC";
    case MqRequestState::MRS_REQUESTING: return "REQUESTING";
    default:                             return "UNKNOWN";
    }
}

enum class MqMachineState
{
    MMS_OPENING,          ///< Construct begin.
    MMS_INITIALIZED,      ///< Construct done.
    MMS_CONNECTED,        ///< Socket connect.
    MMS_SHUTTING,         ///< Shutdown request.
    MMS_DELAY_SHUTTING,   ///< Delay the shutdown request.
    MMS_SHUTDOWN,         ///< onShutdown() done.
    MMS_CLOSED,           ///< onClose() done.
};

inline char const * const getMachineStateName(MqMachineState state) TBAG_NOEXCEPT
{
    switch (state) {
    case MqMachineState::MMS_OPENING:         return "OPENING";
    case MqMachineState::MMS_INITIALIZED:     return "INITIALIZED";
    case MqMachineState::MMS_CONNECTED:       return "CONNECTED";
    case MqMachineState::MMS_SHUTTING:        return "SHUTTING";
    case MqMachineState::MMS_DELAY_SHUTTING:  return "DELAY_SHUTTING";
    case MqMachineState::MMS_SHUTDOWN:        return "SHUTDOWN";
    case MqMachineState::MMS_CLOSED:          return "CLOSED";
    default:                                  return "UNKNOWN";
    }
}

/**
 * MessageQueue data packet.
 *
 * @author zer0
 * @date   2018-11-13
 */
struct MqMsg
{
    using Buffer = libtbag::util::Buffer;
    using Value  = Buffer::value_type;

    static_assert(sizeof(Buffer::value_type) == 1,
                  "The minimum unit of the buffer element must be 1 Byte.");

    MqEvent  event;
    Buffer   buffer;

    MqMsg() : event(MqEvent::ME_NONE), buffer()
    { /* EMPTY. */ }
    MqMsg(std::size_t s) : event(MqEvent::ME_NONE), buffer(s)
    { /* EMPTY. */ }
    MqMsg(MqEvent e) : event(e), buffer()
    { /* EMPTY. */ }
    MqMsg(MqEvent e, std::size_t s) : event(e), buffer(s)
    { /* EMPTY. */ }
    MqMsg(MqEvent e, Value const * d, std::size_t s) : event(e), buffer(d, d + s)
    { /* EMPTY. */ }
    MqMsg(Value const * d, std::size_t s) : event(MqEvent::ME_MSG), buffer(d, d + s)
    { /* EMPTY. */ }

    MqMsg(MqMsg const & obj) : event(obj.event), buffer(obj.buffer)
    { /* EMPTY. */ }
    MqMsg(MqMsg && obj) TBAG_NOEXCEPT : event(std::move(obj.event)), buffer(std::move(obj.buffer))
    { /* EMPTY. */ }

    ~MqMsg()
    { /* EMPTY. */ }

    MqMsg & operator =(MqMsg const & obj)
    {
        if (this != &obj) {
            event  = obj.event;
            buffer = obj.buffer;
        }
        return *this;
    }

    MqMsg & operator =(MqMsg && obj) TBAG_NOEXCEPT
    {
        if (this != &obj) {
            std::swap(event, obj.event);
            buffer.swap(obj.buffer);
        }
        return *this;
    }

    inline char * data() TBAG_NOEXCEPT_SP_OP(buffer.data())
    { return buffer.data(); }

    inline char const * data() const TBAG_NOEXCEPT_SP_OP(buffer.data())
    { return buffer.data(); }

    inline bool empty() const TBAG_NOEXCEPT_SP_OP(buffer.empty())
    { return buffer.empty(); }

    inline std::size_t size() const TBAG_NOEXCEPT_SP_OP(buffer.size())
    { return buffer.size(); }

    inline Buffer::iterator begin() TBAG_NOEXCEPT
    { return buffer.begin(); }
    inline Buffer::const_iterator begin() const TBAG_NOEXCEPT
    { return buffer.begin(); }

    inline Buffer::iterator end() TBAG_NOEXCEPT
    { return buffer.end(); }
    inline Buffer::const_iterator end() const TBAG_NOEXCEPT
    { return buffer.end(); }
};

/**
 * Message copy from.
 *
 * @author zer0
 * @date   2018-11-13
 */
struct TBAG_API MqMsgCopyFrom
{
    MqMsg const & source_msg;

    MqMsgCopyFrom(MqMsg const & msg);
    ~MqMsgCopyFrom();

    bool operator()(MqMsg * msg);
};

/**
 * Message copy to.
 *
 * @author zer0
 * @date   2018-11-13
 */
struct TBAG_API MqMsgCopyTo
{
    MqMsg & destination_msg;

    MqMsgCopyTo(MqMsg & msg);
    ~MqMsgCopyTo();

    bool operator()(MqMsg * msg);
};

struct MqInterface
{
    virtual MqMachineState state() const TBAG_NOEXCEPT = 0;

    virtual Err send(MqMsg const & msg) = 0;
    virtual Err recv(MqMsg & msg) = 0;

    virtual void recvWait(MqMsg & msg) = 0;
};

TBAG_CONSTEXPR std::size_t const TBAG_MQ_DEFAULT_QUEUE_SIZE       = 1024;            // power of 2.
TBAG_CONSTEXPR std::size_t const TBAG_MQ_DEFAULT_PACKET_SIZE      = 256;             // data buffer size.
TBAG_CONSTEXPR std::size_t const TBAG_MQ_DEFAULT_MAX_NODE_SIZE    = 100000;          // Number of C10K.
TBAG_CONSTEXPR std::size_t const TBAG_MQ_DEFAULT_BUILDER_SIZE     = 1 * 1024 * 1024; // FlatBuffers builder capacity.
TBAG_CONSTEXPR std::size_t const TBAG_MQ_DEFAULT_CLOSE_MILLISEC   = 1 * 1000;        // Shutdown -> Close wait-timeout.
TBAG_CONSTEXPR std::size_t const TBAG_MQ_DEFAULT_READ_ERROR_COUNT = 4;               // Max continuous read error count.

struct MqParams
{
    /**
     * Type of stream. must be TCP or PIPE.
     */
    MqType type = MqType::MT_TCP;

    /**
     * Bind or Connect address.
     *
     * @remarks
     *  - tcp: bind socket address.
     *  - pipe: pipe file path.
     */
    std::string address;

    /**
     * Bind port number.
     *
     * @remarks
     *  - tcp: port number.
     *  - pipe: unused.
     */
    int port = 0;

    /*
     * Used with uv_tcp_bind, when an IPv6 address is used.
     *
     * @remarks
     *  - tcp: use this flag.
     *  - pipe: unused.
     */
    bool tcp_ipv6_only = false;

    /**
     * The maximum size of the queue for transmission.
     */
    std::size_t send_queue_size = TBAG_MQ_DEFAULT_QUEUE_SIZE;

    /**
     * The default size of the transmission message packet.
     *
     * @remarks
     *  If memory is insufficient, it will be more expanded.
     */
    std::size_t send_msg_size = TBAG_MQ_DEFAULT_PACKET_SIZE;

    /**
     * The maximum size of the queue for receive
     */
    std::size_t recv_queue_size = TBAG_MQ_DEFAULT_QUEUE_SIZE;

    /**
     * The default size of the receive message packet.
     *
     * @remarks
     *  If memory is insufficient, it will be more expanded.
     */
    std::size_t recv_msg_size = TBAG_MQ_DEFAULT_PACKET_SIZE;

    /**
     * The number of clients that can be accepted.
     */
    std::size_t max_nodes = TBAG_MQ_DEFAULT_MAX_NODE_SIZE;

    /**
     * Temporary buffer size for serialization.
     */
    std::size_t packer_size = TBAG_MQ_DEFAULT_BUILDER_SIZE;

    /**
     * Wait time to closing. If this value is 0, close immediately.
     *
     * @remarks
     *  If you request a shutdown directly, You need time to wait for an idle recv request.
     */
    std::size_t wait_closing_millisec = TBAG_MQ_DEFAULT_CLOSE_MILLISEC;

    /**
     * Verify the restore message.
     */
    bool verify_restore_message = false;

    /**
     * If the consecutive read error is maximum,
     * the connection is forced to close.
     */
    std::size_t continuous_read_error_count = TBAG_MQ_DEFAULT_READ_ERROR_COUNT;

    /**
     * You are given the opportunity to filter IP addresses for acceptance.
     *
     * @remarks
     *  - tcp: use this value.
     *  - pipe: unused.
     */
    std::string accept_ip_regex;

    /**
     * Number of attempts to reconnect
     *
     * @remarks
     *  - Used in client only.
     */
    std::size_t try_reconnect_count = 0;

    /**
     * Wait until connection is completed.
     *
     * @remarks
     *  The server is not used.
     */
    std::size_t wait_on_connection_timeout_millisec = 1000;

    /**
     * Verbose log message.
     */
    bool verbose = false;

    /**
     * The wait time to not miss the send() requested by another thread.
     *
     * @remarks
     *  At least 10 nanoseconds is recommended.
     */
    std::size_t shutdown_wait_nanosec = 1000;

    MqParams()
    { /* EMPTY. */ }

    ~MqParams()
    { /* EMPTY. */ }
};

TBAG_CONSTEXPR static char const * const TCP_IPV6_ONLY_NAME   = "tcp_ipv6";
TBAG_CONSTEXPR static char const * const SEND_QUEUE_SIZE_NAME = "send_queue";
TBAG_CONSTEXPR static char const * const SEND_MSG_SIZE_NAME   = "send_msg";
TBAG_CONSTEXPR static char const * const RECV_QUEUE_SIZE_NAME = "recv_queue";
TBAG_CONSTEXPR static char const * const RECV_MSG_SIZE_NAME   = "recv_msg";
TBAG_CONSTEXPR static char const * const MAX_NODES_NAME       = "max_nodes";
TBAG_CONSTEXPR static char const * const PACKER_SIZE_NAME     = "packer_size";
TBAG_CONSTEXPR static char const * const WAIT_CLOSING_NAME    = "wait_closing";
TBAG_CONSTEXPR static char const * const VERIFY_MSG_NAME      = "verify_msg";
TBAG_CONSTEXPR static char const * const READ_ERROR_NAME      = "read_error";
TBAG_CONSTEXPR static char const * const TRY_RECONNECT_NAME   = "try_reconnect";
TBAG_CONSTEXPR static char const * const WAIT_CONNECTION_NAME = "wait_connection";
TBAG_CONSTEXPR static char const * const VERBOSE_NAME         = "verbose";

// -----------------------
// Miscellaneous utilities
// -----------------------

TBAG_API MqParams convertUriToParams(std::string const & uri_string, bool convert_windows_path = false);

} // namespace details
} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MQ_DETAILS_MQCOMMON_HPP__

