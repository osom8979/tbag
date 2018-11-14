/**
 * @file   MqCommon.cpp
 * @brief  MqCommon class implementation.
 * @author zer0
 * @date   2018-11-10
 */

#include <libtbag/mq/details/MqCommon.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace mq      {
namespace details {

// -----------------------------
// MqMsgCopyFrom implementation.
// -----------------------------

MqMsgCopyFrom::MqMsgCopyFrom(MqEvent e) : MqMsgCopyFrom(e, nullptr, 0)
{
    // EMPTY.
}

MqMsgCopyFrom::MqMsgCopyFrom(char const * d, std::size_t s)
        : MqMsgCopyFrom(MqEvent::ME_MSG, d, s)
{
    // EMPTY.
}

MqMsgCopyFrom::MqMsgCopyFrom(MqEvent e, char const * d, std::size_t s)
        : event(e), data(d), size(s)
{
    // EMPTY.
}

MqMsgCopyFrom::~MqMsgCopyFrom()
{
    // EMPTY.
}

bool MqMsgCopyFrom::operator()(MqMsg * msg)
{
    msg->event = event;
    if (data == nullptr || size == 0) {
        return true;
    }
    msg->buffer.assign(data, data + size);
    return true;
}

// ---------------------------
// MqMsgCopyTo implementation.
// ---------------------------

MqMsgCopyTo::MqMsgCopyTo(MqEvent * e, char * d, std::size_t m, std::size_t * s)
        : event(e), data(d), max(m), size(s)
{
    // EMPTY.
}

MqMsgCopyTo::~MqMsgCopyTo()
{
    // EMPTY.
}

bool MqMsgCopyTo::operator()(MqMsg * msg)
{
    if (event != nullptr) {
        *event = msg->event;
    }
    auto const SIZE = msg->size();
    if (size != nullptr) {
        *size = SIZE;
    }
    if (data != nullptr) {
        auto const BEGIN = msg->begin();
        auto const END   = BEGIN + (SIZE <= max ? SIZE : max);
        std::copy(BEGIN, END, data);
    }
    return true;
}

} // namespace details
} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

