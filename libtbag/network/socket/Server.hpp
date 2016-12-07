/**
 * @file   Server.hpp
 * @brief  Server class prototype.
 * @author zer0
 * @date   2016-10-14
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_SOCKET_SERVER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_SOCKET_SERVER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/container/Pointer.hpp>
#include <libtbag/loop/UvEventLoop.hpp>
#include <libtbag/util/UvUtils.hpp>
#include <libtbag/network/socket/Tcp.hpp>

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <limits>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace socket  {

/**
 * Server class prototype.
 *
 * @author zer0
 * @date   2016-10-14
 * @date   2016-11-07 (Refactoring this class)
 */
class TBAG_API Server : public Noncopyable
{
public:
    using Loop      = loop::UvEventLoop;
    using Handle    = util::UvHandle;

    using ClientKey   = container::Pointer<void>;
    using ClientValue = std::shared_ptr<Tcp>;

    using ClientMap = std::unordered_map<ClientKey, ClientValue, ClientKey::Hash>;

public:
    enum class Code
    {
        SUCCESS,
        FAILURE,
        END_OF_FILE,
    };

    struct EventCallback
    {
        virtual bool onConnection(std::string & client_address, int status) = 0;

        virtual void onClose() = 0;
        virtual void onCloseClient(ClientKey key) = 0;

        virtual void onRead(ClientKey from, Code code, char * buffer, std::size_t size) = 0;
        virtual void onWrite(ClientKey to, Code code) = 0;
    };

private:
    Loop   _loop;
    Tcp    _tcp;
    Handle _write;

private:
    ClientMap _clients;
    std::vector<char> _read_buffer;

private:
    EventCallback * _callback;

public:
    Server();
    ~Server();

public:
    inline void setEventCallback(EventCallback * callback)
    { _callback = callback; }

public:
    bool run(std::string const & address, int port);
    bool runIpv4(std::string const & address, int port);
    bool runIpv6(std::string const & address, int port);
    void close();

public:
    void closeClient(ClientKey key);
    bool write(ClientKey key, char * buffer, std::size_t size);

public:
    void onConnection(void * server, int status);
    void onClose(void * handle);
    void onCloseClient(void * handle);

    void onReadBufferAlloc(void * handle, size_t suggested_size, void * buf);
    void onRead(void * stream, ssize_t nread, void const * buf);
    void onWrite(void * req, int status);
};

} // namespace socket
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_SOCKET_SERVER_HPP__

