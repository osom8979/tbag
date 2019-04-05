/**
 * @file   UxUdpTest.cpp
 * @brief  UxUdp class tester.
 * @author zer0
 * @date   2018-12-07
 */

#include <gtest/gtest.h>
#include <libtbag/uvxx/UxLoop.hpp>
#include <libtbag/uvxx/UxUdp.hpp>
#include <libtbag/uvxx/UxIdle.hpp>

#include <libtbag/net/Ip.hpp>

#include <string>
#include <vector>

using namespace libtbag;
using namespace libtbag::uvxx;

TEST(UxUdpTest, Default)
{
    char const TEST_MESSAGE[] = "TEST_UDP_MESSAGE";

    UxUdp recv_udp;
    ASSERT_FALSE(recv_udp.isInit());

    UxUdp send_udp;
    ASSERT_FALSE(send_udp.isInit());

    UxIdle idle;
    ASSERT_FALSE(idle.isInit());

    UxLoop loop;
    ASSERT_TRUE(loop.empty());

    ASSERT_EQ(E_SUCCESS, recv_udp.init(loop));
    ASSERT_TRUE(recv_udp.isInit());
    ASSERT_FALSE(loop.empty());
    ASSERT_EQ(1, loop.size());

    ASSERT_EQ(E_SUCCESS, send_udp.init(loop));
    ASSERT_TRUE(send_udp.isInit());
    ASSERT_FALSE(loop.empty());
    ASSERT_EQ(2, loop.size());

    ASSERT_EQ(E_SUCCESS, idle.init(loop));
    ASSERT_TRUE(idle.isInit());
    ASSERT_FALSE(loop.empty());
    ASSERT_EQ(3, loop.size());

    sockaddr_in recv_ipv4;
    sockaddr_in send_ipv4;
    UxUdp::UdpSendRequest req;

    std::vector<char> recv_buffer;
    std::string recv_string;

    recv_udp.setOnAlloc([&](std::size_t suggested_size) -> libtbag::uvpp::binf {
        return libtbag::uvpp::defaultOnAlloc(recv_buffer, suggested_size);
    });
    recv_udp.setOnRecv([&](Err code, char const * buffer, std::size_t size, sockaddr const * addr, unsigned int flags){
        recv_string = std::string(buffer, buffer + size);
        recv_udp.close();
    });
    send_udp.setOnSend([&](UxUdp::UdpSendRequest & request, Err code){
        send_udp.close();
    });
    idle.setOnIdle([&](){
        send_udp.send(req, TEST_MESSAGE, sizeof(TEST_MESSAGE), &send_ipv4);
        idle.close();
    });

    std::string const ANY_IPV4      = libtbag::net::ANY_IPV4;
    std::string const LOOPBACK_IPV4 = libtbag::net::LOOPBACK_IPV4;

    ASSERT_EQ(E_SUCCESS, libtbag::uvpp::initAddress(ANY_IPV4, 0, &recv_ipv4));
    ASSERT_EQ(E_SUCCESS, recv_udp.bind(&recv_ipv4));
    ASSERT_EQ(E_SUCCESS, recv_udp.startRecv());

    int const BIND_PORT = recv_udp.getSockPort();
    std::cout << "Bind port: " << BIND_PORT << std::endl;
    ASSERT_EQ(E_SUCCESS, libtbag::uvpp::initAddress(LOOPBACK_IPV4, BIND_PORT, &send_ipv4));

    ASSERT_EQ(E_SUCCESS, idle.start());
    ASSERT_EQ(E_SUCCESS, loop.run());
    ASSERT_TRUE(loop.empty());

    ASSERT_STREQ(TEST_MESSAGE, recv_string.c_str());
}

