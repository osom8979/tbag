/**
 * @file   TcpClient.hpp
 * @brief  TcpClient class prototype.
 * @author zer0
 * @date   2016-12-29
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_TCPCLIENT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_TCPCLIENT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/uvpp/Tcp.hpp>
#include <libtbag/uvpp/Async.hpp>
#include <libtbag/uvpp/Request.hpp>

#include <libtbag/network/DatagramAdapter.hpp>

#include <string>
#include <vector>
#include <memory>
#include <mutex>
#include <atomic>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {

/**
 * TcpClient class prototype.
 *
 * @author zer0
 * @date   2016-12-29
 */
class TBAG_API TcpClient : public Noncopyable
{
public:
    using binf = uvpp::binf;
    using uerr = uvpp::uerr;

    using Loop  = uvpp::Loop;
    using Tcp   = uvpp::Tcp;
    using Async = uvpp::Async;
    using Job   = Async::Job;

    using ConnectRequest = uvpp::ConnectRequest;
    using WriteRequest   = uvpp::WriteRequest;

    using Buffer = std::vector<char>;

    using Mutex = std::mutex;
    using Guard = std::lock_guard<Mutex>;

private:
    ConnectRequest _connector;

public:
    TcpClient();
    virtual ~TcpClient();
};

} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_TCPCLIENT_HPP__

