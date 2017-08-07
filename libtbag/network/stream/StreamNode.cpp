/**
 * @file   StreamNode.cpp
 * @brief  StreamNode class implementation.
 * @author zer0
 * @date   2017-05-23
 * @date   2017-06-01 (Rename: StreamServerNode -> StreamNode)
 */

#include <libtbag/network/stream/StreamNode.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace stream  {

StreamNode::StreamNode(Loop & loop, StreamType type, SharedSafetyAsync async, ServerInterface * parent)
        : StreamClient(loop, type, async, StreamClient::WriteReady()), _parent(parent)
{
    assert(getWriteState() == StreamClient::WriteState::WS_READY);
}

StreamNode::~StreamNode()
{
    // EMPTY.
}

StreamNode::WeakClient StreamNode::getWeakClient()
{
    assert(_parent != nullptr);
    return _parent->get(this->id());
}

void StreamNode::onConnect(Err code)
{
    TBAG_INACCESSIBLE_BLOCK_ASSERT();
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

void StreamNode::onRead(Err code, ReadPacket const & packet)
{
    assert(_parent != nullptr);
    _parent->onClientRead(getWeakClient(), code, packet);
}

void StreamNode::onClose()
{
    assert(_parent != nullptr);
    _parent->onClientClose(getWeakClient());

    // Deallocate user data.
    void * user_data = this->udata();
    if (user_data) {
        _parent->onClientUdataDealloc(getWeakClient(), user_data);
    }
    _parent->remove(this->id());
}

void StreamNode::onTimer()
{
    assert(_parent != nullptr);
    _parent->onClientTimer(getWeakClient());
}

} // namespace stream
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

