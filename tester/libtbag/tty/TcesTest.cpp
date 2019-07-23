/**
 * @file   TcesTest.cpp
 * @brief  Tces class tester.
 * @author zer0
 * @date   2019-07-23
 */

#include <gtest/gtest.h>
#include <libtbag/tty/Tces.hpp>

#include <string>

using namespace libtbag;
using namespace libtbag::tty;

TEST(TcesTest, CharString)
{
    ASSERT_FALSE(std::string(DISPLAY_ATTRIBUTE_RESET).empty());
    ASSERT_FALSE(std::string(DISPLAY_ATTRIBUTE_BRIGHT).empty());
    ASSERT_FALSE(std::string(DISPLAY_ATTRIBUTE_DIM).empty());
    ASSERT_FALSE(std::string(DISPLAY_ATTRIBUTE_UNDERSCORE).empty());
    ASSERT_FALSE(std::string(DISPLAY_ATTRIBUTE_BLINK).empty());
    ASSERT_FALSE(std::string(DISPLAY_ATTRIBUTE_REVERSE).empty());
    ASSERT_FALSE(std::string(DISPLAY_ATTRIBUTE_HIDDEN).empty());
    ASSERT_FALSE(std::string(DISPLAY_ATTRIBUTE_FG_BLACK).empty());
    ASSERT_FALSE(std::string(DISPLAY_ATTRIBUTE_FG_RED).empty());
    ASSERT_FALSE(std::string(DISPLAY_ATTRIBUTE_FG_GREEN).empty());
    ASSERT_FALSE(std::string(DISPLAY_ATTRIBUTE_FG_YELLOW).empty());
    ASSERT_FALSE(std::string(DISPLAY_ATTRIBUTE_FG_BLUE).empty());
    ASSERT_FALSE(std::string(DISPLAY_ATTRIBUTE_FG_MAGENTA).empty());
    ASSERT_FALSE(std::string(DISPLAY_ATTRIBUTE_FG_CYAN).empty());
    ASSERT_FALSE(std::string(DISPLAY_ATTRIBUTE_FG_WHITE).empty());
    ASSERT_FALSE(std::string(DISPLAY_ATTRIBUTE_BG_BLACK).empty());
    ASSERT_FALSE(std::string(DISPLAY_ATTRIBUTE_BG_RED).empty());
    ASSERT_FALSE(std::string(DISPLAY_ATTRIBUTE_BG_GREEN).empty());
    ASSERT_FALSE(std::string(DISPLAY_ATTRIBUTE_BG_YELLOW).empty());
    ASSERT_FALSE(std::string(DISPLAY_ATTRIBUTE_BG_BLUE).empty());
    ASSERT_FALSE(std::string(DISPLAY_ATTRIBUTE_BG_MAGENTA).empty());
    ASSERT_FALSE(std::string(DISPLAY_ATTRIBUTE_BG_CYAN).empty());
    ASSERT_FALSE(std::string(DISPLAY_ATTRIBUTE_BG_WHITE).empty());
}

TEST(TcesTest, WideCharString)
{
    ASSERT_FALSE(std::wstring(DISPLAY_ATTRIBUTE_RESET_WIDE).empty());
    ASSERT_FALSE(std::wstring(DISPLAY_ATTRIBUTE_BRIGHT_WIDE).empty());
    ASSERT_FALSE(std::wstring(DISPLAY_ATTRIBUTE_DIM_WIDE).empty());
    ASSERT_FALSE(std::wstring(DISPLAY_ATTRIBUTE_UNDERSCORE_WIDE).empty());
    ASSERT_FALSE(std::wstring(DISPLAY_ATTRIBUTE_BLINK_WIDE).empty());
    ASSERT_FALSE(std::wstring(DISPLAY_ATTRIBUTE_REVERSE_WIDE).empty());
    ASSERT_FALSE(std::wstring(DISPLAY_ATTRIBUTE_HIDDEN_WIDE).empty());
    ASSERT_FALSE(std::wstring(DISPLAY_ATTRIBUTE_FG_BLACK_WIDE).empty());
    ASSERT_FALSE(std::wstring(DISPLAY_ATTRIBUTE_FG_RED_WIDE).empty());
    ASSERT_FALSE(std::wstring(DISPLAY_ATTRIBUTE_FG_GREEN_WIDE).empty());
    ASSERT_FALSE(std::wstring(DISPLAY_ATTRIBUTE_FG_YELLOW_WIDE).empty());
    ASSERT_FALSE(std::wstring(DISPLAY_ATTRIBUTE_FG_BLUE_WIDE).empty());
    ASSERT_FALSE(std::wstring(DISPLAY_ATTRIBUTE_FG_MAGENTA_WIDE).empty());
    ASSERT_FALSE(std::wstring(DISPLAY_ATTRIBUTE_FG_CYAN_WIDE).empty());
    ASSERT_FALSE(std::wstring(DISPLAY_ATTRIBUTE_FG_WHITE_WIDE).empty());
    ASSERT_FALSE(std::wstring(DISPLAY_ATTRIBUTE_BG_BLACK_WIDE).empty());
    ASSERT_FALSE(std::wstring(DISPLAY_ATTRIBUTE_BG_RED_WIDE).empty());
    ASSERT_FALSE(std::wstring(DISPLAY_ATTRIBUTE_BG_GREEN_WIDE).empty());
    ASSERT_FALSE(std::wstring(DISPLAY_ATTRIBUTE_BG_YELLOW_WIDE).empty());
    ASSERT_FALSE(std::wstring(DISPLAY_ATTRIBUTE_BG_BLUE_WIDE).empty());
    ASSERT_FALSE(std::wstring(DISPLAY_ATTRIBUTE_BG_MAGENTA_WIDE).empty());
    ASSERT_FALSE(std::wstring(DISPLAY_ATTRIBUTE_BG_CYAN_WIDE).empty());
    ASSERT_FALSE(std::wstring(DISPLAY_ATTRIBUTE_BG_WHITE_WIDE).empty());
}

