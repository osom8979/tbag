/**
 * @file   DnsTest.cpp
 * @brief  Dns class tester.
 * @author zer0
 * @date   2017-05-03
 */

#include <gtest/gtest.h>
#include <libtbag/uvpp/Dns.hpp>
#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/uvpp/Tcp.hpp>
#include <libtbag/network/details/NetCommon.hpp>

#include <iostream>

using namespace libtbag;
using namespace libtbag::uvpp;

TEST(DnsTest, DnsAddrInfo)
{
    char const * TEST_DOMAIN_NAME = "localhost";
    Loop loop;

    //addrinfo hints = {0,};
    //hints.ai_family   = AF_INET;     // AF_INET/AF_INET6/AF_UNSPEC
    //hints.ai_socktype = SOCK_STREAM; // SOCK_STREAM/SOCK_DGRAM/SOCK_RAW
    //hints.ai_flags    = AI_PASSIVE;  // Fill in my IP for me.

    DnsAddrInfo addr;
    ASSERT_EQ(Err::E_SUCCESS, addr.requestAddrInfo(loop, TEST_DOMAIN_NAME));

    ASSERT_EQ(Err::E_SUCCESS, loop.run());
    ASSERT_NE(nullptr, addr.getAddrInfo());

    ASSERT_LT(0, addr.getAddrInfo()->ai_addrlen);
    struct addrinfo * info = addr.getAddrInfo()->ai_next;
    ASSERT_NE(nullptr, info);

    for (; info != nullptr; info = info->ai_next) {
        std::string ip = getIpName(info->ai_addr);
        std::cout << "Domain: " << TEST_DOMAIN_NAME << " -> Ip: " << ip << std::endl;

        using namespace libtbag::network::details;
        ASSERT_TRUE(isIpv4(ip) || isIpv6(ip));
    }
}

TEST(DnsTest, DnsNameInfo)
{
    char const * TEST_IP_NAME = "127.0.0.1";

    Loop loop;
    DnsNameInfo name;
    struct sockaddr_in addr = {0,};
    ASSERT_EQ(Err::E_SUCCESS, initAddress(TEST_IP_NAME, 0, &addr));
    ASSERT_EQ(Err::E_SUCCESS, name.requestNameInfo(loop, (sockaddr*)&addr, 0));

    ASSERT_EQ(Err::E_SUCCESS, loop.run());
    ASSERT_FALSE(name.getHost().empty()); // localhost/HostName/ETC ...
    std::cout << "Ip: " << TEST_IP_NAME << " -> Domain: " << name.getHost() << std::endl;
}

