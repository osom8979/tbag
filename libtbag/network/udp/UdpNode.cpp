/**
 * @file   UdpNode.cpp
 * @brief  UdpNode class implementation.
 * @author zer0
 * @date   2017-06-15
 */

#include <libtbag/network/udp/UdpNode.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/Noncopyable.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace udp     {

/**
 * Udp internal helper class.
 *
 * @author zer0
 * @date   2017-06-17
 *
 * @warning
 *  Don't mutex guard in this class. (However, callbacks are an exception)
 */
struct UdpNode::Internal : private Noncopyable
{
    using binf = UdpNode::binf;

    UdpNode * _parent;

    Internal(UdpNode * parent) : _parent(parent)
    {
        // EMPTY.
    }

    ~Internal()
    {
        // EMPTY.
    }
};

// -----------------------
// UdpNode implementation.
// -----------------------

UdpNode::UdpNode(Loop & loop, NodeType type, SharedSafetyAsync async, ServerInterface * parent)
        : NODE_TYPE(type), _owner_async(false), _internal(new Internal(this))
{
    _client = loop.newHandle<UdpNodeBackend>(loop, this);
    assert(static_cast<bool>(_client));

    if (static_cast<bool>(async) && loop.id() == async->getLoop()->id()) {
        _owner_async = false;
        _async = async;
    } else {
        _owner_async = true;
        _async.reset();
    }

    _sender.status = SendStatus::SS_NOT_READY;
}

UdpNode::~UdpNode()
{
    // EMPTY.
}

char const * UdpNode::getSendStatusName(SendStatus status) TBAG_NOEXCEPT
{
    switch (status) {
    case SendStatus::SS_NOT_READY:     return "NOT_READY";
    case SendStatus::SS_READY:         return "READY";
    case SendStatus::SS_ASYNC:         return "ASYNC";
    case SendStatus::SS_ASYNC_CANCEL:  return "ASYNC_CANCEL";
    case SendStatus::SS_SEND:          return "SEND";
    case SendStatus::SS_END:           return "END";
    }

    TBAG_INACCESSIBLE_BLOCK_ASSERT();
    return "UNKNOWN";
}

} // namespace udp
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

