/**
 * @file   MassiveUdp.hpp
 * @brief  MassiveUdp class prototype.
 * @author zer0
 * @date   2017-01-12
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_SAMPLE_MASSIVEUDP_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_SAMPLE_MASSIVEUDP_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace sample  {

/**
 * MassiveUdpServer class prototype.
 *
 * @author zer0
 * @date   2017-01-12
 */
class TBAG_API MassiveUdpServer
{
public:
    MassiveUdpServer();
    ~MassiveUdpServer();
};

/**
 * MassiveUdpClient class prototype.
 *
 * @author zer0
 * @date   2017-01-12
 */
class TBAG_API MassiveUdpClient
{
public:
    MassiveUdpClient();
    ~MassiveUdpClient();
};

char const * const TEST_MASSIVE_UDP_SERVER_ADDRESS =   "0.0.0.0";
char const * const TEST_MASSIVE_UDP_CLIENT_ADDRESS = "127.0.0.1";
int          const TEST_MASSIVE_UDP_PORT           =        9999;

TBAG_API int runMassiveUdpServer(std::string const & ip = TEST_MASSIVE_UDP_SERVER_ADDRESS, int port = TEST_MASSIVE_UDP_PORT);
TBAG_API int runMassiveUdpClient(std::string const & ip = TEST_MASSIVE_UDP_CLIENT_ADDRESS, int port = TEST_MASSIVE_UDP_PORT);

} // namespace sample
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_SAMPLE_MASSIVEUDP_HPP__

