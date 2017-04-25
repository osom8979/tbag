/**
 * @file   BitFlagsTest.cpp
 * @brief  BitFlags class tester.
 * @author zer0
 * @date   2017-04-24
 */

#include <gtest/gtest.h>
#include <libtbag/bitwise/BitFlags.hpp>
#include <libtbag/bitwise/ByteUtils.hpp>
#include <libtbag/bitwise/Endian.hpp>

#include <cstdint>
#include <array>

using namespace libtbag;
using namespace libtbag::bitwise;

TEST(BitFlagsTest, Default)
{
    uint32_t host = 1;
    uint32_t net  = toNetwork(host);
    ASSERT_NE(host, net);

    std::array<char, 8> buffer;
    bitwise::assign(buffer.data() + 3, net);

    uint32_t temp;
    bitwise::obtain(buffer.data() + 3, &temp);

    uint32_t conv = toHost(temp);
    ASSERT_EQ(host, conv);

    ASSERT_TRUE(true);
}

