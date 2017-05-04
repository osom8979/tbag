/**
 * @file   NetCommonTest.cpp
 * @brief  NetCommon class tester.
 * @author zer0
 * @date   2017-05-04
 */

#include <gtest/gtest.h>
#include <libtbag/network/details/NetCommon.hpp>

using namespace libtbag;
using namespace libtbag::network;
using namespace libtbag::network::details;

TEST(NetCommonTest, Ipv4)
{
    ASSERT_FALSE(isIpv4("255.255.256.250"));
    ASSERT_FALSE(isIpv4("255,255.256.250"));
    ASSERT_FALSE(isIpv4(".255.0.250"));

    ASSERT_TRUE(isIpv4("255.255.255.255"));
    ASSERT_TRUE(isIpv4("0.0.0.0"));
    ASSERT_TRUE(isIpv4("0.09.0.00"));
    ASSERT_TRUE(isIpv4("127.0.0.1"));
}

TEST(NetCommonTest, Ipv6)
{
    ASSERT_TRUE(isIpv6("2001:cdba:0000:0000:0000:0000:3257:9652"));
    ASSERT_TRUE(isIpv6("2001:cdba:0:0:0:0:3257:9652"));
    ASSERT_TRUE(isIpv6("2001:cdba::3257:9652"));

    ASSERT_TRUE(isIpv6("1:2:3:4:5:6:7:8"));

    ASSERT_TRUE(isIpv6("1::"));
    ASSERT_TRUE(isIpv6("1:2:3:4:5:6:7::"));

    ASSERT_TRUE(isIpv6("1::8"));
    ASSERT_TRUE(isIpv6("1:2:3:4:5:6::8"));

    ASSERT_TRUE(isIpv6("1::7:8"));
    ASSERT_TRUE(isIpv6("1:2:3:4:5::7:8"));
    ASSERT_TRUE(isIpv6("1:2:3:4:5::8"));

    ASSERT_TRUE(isIpv6("1::6:7:8"));
    ASSERT_TRUE(isIpv6("1:2:3:4::6:7:8"));
    ASSERT_TRUE(isIpv6("1:2:3:4::8"));

    ASSERT_TRUE(isIpv6("1::5:6:7:8"));
    ASSERT_TRUE(isIpv6("1:2:3::5:6:7:8"));
    ASSERT_TRUE(isIpv6("1:2:3::8"));

    ASSERT_TRUE(isIpv6("1::4:5:6:7:8"));
    ASSERT_TRUE(isIpv6("1:2::4:5:6:7:8"));
    ASSERT_TRUE(isIpv6("1:2::8"));

    ASSERT_TRUE(isIpv6("1:2::4:5:6:7:8"));
    ASSERT_TRUE(isIpv6("1::3:4:5:6:7:8"));
    ASSERT_TRUE(isIpv6("1::8"));

    ASSERT_TRUE(isIpv6("::2:3:4:5:6:7:8"));
    ASSERT_TRUE(isIpv6("::8"));
    ASSERT_TRUE(isIpv6("::"));

    ASSERT_TRUE(isIpv6("fe80::7:8%eth0"));
    ASSERT_TRUE(isIpv6("fe80::7:8%1"));

    ASSERT_TRUE(isIpv6("::255.255.255.255"));
    ASSERT_TRUE(isIpv6("::ffff:255.255.255.255"));
    ASSERT_TRUE(isIpv6("::ffff:0:255.255.255.255"));

    ASSERT_TRUE(isIpv6("2001:db8:3:4::192.0.2.33"));
    ASSERT_TRUE(isIpv6("64:ff9b::192.0.2.33"));
}

