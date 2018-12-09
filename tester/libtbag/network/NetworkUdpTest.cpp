/**
 * @file   NetworkUdpTest.cpp
 * @brief  NetworkUdp class tester.
 * @author zer0
 * @date   2017-06-15
 */

#include <gtest/gtest.h>
#include <libtbag/log/Log.hpp>
#include <libtbag/network/details/NetCommon.hpp>
#include <libtbag/network/udp/UdpNode.hpp>

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
    FuncUdpNode node1(loop);
    FuncUdpNode node2(loop);

    node1.setFlags(FuncUdpNode::UDP_NODE_FLAG_USE_BIND);
    node2.setFlags(FuncUdpNode::UDP_NODE_FLAG_USE_BIND);

    ASSERT_EQ(Err::E_SUCCESS, node1.init(ANY_IPV4, 0));
    ASSERT_EQ(Err::E_SUCCESS, node2.init(ANY_IPV4, 0));

    node1.setDestination(LOOPBACK_IPV4, node2.port());
    node2.setDestination(LOOPBACK_IPV4, node1.port());

    int node1_read  = 0;
    int node1_write = 0;
    int node1_close = 0;

    int node2_read  = 0;
    int node2_write = 0;
    int node2_close = 0;

    std::string result_message;

    node1.set_onRead([&](Err code, ReadPacket const & packet){
        if (code == Err::E_SUCCESS && packet.size > 0 && packet.addr != nullptr) {
            ++node1_read;
            node1.write(packet.buffer, packet.size);
        }
    });
    node1.set_onWrite([&](Err code){
        ++node1_write;
        node1.close();
    });
    node1.set_onClose([&](){
        ++node1_close;
    });
    node1.start();

    node2.set_onRead([&](Err code, ReadPacket const & packet){
        if (code == Err::E_SUCCESS && packet.size > 0 && packet.addr != nullptr) {
            ++node2_read;
            result_message.assign(packet.buffer, packet.buffer + packet.size);
            node2.close();
        }
    });
    node2.set_onWrite([&](Err code){
        ++node2_write;
    });
    node2.set_onClose([&](){
        ++node2_close;
    });
    node2.start();

    node2.write(TEST_MESSAGE, sizeof(TEST_MESSAGE));

    ASSERT_EQ(Err::E_SUCCESS, loop.run());

    ASSERT_EQ(1, node1_read );
    ASSERT_EQ(1, node1_write);
    ASSERT_EQ(1, node1_close);

    ASSERT_EQ(1, node2_read );
    ASSERT_EQ(1, node2_write);
    ASSERT_EQ(1, node2_close);
}

