/**
 * @file   StreamNode.cpp
 * @brief  StreamNode class implementation.
 * @author zer0
 * @date   2017-05-23
 * @date   2017-06-01 (Rename: StreamServerNode -> StreamNode)
 */

#include <libtbag/network/stream/StreamNode.hpp>
#include <libtbag/log/Log.hpp>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace stream  {

StreamNode::StreamNode(Loop & loop, StreamType type, ServerInterface * parent)
        : StreamClient(loop, type), _parent(parent)
{
    updateWriteStatusToReady();
}

StreamNode::StreamNode(Loop & loop, StreamType type, SharedSafetyAsync async, ServerInterface * parent)
        : StreamClient(loop, type, async), _parent(parent)
{
    updateWriteStatusToReady();
}

StreamNode::~StreamNode()
{
    // EMPTY.
}

StreamNode::WeakClient StreamNode::getWeakClient()
{
    assert(_parent != nullptr);
    return _parent->getClient(this->getId());
}

void StreamNode::onConnect(Err code)
{
    assert(false && "Not used.");
}

void StreamNode::onShutdown(Err code)
{
    assert(_parent != nullptr);
    _parent->onClientShutdown(getWeakClient(), code);
}

void StreamNode::onWrite(Err code)
{
    assert(_parent != nullptr);
    _parent->onClientWrite(getWeakClient(), code);
}

void StreamNode::onRead(Err code, char const * buffer, std::size_t size)
{
    assert(_parent != nullptr);
    _parent->onClientRead(getWeakClient(), code, buffer, size);
}

void StreamNode::onClose()
{
    assert(_parent != nullptr);
    _parent->onClientClose(getWeakClient());

    // Deallocate user data.
    void * user_data = this->getUserData();
    if (user_data) {
        _parent->onClientUserDataDealloc(getWeakClient(), user_data);
    }

    _parent->removeClient(this->getId());
}

} // namespace stream
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

