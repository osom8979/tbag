/**
 * @file   DefaultColorGeneratorTest.cpp
 * @brief  DefaultColorGenerator class tester.
 * @author zer0
 * @date   2019-07-02
 */

#include <gtest/gtest.h>
#include <libtbag/log/msg/DefaultColorGenerator.hpp>

using namespace libtbag;
using namespace libtbag::log;
using namespace libtbag::log::msg;

TEST(DefaultColorGeneratorTest, Default)
{
    DefaultColorGenerator gen(DefaultColorGenerator::LineFeedStyle::LFS_NONE);
    auto const msg = gen.make_string("A", 8, "D", "abcdefg", 4);
    std::cout << "Generated message: " << msg << std::endl;
    auto const tokens = libtbag::string::splitTokens(msg, " ");
    ASSERT_EQ(4, tokens.size());
    ASSERT_EQ('D', tokens[0].back());
    ASSERT_FALSE(tokens[1].empty());
    ASSERT_EQ('@', tokens[2][0]);
    ASSERT_STREQ("abcd", tokens[3].substr(0, 4).c_str());
}

