/**
 * @file   RawGeneratorTest.cpp
 * @brief  RawGenerator class tester.
 * @author zer0
 * @date   2019-07-02
 */

#include <gtest/gtest.h>
#include <libtbag/log/msg/RawGenerator.hpp>

using namespace libtbag;
using namespace libtbag::log;
using namespace libtbag::log::msg;

TEST(RawGeneratorTest, Default)
{
    RawGenerator gen(RawGenerator::LineFeedStyle::LFS_UNIX);
    ASSERT_STREQ("abcd\n", gen.make_string("A", 9, "U", "abcdefg", 4).c_str());
}

