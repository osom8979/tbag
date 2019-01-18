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
#include <libtbag/net/Ip.hpp>

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
    ASSERT_EQ(Err::E_SUCCESS, addr.requestAddrInfo(loop, TEST_DOMAIN_NAME, "http"));

    ASSERT_EQ(Err::E_SUCCESS, loop.run());
    ASSERT_NE(nullptr, addr.getAddrInfo());

    ASSERT_LT(0, addr.getAddrInfo()->ai_addrlen);
    struct addrinfo const * info = addr.getAddrInfo();
    ASSERT_NE(nullptr, info);

    bool exists_ipv4 = false;
    bool exists_ipv6 = false;

    for (; info != nullptr; info = info->ai_next) {
        std::string ip = getIpName(info->ai_addr);
        std::cout << "Domain: "  << TEST_DOMAIN_NAME  << " -> " << "Ip(" << ip << "), "
                  << "Protocol(" << info->ai_protocol << "), "
                  << "Socktype(" << info->ai_socktype << "), "
                  << "Family("   << info->ai_family   << "), "
                  << "Flags("    << info->ai_flags    << ")\n";

        if (info->ai_family == AF_INET) {
            exists_ipv4 = true;
        } else if (info->ai_family == AF_INET6) {
            exists_ipv6 = true;
        }

        ASSERT_TRUE(libtbag::net::isIpv4(ip) || libtbag::net::isIpv6(ip));
        ASSERT_EQ(80, getPortNumber(info->ai_addr));
    }

    auto * ipv4_addr = addr.findFirstIPv4();
    auto * ipv6_addr = addr.findFirstIPv6();

    if (exists_ipv4) {
        ASSERT_NE(nullptr, ipv4_addr);
        std::cout << "Found first IPv4: " << getIpName(ipv4_addr) << std::endl;
    } else {
        ASSERT_EQ(nullptr, ipv4_addr);
        std::cout << "Not found IPv4.\n";
    }

    if (exists_ipv6) {
        ASSERT_NE(nullptr, ipv6_addr);
        std::cout << "Found first IPv6: " << getIpName(ipv6_addr) << std::endl;
    } else {
        ASSERT_EQ(nullptr, ipv6_addr);
        std::cout << "Not found IPv6.\n";
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

TEST(DnsTest, RemoteServer)
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

        ASSERT_TRUE(libtbag::net::isIpv4(CURRENT_IP) || libtbag::net::isIpv6(CURRENT_IP));
        ip_list.push_back(CURRENT_IP);
    }

    ASSERT_LE(1U, ip_list.size());
    for (auto & ip : ip_list) {
        DnsNameInfo name;
        if (libtbag::net::isIpv4(ip)) {
            struct sockaddr_in addr = {0,};
            ASSERT_EQ(Err::E_SUCCESS, initAddress(ip.c_str(), 0, &addr));
            ASSERT_EQ(Err::E_SUCCESS, name.requestNameInfoWithSync(loop, (sockaddr*)&addr, 0));
        } else if (libtbag::net::isIpv6(ip)) {
            struct sockaddr_in6 addr = {0,};
            ASSERT_EQ(Err::E_SUCCESS, initAddress(ip.c_str(), 0, &addr));
            ASSERT_EQ(Err::E_SUCCESS, name.requestNameInfoWithSync(loop, (sockaddr*)&addr, 0));
        }

        ASSERT_FALSE(name.getHost().empty());
        std::cout << "Ip: " << ip << " -> Domain: " << name.getHost() << std::endl;
    }
}


