/**
 * @file   RawTest.cpp
 * @brief  Raw logging tester.
 * @author zer0
 * @date   2016-04-04
 */

#include <gtest/gtest.h>
#include <libtbag/log/details/Raw.hpp>

using namespace libtbag;
using namespace libtbag::log;
using namespace libtbag::log::details;

TEST(RawTest, format)
{
    std::size_t const buffer_size = 256;
    char buffer[buffer_size + 1] = { 0, };

    ASSERT_TRUE(formatToMemory(buffer, buffer_size, "%s%d", "Test", 0));
    ASSERT_STREQ(buffer, "Test0");
}

