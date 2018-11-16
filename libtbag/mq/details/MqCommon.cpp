/**
 * @file   MqCommon.cpp
 * @brief  MqCommon class implementation.
 * @author zer0
 * @date   2018-11-10
 */

#include <libtbag/mq/details/MqCommon.hpp>
#include <libtbag/log/Log.hpp>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace mq      {
namespace details {

// -----------------------------
// MqMsgCopyFrom implementation.
// -----------------------------

MqMsgCopyFrom::MqMsgCopyFrom(MqMsg const & msg) : source_msg(msg)
{
    // EMPTY.
}

MqMsgCopyFrom::~MqMsgCopyFrom()
{
    // EMPTY.
}

bool MqMsgCopyFrom::operator()(MqMsg * msg)
{
    assert(msg != nullptr);
    *msg = source_msg;
    return true;
}

// ---------------------------
// MqMsgCopyTo implementation.
// ---------------------------

MqMsgCopyTo::MqMsgCopyTo(MqMsg & msg) : destination_msg(msg)
{
    // EMPTY.
}

MqMsgCopyTo::~MqMsgCopyTo()
{
    // EMPTY.
}

bool MqMsgCopyTo::operator()(MqMsg * msg)
{
    destination_msg = *msg;
    return true;
}

// -----------------------
// Miscellaneous utilities
// -----------------------

MqParams convertUriToParams(std::string const & uri)
{
    MqParams params;
    return params;
}

} // namespace details
} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

