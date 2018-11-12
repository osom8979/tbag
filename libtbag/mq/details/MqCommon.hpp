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

enum class MqType : int
{
    MT_NONE,
    MT_BOX_ADDRESS,
    MT_CREATE,
};

struct MqMsg
{
    using Box          = libtbag::container::Box;
    using BoxTypeTable = libtbag::container::BoxTypeTable;

    MqType type;
    Box    box;

    MqMsg(std::size_t size) : type(MqType::MT_NONE),
                              box(BoxTypeTable::BTT_INT8, static_cast<unsigned>(size))
    { /* EMPTY. */ }

    ~MqMsg()
    { /* EMPTY. */ }
};

struct MqMsgCopyFrom
{
    char const * data;
    std::size_t  size;

    MqMsgCopyFrom(char const * d, std::size_t s) : data(d), size(s)
    { /* EMPTY. */ }

    ~MqMsgCopyFrom()
    { /* EMPTY. */ }

    bool operator()(MqMsg * msg)
    {
        Err code;
        msg->type = MqType::MT_BOX_ADDRESS;
        assert(size <= libtbag::type::TypeInfo<unsigned>::maximum());
        code = msg->box.resize(static_cast<unsigned>(size));
        assert(isSuccess(code));
        code = msg->box.copyFrom(data, size);
        assert(isSuccess(code));
        return true;
    }
};

struct MqMsgCopyTo
{
    MqType      * type;
    char        * data;
    std::size_t    max;
    std::size_t * size;

    MqMsgCopyTo(MqType * t, char * d, std::size_t m, std::size_t * s)
            : type(t), data(d), max(m), size(s)
    { /* EMPTY. */ }

    ~MqMsgCopyTo()
    { /* EMPTY. */ }

    bool operator()(MqMsg * msg)
    {
        if (type != nullptr) {
            *type = msg->type;
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

