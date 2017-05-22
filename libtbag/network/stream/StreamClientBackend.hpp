/**
 * @file   StreamClientBackend.hpp
 * @brief  StreamClientBackend class prototype.
 * @author zer0
 * @date   2017-05-22
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_STREAM_STREAMCLIENTBACKEND_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_STREAM_STREAMCLIENTBACKEND_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/Type.hpp>

#include <libtbag/network/details/NetCommon.hpp>
#include <libtbag/network/Client.hpp>
#include <libtbag/uvpp/UvCommon.hpp>
#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/uvpp/Request.hpp>

#include <libtbag/uvpp/Tcp.hpp>
#include <libtbag/uvpp/Pipe.hpp>

#include <cassert>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace stream  {

/**
 * StreamClientBackend class prototype.
 *
 * @author zer0
 * @date   2017-05-10
 * @date   2017-05-22 (Rename: StreamClient::ClientBackend -> StreamClientBackend)
 */
template <typename BaseType>
class StreamClientBackend : public BaseType
{
public:
    using StreamType = details::StreamType;
    using Parent     = BaseType;
    using Loop       = uvpp::Loop;
    using binf       = uvpp::binf;
    using Buffer     = std::vector<char>;

    using ConnectRequest  = uvpp::ConnectRequest;
    using ShutdownRequest = uvpp::ShutdownRequest;
    using WriteRequest    = uvpp::WriteRequest;

    TBAG_CONSTEXPR static StreamType getStreamType() TBAG_NOEXCEPT
    {
        return details::IsNetworkType<BaseType>::STREAM_TYPE;
    }

    static_assert(getStreamType() != StreamType::UNKNOWN,
                  "The BaseType must be uvpp::Tcp or uvpp::Pipe.");

private:
    Client * _parent;
    Buffer   _buffer;

    ConnectRequest _connect_req;

public:
    StreamClientBackend(Loop & loop, Client * parent) : Parent(loop), _parent(parent)
    { /* EMPTY. */ }
    virtual ~StreamClientBackend()
    { /* EMPTY. */ }

public:
    inline ConnectRequest       & atConnectReq()       TBAG_NOEXCEPT { return _connect_req; }
    inline ConnectRequest const & atConnectReq() const TBAG_NOEXCEPT { return _connect_req; }

public:
    virtual void onConnect(ConnectRequest & request, Err code) override
    {
        assert(_parent != nullptr);
        _parent->runBackendConnect(code);
    }

    virtual void onShutdown(ShutdownRequest & request, Err code) override
    {
        assert(_parent != nullptr);
        _parent->runBackendShutdown(code);
    }

    virtual void onWrite(WriteRequest & request, Err code) override
    {
        assert(_parent != nullptr);
        _parent->runBackendWrite(code);
    }

    virtual binf onAlloc(std::size_t suggested_size) override
    {
        return uvpp::defaultOnAlloc(_buffer, suggested_size);
    }

    virtual void onRead(Err code, char const * buffer, std::size_t size) override
    {
        assert(_parent != nullptr);
        _parent->runBackendRead(code, buffer, size);
    }

    virtual void onClose() override
    {
        assert(_parent != nullptr);
        _parent->runBackendClose();
    }
};

using  TcpStreamClientBackend = StreamClientBackend<uvpp::Tcp>;
using PipeStreamClientBackend = StreamClientBackend<uvpp::Pipe>;

} // namespace stream
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_STREAM_STREAMCLIENTBACKEND_HPP__

