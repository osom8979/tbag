/**
 * @file   Client.hpp
 * @brief  Client class prototype.
 * @author zer0
 * @date   2016-10-14
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_SOCKET_CLIENT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_SOCKET_CLIENT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/debug/ErrorCode.hpp>
#include <libtbag/uv/Loop.hpp>
#include <libtbag/uv/Tcp.hpp>
#include <libtbag/uv/Request.hpp>
#include <libtbag/uv/ex/RequestQueue.hpp>

#include <string>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace socket  {

/**
 * Client class prototype.
 *
 * @author zer0
 * @date   2016-10-14
 * @date   2016-11-07 (Refactoring this class)
 */
class TBAG_API Client : public uv::Tcp
{
public:
    using Parent     = uv::Tcp;
    using BufferInfo = uv::BufferInfo;
    using Loop       = uv::Loop;

    using ConnectRequest = uv::ConnectRequest;
    using RequestQueue   = uv::ex::RequestQueue;

    using Buffer = std::vector<char>;

public:
    struct EventCallback
    {
        virtual void onConnect(Err code) = 0;
        virtual void onClose() = 0;
        virtual void onRead(Err code, char const * buffer, std::size_t size) = 0;
        virtual void onWrite(Err code) = 0;
    };

private:
    ConnectRequest _connector;
    RequestQueue   _writer;

private:
    Buffer _read_buffer;

private:
    EventCallback * _callback;

public:
    Client(Loop & loop, EventCallback * callback);
    Client(Loop & loop);
    ~Client();

public:
    inline void setEventCallback(EventCallback * callback) TBAG_NOEXCEPT
    { _callback = callback; }

public:
    bool init(std::string const & ip, int port);
    bool initIpv4(std::string const & ip, int port);

public:
    bool autoWrite(char const * buffer, std::size_t size);

// Tcp Event methods.
public:
    virtual void onConnect(Err code) override;
    virtual BufferInfo onAlloc(std::size_t suggested_size) override;
    virtual void onRead(Err code, char const * buffer, std::size_t size) override;
    virtual void onWrite(Err code) override;
    virtual void onClose() override;
};

} // namespace socket
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_SOCKET_CLIENT_HPP__

