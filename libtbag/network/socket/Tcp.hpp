/**
 * @file   Tcp.hpp
 * @brief  Tcp class prototype.
 * @author zer0
 * @date   2016-11-04
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_SOCKET_TCP_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_SOCKET_TCP_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/loop/UvEventLoop.hpp>

#include <string>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace socket  {

/**
 * Tcp class prototype.
 *
 * @author zer0
 * @date   2016-11-04
 */
class Tcp : public libtbag::Noncopyable
{
public:
    struct StreamPimpl;
    friend struct StreamPimpl;

public:
    enum class ReadErrorCode
    {
        UNKNOWN_ERROR = 0,
        SUCCESS,
        END_OF_FILE,
    };

    enum class WriteErrorCode
    {
        UNKNOWN_ERROR = 0,
        SUCCESS,
    };

    struct Callback
    {
        virtual void onConnect(int status){};
        virtual void onConnection(int status){};

        virtual void onCloseTcp(){};
        virtual void onCloseWrite(){};
        virtual void onCloseConnect(){};

        virtual void onRead(ReadErrorCode code, char * buffer, std::size_t length) = 0;
        virtual void onWrite(WriteErrorCode code) = 0;
    };

public:
    using EventLoop    = libtbag::loop::UvEventLoop;
    using UniqueStream = std::unique_ptr<StreamPimpl>;

private:
    EventLoop    _loop;
    UniqueStream _stream;
    Callback *   _callback;

public:
    Tcp(Callback * callback);
    Tcp();
    ~Tcp();

public:
    inline void setCallback(Callback * callback)
    { _callback = callback; }

public:
    void * getTcp();
    void const * getTcp() const;

public:
    bool init(libtbag::loop::UvEventLoop & loop);

// CLIENT ONLY.
public:
    bool connectIpv4(std::string const & ip, int port);
    bool connectIpv6(std::string const & ip, int port);

// SERVER ONLY.
public:
    bool bindIpv4(std::string const & ip, int port);
    bool bindIpv6(std::string const & ip, int port);
    bool listen();
    bool accept(Tcp & client);

public:
    bool read();
    bool write(char const * buffer, std::size_t length);

public:
    void closeTcp();
    void closeConnect();
    void closeWrite();

public:
    static bool isIpv4(std::string const & ip);
};

} // namespace socket
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_SOCKET_TCP_HPP__

