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

#include <string>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

// Forward declaration.
namespace loop { class UvEventLoop; }

namespace network {
namespace socket  {

//class TBAG_API Tcp : public libtbag::Noncopyable
//{
//public:
//    struct StreamPimpl;
//    friend struct StreamPimpl;
//
//public:
//    enum class ReadErrorCode
//    {
//        UNKNOWN_ERROR = 0,
//        SUCCESS,
//        END_OF_FILE,
//    };
//
//    enum class WriteErrorCode
//    {
//        UNKNOWN_ERROR = 0,
//        SUCCESS,
//    };
//
//    struct Callback
//    {
//        virtual void onConnect(int status) = 0;
//        virtual void onConnection(int status) = 0;
//
//        virtual void onClose() = 0;
//
//        virtual void onRead(ReadErrorCode code, char * buffer, std::size_t length) = 0;
//        virtual void onWrite(WriteErrorCode code) = 0;
//    };
//
//public:
//    using UniqueStream = std::unique_ptr<StreamPimpl>;
//
//private:
//    UniqueStream _stream;
//    Callback *   _callback;
//
//public:
//    Tcp(Callback * callback);
//    Tcp();
//    virtual ~Tcp();
//
//public:
//    inline void setCallback(Callback * callback)
//    { _callback = callback; }
//
//public:
//    void * getTcp();
//    void const * getTcp() const;
//
//public:
//    inline std::size_t getId() const
//    { return reinterpret_cast<std::size_t>(getTcp()); }
//
//public:
//    bool init(libtbag::loop::UvEventLoop & loop);
//
//// CLIENT ONLY.
//public:
//    bool connectIpv4(std::string const & ip, int port);
//    bool connectIpv6(std::string const & ip, int port);
//
//// SERVER ONLY.
//public:
//    bool bindIpv4(std::string const & ip, int port);
//    bool bindIpv6(std::string const & ip, int port);
//    bool listen();
//    bool accept(Tcp & client);
//
//public:
//    std::string getSocketName() const;
//    std::string getPeerName() const;
//
//public:
//    bool read();
//    bool write(char const * buffer, std::size_t length);
//
//public:
//    void close();
//
//public:
//    static bool isIpv4(std::string const & ip);
//};

/**
 * Tcp class prototype.
 *
 * @author zer0
 * @date   2016-11-04
 * @date   2016-11-07 (Refactoring this class)
 */
class TBAG_API Tcp : public Noncopyable
{
public:
    struct TcpPimpl;
    friend struct TcpPimpl;

public:
    using UniqueTcp = std::unique_ptr<TcpPimpl>;

private:
    UniqueTcp _tcp;

public:
    Tcp();
    ~Tcp();

public:
    bool init(libtbag::loop::UvEventLoop & loop);
    void close();

public:
    std::string getPeerName() const;
    std::string getSocketName() const;

public:
    void * getNative();
    void const * getNative() const;

public:
    static bool isIpv4(std::string const & address);
    static bool isIpv6(std::string const & address);
};

} // namespace socket
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_SOCKET_TCP_HPP__

