/**
 * @file   TcpNet.hpp
 * @brief  TcpNet class prototype.
 * @author zer0
 * @date   2017-05-03
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DETAILS_TCPNET_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DETAILS_TCPNET_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/network/Client.hpp>
#include <libtbag/network/Server.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace details {

/**
 * TcpNet client class prototype.
 *
 * @author zer0
 * @date   2017-05-03
 */
class TBAG_API TcpNetClient : public Client
{
private:
    // Insert member variables.

public:
    TcpNetClient(Loop & loop);
    virtual ~TcpNetClient();

public:
    virtual Type getType() const override;
};

/**
 * TcpNet server class prototype.
 *
 * @author zer0
 * @date   2017-05-03
 */
class TBAG_API TcpNetServer : public Server
{
private:
    // Insert member variables.

public:
    TcpNetServer(Loop & loop);
    virtual ~TcpNetServer();

public:
    virtual Type getType() const override;
};

} // namespace details
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DETAILS_TCPNET_HPP__

