/**
 * @file   UdpTest.cpp
 * @brief  Udp class tester.
 * @author zer0
 * @date   2017-06-15
 */

#include <gtest/gtest.h>
#include <libtbag/uvpp/Udp.hpp>
#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/uvpp/func/FunctionalUdp.hpp>
#include <libtbag/uvpp/func/FunctionalIdle.hpp>
#include <libtbag/network/details/NetCommon.hpp>

#include <vector>

using namespace libtbag;
using namespace libtbag::uvpp;
using namespace libtbag::uvpp::func;
using namespace libtbag::network;
using namespace libtbag::network::details;

TEST(UdpTest, Default)
{
    char const TEST_MESSAGE[] = "TEST_UDP_MESSAGE";
    using Buffer = std::vector<char>;

    Loop loop;
    auto recv_udp = loop.newHandle<FuncUdp>(loop);
    auto send_udp = loop.newHandle<FuncUdp>(loop);
    auto idle = loop.newHandle<FuncIdle>(loop);

    sockaddr_in recv_ipv4;
    sockaddr_in send_ipv4;
    UdpSendRequest req;

    Buffer recv_buffer;
    std::string recv_string;
    recv_udp->setOnAlloc([&](std::size_t suggested_size) -> binf {
        return defaultOnAlloc(recv_buffer, suggested_size);
    });
    recv_udp->setOnRecv([&](Err code, char const * buffer, std::size_t size, sockaddr const * addr, unsigned int flags){
        if (size == 0 && addr == nullptr) {
            recv_udp->close();
        } else {
            recv_string = std::string(buffer, buffer + size);
        }
    });
    send_udp->setOnSend([&](UdpSendRequest & request, Err code){
        send_udp->close();
    });
    idle->setOnIdle([&](){
        send_udp->send(req, TEST_MESSAGE, sizeof(TEST_MESSAGE), &send_ipv4);
        idle->close();
    });

    ASSERT_EQ(Err::E_SUCCESS, initAddress(ANY_IPV4, 0, &recv_ipv4));
    ASSERT_EQ(Err::E_SUCCESS, recv_udp->bind(&recv_ipv4));
    ASSERT_EQ(Err::E_SUCCESS, recv_udp->startRecv());

    int const BIND_PORT = recv_udp->getSockPort();
    std::cout << "Bind port: " << BIND_PORT << std::endl;
    ASSERT_EQ(Err::E_SUCCESS, initAddress(LOOPBACK_IPV4, BIND_PORT, &send_ipv4));

    ASSERT_EQ(Err::E_SUCCESS, idle->start());
    ASSERT_EQ(Err::E_SUCCESS, loop.run());
    ASSERT_STREQ(TEST_MESSAGE, recv_string.c_str());
}

