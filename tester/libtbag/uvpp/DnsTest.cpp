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
#include <libtbag/string/StringUtils.hpp>

#include <iostream>

using namespace libtbag;
using namespace libtbag::uvpp;

TEST(DnsTest, DnsAddrInfo)
{
    Loop loop;

    DnsAddrInfo addr;
    addrinfo hints = {0,};
    hints.ai_family   = AF_INET;     // AF_INET/AF_INET6/AF_UNSPEC
    hints.ai_socktype = SOCK_STREAM; // SOCK_STREAM/SOCK_DGRAM/SOCK_RAW
    hints.ai_flags    = AI_PASSIVE;  // Fill in my IP for me.
    ASSERT_EQ(Err::E_SUCCESS, addr.requestAddrInfo(loop, "localhost", "", &hints));

    ASSERT_EQ(Err::E_SUCCESS, loop.run());
    ASSERT_NE(nullptr, addr.getAddrInfo());

    for (struct addrinfo * info = addr.getAddrInfo()->ai_next; info != nullptr; info = info->ai_next) {
        ASSERT_TRUE(string::isMatch(getIpName(info->ai_addr), "127\\.0\\.0\\.[0-9]{1,3}"));
    }
}

TEST(DnsTest, DnsNameInfo)
{
    Loop loop;
    DnsNameInfo name;
    struct sockaddr_in addr = {0,};
    ASSERT_EQ(Err::E_SUCCESS, initAddress("127.0.0.1", 0, &addr));
    ASSERT_EQ(Err::E_SUCCESS, name.requestNameInfo(loop, (sockaddr*)&addr, 0));

    ASSERT_EQ(Err::E_SUCCESS, loop.run());
    ASSERT_FALSE(name.getHost().empty()); // localhost/HostName/ETC ...
    std::cout << "Host name: " << name.getHost() << std::endl;
}

