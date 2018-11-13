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
#include <libtbag/container/Box.hpp>

#include <cstdint>
#include <cstdlib>

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

struct MqMsg
{
    using Box          = libtbag::container::Box;
    using BoxTypeTable = libtbag::container::BoxTypeTable;

    MqEvent event;
    Box     box;

    MqMsg(std::size_t size) : event(MqEvent::ME_NONE),
                              box(BoxTypeTable::BTT_INT8, static_cast<unsigned>(size))
    { /* EMPTY. */ }

    ~MqMsg()
    { /* EMPTY. */ }
};

struct MqMsgCopyFrom
{
    MqEvent      event;
    char const * data;
    std::size_t  size;

    MqMsgCopyFrom(MqEvent e) : MqMsgCopyFrom(e, nullptr, 0)
    { /* EMPTY. */ }
    MqMsgCopyFrom(char const * d, std::size_t s) : MqMsgCopyFrom(MqEvent::ME_MSG, d, s)
    { /* EMPTY. */ }
    MqMsgCopyFrom(MqEvent e, char const * d, std::size_t s) : event(e), data(d), size(s)
    { /* EMPTY. */ }
    ~MqMsgCopyFrom()
    { /* EMPTY. */ }

    bool operator()(MqMsg * msg)
    {
        msg->event = event;
        if (data == nullptr || size == 0) {
            return true;
        }

        assert(size <= libtbag::type::TypeInfo<unsigned>::maximum());
        Err const RESIZE_CODE = msg->box.resize(static_cast<unsigned>(size));
        assert(isSuccess(RESIZE_CODE));
        Err const COPY_CODE = msg->box.copyFrom(data, size);
        assert(isSuccess(COPY_CODE));
        return true;
    }
};

struct MqMsgCopyTo
{
    MqEvent     * event;
    char        * data;
    std::size_t    max;
    std::size_t * size;

    MqMsgCopyTo(MqEvent * e, char * d, std::size_t m, std::size_t * s)
            : event(e), data(d), max(m), size(s)
    { /* EMPTY. */ }
    ~MqMsgCopyTo()
    { /* EMPTY. */ }

    bool operator()(MqMsg * msg)
    {
        if (event != nullptr) {
            *event = msg->event;
        }
        auto const BOX_SIZE = msg->box.size();
        if (size != nullptr) {
            *size = BOX_SIZE;
        }
        if (data != nullptr) {
            auto const * BEGIN = msg->box.cast<int8_t>();
            auto const * END   = BEGIN + (BOX_SIZE <= max ? BOX_SIZE : max);
            std::copy(BEGIN, END, data);
        }
        return true;
    }
};

struct MqInterface
{
    virtual bool send() = 0;
    virtual bool recv() = 0;
};

} // namespace details
} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MQ_DETAILS_MQCOMMON_HPP__

