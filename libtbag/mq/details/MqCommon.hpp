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
#include <libtbag/container/Box.hpp>
#include <libtbag/util/BufferInfo.hpp>

#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/uvpp/Tcp.hpp>
#include <libtbag/uvpp/Pipe.hpp>
#include <libtbag/uvpp/Udp.hpp>
#include <libtbag/uvpp/Request.hpp>

#include <cassert>
#include <cstdint>
#include <cstdlib>

#include <string>
#include <vector>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace mq      {
namespace details {

enum class MqEvent : int
{
    ME_NONE,
    ME_MSG,
    ME_SHUTDOWN,
    ME_CLOSE,
};

inline bool isMqMessageType(MqEvent type) TBAG_NOEXCEPT
{
    return type == MqEvent::ME_MSG /*|| type == MqEvent::ME_BOX_MSG*/;
}

enum class MqType : int
{
    MT_PIPE,
    MT_UDP,
    MT_TCP,
};

struct pipe_t { /* EMPTY. */ };
struct udp_t  { /* EMPTY. */ };
struct tcp_t  { /* EMPTY. */ };

/**
 * MessageQueue data packet.
 *
 * @author zer0
 * @date   2018-11-13
 */
struct MqMsg
{
    using Box          = libtbag::container::Box;
    using BoxTypeTable = libtbag::container::BoxTypeTable;

    MqEvent event;
    int     channel;
    Box     box;

    MqMsg(std::size_t size) : event(MqEvent::ME_NONE), channel(0),
                              box(BoxTypeTable::BTT_INT8, static_cast<unsigned>(size))
    { /* EMPTY. */ }

    ~MqMsg()
    { /* EMPTY. */ }
};

/**
 * Message copy from.
 *
 * @author zer0
 * @date   2018-11-13
 */
struct TBAG_API MqMsgCopyFrom
{
    MqEvent      event;
    char const * data;
    std::size_t  size;

    MqMsgCopyFrom(MqEvent e);
    MqMsgCopyFrom(char const * d, std::size_t s);
    MqMsgCopyFrom(MqEvent e, char const * d, std::size_t s);
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
    MqEvent     * event;
    char        * data;
    std::size_t    max;
    std::size_t * size;

    MqMsgCopyTo(MqEvent * e, char * d, std::size_t m, std::size_t * s);
    ~MqMsgCopyTo();

    bool operator()(MqMsg * msg);
};

struct MqCommonTypes
{
    using Loop   = libtbag::uvpp::Loop;
    using Stream = libtbag::uvpp::Stream;
    using Tcp    = libtbag::uvpp::Tcp;
    using Udp    = libtbag::uvpp::Udp;
    using Pipe   = libtbag::uvpp::Pipe;

    using ConnectRequest  = libtbag::uvpp::ConnectRequest;
    using ShutdownRequest = libtbag::uvpp::ShutdownRequest;
    using WriteRequest    = libtbag::uvpp::WriteRequest;
    using UdpSendRequest  = libtbag::uvpp::UdpSendRequest;

    using NativeHandle = Loop::NativeHandle;
    using SharedHandle = Loop::SharedHandle;
    using WeakHandle   = Loop::WeakHandle;

    using  binf = libtbag::util::binf;
    using cbinf = libtbag::util::cbinf;

    using pipe_t = libtbag::mq::details::pipe_t;
    using udp_t  = libtbag::mq::details::udp_t;
    using tcp_t  = libtbag::mq::details::tcp_t;
};

struct MqInterface : public MqCommonTypes
{
    virtual Err send(char const * buffer, std::size_t size) = 0;
    virtual Err recv(std::vector<char> & buffer) = 0;
};

} // namespace details
} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MQ_DETAILS_MQCOMMON_HPP__

