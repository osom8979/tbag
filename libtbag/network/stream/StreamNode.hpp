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
 * StreamNode class prototype.
 *
 * @author zer0
 * @date   2017-05-23
 * @date   2017-06-01 (Rename: StreamServerNode -> StreamNode)
 */
class TBAG_API StreamNode : public StreamClient
{
public:
    using StreamType      = details::StreamType;
    using ServerInterface = details::ServerInterface;

    using WeakClient = ServerInterface::WeakClient;
    using Loop       = uvpp::Loop;

    using        ReadPacket = StreamClient::ReadPacket;
    using       SafetyAsync = StreamClient::SafetyAsync;
    using SharedSafetyAsync = StreamClient::SharedSafetyAsync;
    using   WeakSafetyAsync = StreamClient::WeakSafetyAsync;

private:
    ServerInterface * _parent;

public:
    StreamNode(Loop & loop, StreamType type, ServerInterface * parent);
    virtual ~StreamNode();

public:
    inline ServerInterface       * getParentPtr()       TBAG_NOEXCEPT { return _parent; }
    inline ServerInterface const * getParentPtr() const TBAG_NOEXCEPT { return _parent; }

public:
    WeakClient getWeakClient();

public:
    virtual void onConnect(Err code) override;
    virtual void onShutdown(Err code) override;
    virtual void onWrite(Err code) override;
    virtual void onRead(Err code, ReadPacket const & packet) override;
    virtual void onClose() override;
    virtual void onTimer() override;
};

} // namespace stream
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_STREAM_STREAMNODE_HPP__

