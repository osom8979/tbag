/**
 * @file   StreamServerBackend.hpp
 * @brief  StreamServerBackend class prototype.
 * @author zer0
 * @date   2017-05-23
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_STREAM_STREAMSERVERBACKEND_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_STREAM_STREAMSERVERBACKEND_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/Type.hpp>

#include <libtbag/net/Ip.hpp>
#include <libtbag/network/details/NetInterface.hpp>

#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace stream  {

/**
 * StreamServerBackend class prototype.
 *
 * @author zer0
 * @date   2017-05-23
 * @date   2017-05-10
 * @date   2017-05-23 (Rename: StreamServer::ServerBackend -> StreamClientBackend)
 */
template <typename BaseType>
class StreamServerBackend : public BaseType
{
public:
    using StreamType      = details::StreamType;
    using ServerInterface = details::ServerInterface;

    using Parent = BaseType;
    using Loop   = uvpp::Loop;

public:
    TBAG_CONSTEXPR static StreamType const STREAM_TYPE = details::IsNetworkType<BaseType>::STREAM_TYPE;

    static_assert(STREAM_TYPE != StreamType::UNKNOWN, "The BaseType must be uvpp::Tcp or uvpp::Pipe.");

public:
    TBAG_CONSTEXPR static StreamType getStreamType() TBAG_NOEXCEPT { return STREAM_TYPE; }

private:
    ServerInterface * _parent;

public:
    StreamServerBackend(Loop & loop, ServerInterface * parent) : Parent(loop), _parent(parent)
    {
        // EMPTY.
    }

    virtual ~StreamServerBackend()
    {
        // EMPTY.
    }

public:
    virtual void onConnection(Err code) override
    {
        assert(_parent != nullptr);
        _parent->backConnection(code);
    }

    virtual void onClose() override
    {
        assert(_parent != nullptr);
        _parent->backClose();
    }
};

} // namespace stream
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_STREAM_STREAMSERVERBACKEND_HPP__

