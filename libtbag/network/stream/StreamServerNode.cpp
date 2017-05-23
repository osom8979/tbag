/**
 * @file   StreamServerNode.cpp
 * @brief  StreamServerNode class implementation.
 * @author zer0
 * @date   2017-05-23
 */

#include <libtbag/network/stream/StreamServerNode.hpp>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace stream  {

StreamServerNode::StreamServerNode(Loop & loop, StreamType type, Server * parent)
        : StreamClient(loop, type), _parent(parent)
{
    // EMPTY.
}

StreamServerNode::~StreamServerNode()
{
    // EMPTY.
}

StreamServerNode::WeakClient StreamServerNode::getWeakClient()
{
    assert(_parent != nullptr);
    return _parent->getClient(this->getId());
}

void StreamServerNode::onConnect(Err code)
{
    // EMPTY.
}

void StreamServerNode::onShutdown(Err code)
{
    assert(_parent != nullptr);
    _parent->onClientShutdown(getWeakClient(), code);
}

void StreamServerNode::onWrite(Err code)
{
    assert(_parent != nullptr);
    _parent->onClientWrite(getWeakClient(), code);
}

void StreamServerNode::onRead(Err code, char const * buffer, std::size_t size)
{
    assert(_parent != nullptr);
    _parent->onClientRead(getWeakClient(), code, buffer, size);
}

void StreamServerNode::onClose()
{
    assert(_parent != nullptr);
    _parent->onClientClose(getWeakClient());

    // Deallocate user data.
    void * user_data = this->getUserData();
    if (user_data) {
        _parent->onClientUserDataDealloc(getWeakClient(), user_data);
    }
}

} // namespace stream
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

