/**
 * @file   StreamNode.hpp
 * @brief  StreamNode class prototype.
 * @author zer0
 * @date   2017-05-23
 * @date   2017-06-01 (Rename: StreamServerNode -> StreamNode)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_STREAM_STREAMNODE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_STREAM_STREAMNODE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/Type.hpp>

#include <libtbag/network/details/NetCommon.hpp>
#include <libtbag/network/stream/StreamClient.hpp>
#include <libtbag/uvpp/Loop.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace stream  {

/**
 * BaseStreamNode class prototype.
 *
 * @author zer0
 * @date   2017-09-28
 */
template <typename StreamClientType>
class BaseStreamNode : public StreamClientType
{
public:
    using Parent = StreamClientType;
    STATIC_ASSERT_CHECK_IS_BASE_OF(stream::StreamClient, Parent);

public:
    using StreamType      = details::StreamType;
    using ServerInterface = details::ServerInterface;

    using WeakClient = ServerInterface::WeakClient;
    using Loop       = uvpp::Loop;

    using ReadPacket        = StreamClient::ReadPacket;
    using SharedSafetyAsync = StreamClient::SharedSafetyAsync;
    using WeakSafetyAsync   = StreamClient::WeakSafetyAsync;

private:
    ServerInterface * _parent;

public:
    BaseStreamNode(Loop & loop, StreamType type, ServerInterface * parent)
            : StreamClientType(loop, type, StreamClient::UpdateReady()), _parent(parent)
    {
        assert(StreamClientType::getState() == StreamClient::WriteState::WS_READY);
    }

    virtual ~BaseStreamNode()
    {
        // EMPTY.
    }

public:
    inline ServerInterface       * getParentPtr()       TBAG_NOEXCEPT { return _parent; }
    inline ServerInterface const * getParentPtr() const TBAG_NOEXCEPT { return _parent; }

public:
    WeakClient getWeakClient()
    {
        assert(_parent != nullptr);
        return _parent->get(this->id());
    }

public:
    virtual void onConnect (Err code) override
    {
        TBAG_INACCESSIBLE_BLOCK_ASSERT();
    }

    virtual void onShutdown(Err code) override
    {
        assert(_parent != nullptr);
        _parent->onClientShutdown(getWeakClient(), code);
    }

    virtual void onWrite(Err code) override
    {
        assert(_parent != nullptr);
        _parent->onClientWrite(getWeakClient(), code);
    }

    virtual void onRead(Err code, ReadPacket const & packet) override
    {
        assert(_parent != nullptr);
        _parent->onClientRead(getWeakClient(), code, packet);
    }

    virtual void onClose() override
    {
        assert(_parent != nullptr);
        _parent->onClientClose(getWeakClient());
        _parent->remove(this->id());
    }

    virtual void onTimer() override
    {
        assert(_parent != nullptr);
        _parent->onClientTimer(getWeakClient());
    }
};

/**
 * StreamNode class prototype.
 *
 * @author zer0
 * @date   2017-05-23
 * @date   2017-06-01 (Rename: StreamServerNode -> StreamNode)
 */
using StreamNode = BaseStreamNode<StreamClient>;

} // namespace stream
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_STREAM_STREAMNODE_HPP__

