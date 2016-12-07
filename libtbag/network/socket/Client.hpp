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
#include <libtbag/loop/UvEventLoop.hpp>
#include <libtbag/util/UvUtils.hpp>
#include <libtbag/network/socket/Tcp.hpp>

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
class TBAG_API Client : public Noncopyable
{
public:
    using Loop   = loop::UvEventLoop;
    using Handle = util::UvHandle;

public:
    enum class Code
    {
        SUCCESS,
        FAILURE,
        END_OF_FILE,
    };

    struct EventCallback
    {
        virtual bool onConnect(int status) = 0;
        virtual void onClose() = 0;
        virtual void onRead(Code code, char const * buffer, std::size_t size) = 0;
        virtual void onWrite(Code code) = 0;
    };

private:
    Loop   _loop;
    Tcp    _tcp;
    Handle _connect;
    Handle _write;

private:
    std::vector<char> _read_buffer;

private:
    EventCallback * _callback;

public:
    Client(EventCallback * callback);
    Client();
    ~Client();

public:
    inline void setEventCallback(EventCallback * callback)
    { _callback = callback; }

public:
    bool run(std::string const & address, int port);
    bool runIpv4(std::string const & address, int port);
    bool runIpv6(std::string const & address, int port);
    void close();

public:
    bool write(char const * buffer, std::size_t size);

public:
    void onConnect(void * req, int status);
    void onClose(void * handle);
    void onReadBufferAlloc(void * handle, size_t suggested_size, void * buf);
    void onRead(void * stream, ssize_t nread, void const * buf);
    void onWrite(void * req, int status);
};

} // namespace socket
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_SOCKET_CLIENT_HPP__

