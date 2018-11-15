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

enum class MqType : int
{
    MT_NONE,
    MT_PIPE,
    MT_UDP,
    MT_TCP,
};

TBAG_CONSTEXPR char const * const PIPE_LOWER_NAME = "pipe";
TBAG_CONSTEXPR char const * const  UDP_LOWER_NAME = "udp";
TBAG_CONSTEXPR char const * const  TCP_LOWER_NAME = "tcp";

enum class MqMode : int
{
    MM_BIND,
    MM_CONNECT,
};

struct pipe_t { /* EMPTY. */ };
struct udp_t  { /* EMPTY. */ };
struct tcp_t  { /* EMPTY. */ };

struct bind_t    { /* EMPTY. */ };
struct connect_t { /* EMPTY. */ };

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
    virtual Err send(MqMsg const & msg) = 0;
    virtual Err recv(MqMsg & msg) = 0;

    virtual void recvWait(MqMsg & msg) = 0;
};

} // namespace details
} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MQ_DETAILS_MQCOMMON_HPP__

