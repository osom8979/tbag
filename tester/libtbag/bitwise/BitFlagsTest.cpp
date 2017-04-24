/**
 * @file   BitFlagsTest.cpp
 * @brief  BitFlags class tester.
 * @author zer0
 * @date   2017-04-24
 */

#include <gtest/gtest.h>
#include <libtbag/bitwise/BitFlags.hpp>
#include <libtbag/uvpp/UvCommon.hpp>

#include <cstdint>
#include <array>

using namespace libtbag;
using namespace libtbag::bitwise;

TEST(BitFlagsTest, OrderTest)
{
    ASSERT_TRUE(true);
}

TEST(BitFlagsTest, Default)
{
    uint32_t host = 1;
    uint32_t net  = ntohl(host);
    ASSERT_NE(host, net);

    std::array<char, 8> buffer;
    *((uint32_t*)(buffer.data() + 3)) = net;
    //((uint32_t)(*(buffer.data() + 3))) = net;

    uint32_t conv = ntohl(*((uint32_t*)(buffer.data() + 3)));
    ASSERT_EQ(host, conv);

    ASSERT_TRUE(true);
}

