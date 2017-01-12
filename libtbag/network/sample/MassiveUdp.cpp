/**
 * @file   MassiveUdp.cpp
 * @brief  MassiveUdp class implementation.
 * @author zer0
 * @date   2017-01-12
 */

#include <libtbag/network/sample/MassiveUdp.hpp>
#include <libtbag/log/Log.hpp>
#include <iostream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace sample  {

// ---------------------------------
// MassiveUdp server implementation.
// ---------------------------------

MassiveUdpServer::MassiveUdpServer()
{
    // EMPTY.
}

MassiveUdpServer::~MassiveUdpServer()
{
    // EMPTY.
}

// ---------------------------------
// MassiveUdp client implementation.
// ---------------------------------

MassiveUdpClient::MassiveUdpClient()
{
    // EMPTY.
}

MassiveUdpClient::~MassiveUdpClient()
{
    // EMPTY.
}

// -------------
// Main methods.
// -------------

int runMassiveUdpServer(std::string const & ip, int port)
{
    std::cout << "Start massive udp server: " << ip << " (" << port << ")\n";
    //return MassiveUdpServer().run(ip, port) ? EXIT_FAILURE : EXIT_SUCCESS;
    return EXIT_FAILURE;
}

int runMassiveUdpClient(std::string const & ip, int port)
{
    std::cout << "Start massive udp client: " << ip << " (" << port << ")\n";
    //return MassiveUdpClient().run(ip, port) ? EXIT_FAILURE : EXIT_SUCCESS;
    return EXIT_FAILURE;
}

} // namespace sample
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

