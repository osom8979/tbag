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
    using Buffer = libtbag::util::Buffer;

    static_assert(sizeof(Buffer::value_type) == 1,
                  "The minimum unit of the buffer element must be 1 Byte.");

    MqEvent event;
    Buffer  data;

    MqMsg() : event(MqEvent::ME_NONE), data()
    { /* EMPTY. */ }
    MqMsg(std::size_t s) : event(MqEvent::ME_NONE), data(s)
    { /* EMPTY. */ }
    MqMsg(MqEvent e, std::size_t s) : event(e), data(s)
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

struct MqInterface
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

