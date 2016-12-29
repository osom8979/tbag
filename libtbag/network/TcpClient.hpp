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
#include <libtbag/network/TcpLoop.hpp>

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
class TBAG_API TcpClient : public TcpLoopEx
{
private:
    ConnectRequest _connector;

public:
    TcpClient();
    virtual ~TcpClient();

public:
    bool initIpv4(std::string const & ip, int port);

public:
    virtual bool run(std::string const & ip, int port) override;
};

} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_TCPCLIENT_HPP__

