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
    auto const msg = gen.make_string("A", 1, "U", "abcdefg", 4);
    std::cout << "Generated message: " << msg << std::endl;
    auto const tokens = libtbag::string::splitTokens(msg, " ");
    ASSERT_EQ(4, tokens.size());
    ASSERT_EQ('M', tokens[0][0]);
    ASSERT_FALSE(tokens[1].empty());
    ASSERT_EQ('@', tokens[2][0]);
    ASSERT_STREQ("abcd\r\n", tokens[3].c_str());
}

