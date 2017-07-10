/**
 * @file   NetworkUdpTest.cpp
 * @brief  NetworkUdp class tester.
 * @author zer0
 * @date   2017-06-15
 */

#include <gtest/gtest.h>
#include <libtbag/log/Log.hpp>
#include <libtbag/network/details/NetCommon.hpp>
#include <libtbag/network/udp/UdpServer.hpp>

#include <iostream>

using namespace libtbag;
using namespace libtbag::uvpp;
using namespace libtbag::network;
using namespace libtbag::network::udp;
using namespace libtbag::network::details;

TEST(NetworkUdpTest, Default)
{
    log::SeverityGuard guard;

    char const TEST_MESSAGE[] = "TEST_UDP_MESSAGE";
    using Buffer = std::vector<char>;

    Loop loop;
    FunctionalUdpServer server1(loop);
    FunctionalUdpServer server2(loop);

    ASSERT_EQ(Err::E_SUCCESS, server1.init(ANY_IPV4, 0));
    ASSERT_EQ(Err::E_SUCCESS, server2.init(ANY_IPV4, 0));

    //std::cout << "* Server1: " << server1.dest() << ":" << server1.port() << std::endl;
    //std::cout << "* Server2: " << server2.dest() << ":" << server2.port() << std::endl;

    auto sender1 = server1.newSender(LOOPBACK_IPV4, server2.port()).lock();
    auto sender2 = server2.newSender(LOOPBACK_IPV4, server1.port()).lock();

    ASSERT_TRUE(static_cast<bool>(sender1));
    ASSERT_TRUE(static_cast<bool>(sender2));

    int server1_client_count = 0;
    int server1_client_read  = 0;
    int server1_client_write = 0;
    int server1_client_close = 0;
    int server1_server_close = 0;

    int server2_client_count = 0;
    int server2_client_read  = 0;
    int server2_client_write = 0;
    int server2_client_close = 0;
    int server2_server_close = 0;

    std::string result_message;

    server1.setOnClientRead([&](FunctionalUdpServer::WeakClient node, Err code, ReadPacket const & packet){
        if (code == Err::E_SUCCESS && packet.size > 0 && packet.addr != nullptr) {
            ++server1_client_read;
            server1.foreach([&](UdpServer::ClientPair const & value){
                ++server1_client_count;
                value.second->write(packet.buffer, packet.size);
            });
        }
    });
    server1.setOnClientWrite([&](FunctionalUdpServer::WeakClient node, Err code){
        ++server1_client_write;
        server1.close();
    });
    server1.setOnClientClose([&](FunctionalUdpServer::WeakClient node){
        ++server1_client_close;
    });
    server1.setOnServerClose([&](){
        ++server1_server_close;
    });

    server2.setOnClientRead([&](FunctionalUdpServer::WeakClient node, Err code, ReadPacket const & packet){
        if (code == Err::E_SUCCESS && packet.size > 0 && packet.addr != nullptr) {
            ++server2_client_read;
            result_message.assign(packet.buffer, packet.buffer + packet.size);
            server2.close();
        }
    });
    server2.setOnClientWrite([&](FunctionalUdpServer::WeakClient node, Err code){
        ++server2_client_write;
    });
    server2.setOnClientClose([&](FunctionalUdpServer::WeakClient node){
        ++server2_client_close;
    });
    server2.setOnServerClose([&](){
        ++server2_server_close;
    });

    server2.foreach([&](UdpServer::ClientPair const & value){
        ++server2_client_count;
        value.second->write(TEST_MESSAGE, sizeof(TEST_MESSAGE));
    });

    ASSERT_EQ(Err::E_SUCCESS, loop.run());

    ASSERT_EQ(1, server1_client_count);
    ASSERT_EQ(1, server1_client_read );
    ASSERT_EQ(1, server1_client_write);
    ASSERT_EQ(2, server1_client_close);
    ASSERT_EQ(0, server1_server_close);

    ASSERT_EQ(1, server2_client_count);
    ASSERT_EQ(1, server2_client_read );
    ASSERT_EQ(1, server2_client_write);
    ASSERT_EQ(2, server2_client_close);
    ASSERT_EQ(0, server2_server_close);

    ASSERT_STREQ(TEST_MESSAGE, result_message.c_str());
}

