/**
 * @file   RawTest.cpp
 * @brief  Raw class tester.
 * @author zer0
 * @date   2017-04-13
 */

#include <gtest/gtest.h>
#include <libtbag/io/Raw.hpp>

using namespace libtbag;
using namespace libtbag::io;

TEST(RawTest, Format)
{
    std::size_t const buffer_size = 256;
    char buffer[buffer_size + 1] = { 0, };

    ASSERT_TRUE(formatToMemory(buffer, buffer_size, "%s%d", "Test", 0));
    ASSERT_STREQ("Test0", buffer);
}

