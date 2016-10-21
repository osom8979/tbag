/**
 * @file   tbserver.cpp
 * @brief  Program entry-point.
 * @author zer0
 * @date   2016-10-17
 */

#include <iostream>
#include <libtbag/network/socket/Server.hpp>

int main(int argc, char ** argv)
{
    using Server = libtbag::network::socket::Server;
    Server server;

    std::cout << "Start server!\n";
    server.runIpv4("0.0.0.0", 9999);

    std::cout << "Start end.\n";
    return 0;
}

