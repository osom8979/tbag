/**
 * @file   tbclient.cpp
 * @brief  Program entry-point.
 * @author zer0
 * @date   2016-10-17
 */

#include <iostream>
#include <libtbag/network/socket/Client.hpp>

int main(int argc, char ** argv)
{
    using Client = libtbag::network::socket::Client;
    Client client;

    std::cout << "Client start!\n";
    client.connect("127.0.0.1", 9999);

    std::cout << "Client end.\n";
    return 0;
}

