/**
 * @file   DefaultColorGeneratorTest.cpp
 * @brief  DefaultColorGenerator class tester.
 * @author zer0
 * @date   2019-07-02
 */

#include <gtest/gtest.h>
#include <libtbag/log/msg/DefaultColorGenerator.hpp>
#include <libtbag/tty/Tces.hpp>

#include <cstring>

using namespace libtbag;
using namespace libtbag::log;
using namespace libtbag::log::msg;

TEST(DefaultColorGeneratorTest, Default)
{
    DefaultColorGenerator gen(DefaultColorGenerator::LineFeedStyle::LFS_NONE);
    std::string msg;
    gen.make("A", 8, "D", "abcdefg", 4, msg);
    std::cout << "Generated message: " << msg << std::endl;
    auto const tokens = libtbag::string::splitTokens(msg, " ");
    ASSERT_EQ(4, tokens.size());
    ASSERT_EQ('D', tokens[0].back());
    ASSERT_FALSE(tokens[1].empty());
    ASSERT_EQ('@', tokens[2][0]);
    ASSERT_STREQ("abcd", tokens[3].substr(0, 4).c_str());
}

TEST(DefaultColorGeneratorTest, SmallBuffer)
{
    auto const LINE_FEED = DefaultColorGenerator::LineFeedStyle::LFS_WINDOWS;
    DefaultColorGenerator gen(LINE_FEED);
    std::string msg;
    gen.make("logger", 8, "debug", "msg", 3, msg);
    std::cout << "Generated message: " << msg << std::endl;

    auto const blue_fg_attribute_length = strlen(TBAG_TTY_DISPLAY_ATTRIBUTE_FG_BLUE);
    auto const reset_attribute_length = strlen(TBAG_TTY_DISPLAY_ATTRIBUTE_RESET);
    auto const line_feed = libtbag::string::getLineFeed(LINE_FEED);
    std::cout << "Blue fg attribute length: " << blue_fg_attribute_length/*5*/ << std::endl;
    std::cout << "Reset attribute length: " << reset_attribute_length/*4*/ << std::endl;
    std::cout << "LineFeed length: " << line_feed.size()/*2*/ << std::endl;
    ASSERT_EQ(0, ::strncmp(msg.c_str(), TBAG_TTY_DISPLAY_ATTRIBUTE_FG_BLUE, blue_fg_attribute_length));
    ASSERT_EQ(0, ::strncmp(msg.c_str()+msg.size()-line_feed.size(), line_feed.data(), line_feed.size()));
}

