/**
 * @file   BufferInfoTest.cpp
 * @brief  BufferInfo class tester.
 * @author zer0
 * @date   2017-09-29
 */

#include <gtest/gtest.h>
#include <libtbag/util/BufferInfo.hpp>

#include <vector>

using namespace libtbag;
using namespace libtbag::util;

TEST(BufferInfoTest, Coverage)
{
    Buffer buffer;
    binf info1;
    binf info2(nullptr, 0);
    binf info3(buffer);

    std::vector<int> int_buffer;

    info1.set(nullptr, 0);
    info1.set(int_buffer);
}

TEST(BufferInfoTest, Default)
{
    std::vector<uint8_t> buffer = { 0x01, 0x02 };
    binf info(buffer);

    ASSERT_NE(nullptr, info.buffer);
    ASSERT_EQ(2, info.size);
}

