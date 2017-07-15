/**
 * @file   NetworkDnsTest.cpp
 * @brief  NetworkDns class tester.
 * @author zer0
 * @date   2017-05-19
 */

#include <gtest/gtest.h>
#include <libtbag/uvpp/Dns.hpp>
#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/uvpp/Tcp.hpp>
#include <libtbag/network/details/NetCommon.hpp>

using namespace libtbag;
using namespace libtbag::uvpp;
using namespace libtbag::network;
using namespace libtbag::network::details;

TEST(NetworkDnsTest, Sync)
{
    char const * TEST_DOMAIN_NAME = "osom8979.github.io";
    Loop loop;

    DnsAddrInfo addr;
    ASSERT_EQ(Err::E_SUCCESS, addr.requestAddrInfoWithSync(loop, TEST_DOMAIN_NAME));
    ASSERT_NE(nullptr, addr.getAddrInfo());

    ASSERT_LT(0, addr.getAddrInfo()->ai_addrlen);
    struct addrinfo * info = addr.getAddrInfo()->ai_next;
    ASSERT_NE(nullptr, info);

    std::vector<std::string> ip_list;
    for (; info != nullptr; info = info->ai_next) {
        std::string const CURRENT_IP = getIpName(info->ai_addr);
        std::cout << "Domain: " << TEST_DOMAIN_NAME << " -> Ip: " << CURRENT_IP << std::endl;

        ASSERT_TRUE(isIpv4(CURRENT_IP) || isIpv6(CURRENT_IP));
        ip_list.push_back(CURRENT_IP);
    }

    ASSERT_LE(1U, ip_list.size());
    for (auto & ip : ip_list) {
        DnsNameInfo name;
        if (isIpv4(ip)) {
            struct sockaddr_in addr = {0,};
            ASSERT_EQ(Err::E_SUCCESS, initAddress(ip.c_str(), 0, &addr));
            ASSERT_EQ(Err::E_SUCCESS, name.requestNameInfoWithSync(loop, (sockaddr*)&addr, 0));
        } else if (isIpv6(ip)) {
            struct sockaddr_in6 addr = {0,};
            ASSERT_EQ(Err::E_SUCCESS, initAddress(ip.c_str(), 0, &addr));
            ASSERT_EQ(Err::E_SUCCESS, name.requestNameInfoWithSync(loop, (sockaddr*)&addr, 0));
        }

        ASSERT_FALSE(name.getHost().empty());
        std::cout << "Ip: " << ip << " -> Domain: " << name.getHost() << std::endl;
    }
}


