/**
 * @file   DefaultGeneratorTest.cpp
 * @brief  DefaultGenerator class tester.
 * @author zer0
 * @date   2019-07-02
 */

#include <gtest/gtest.h>
#include <libtbag/log/msg/DefaultGenerator.hpp>
#include <libtbag/string/StringUtils.hpp>

using namespace libtbag;
using namespace libtbag::log;
using namespace libtbag::log::msg;

TEST(DefaultGeneratorTest, Default)
{
    DefaultGenerator gen(DefaultGenerator::LineFeedStyle::LFS_WINDOWS);
    auto const msg = gen.make_string("A", 9, "U", "abcdefg", 4);
    auto const tokens = libtbag::string::splitTokens(msg, " ");
    ASSERT_EQ(4, tokens.size());
    ASSERT_FALSE(tokens[0].empty());
    ASSERT_EQ('@', tokens[1][0]);
    ASSERT_STREQ("[U]", tokens[2].c_str());
    ASSERT_STREQ("abcd\r\n", tokens[3].c_str());
}

