/**
 * @file   ConvertTest.cpp
 * @brief  Convert class tester.
 * @author zer0
 * @date   2016-07-03
 */

#include <gtest/gtest.h>
#include <libtbag/locale/Convert.hpp>

using namespace libtbag;
using namespace libtbag::locale;

TEST(ConvertTest, Default)
{
    // TEST STRING: "가나다"
    // "가" == "\xea\xb0\x80"
    // "나" == "\xeb\x82\x98"
    // "다" == "\xeb\x8b\xa4"
    std::string utf8_string = "\xea\xb0\x80\xeb\x82\x98\xeb\x8b\xa4"; // Don't use u8 literal.
    std::string windows949_string = "\xb0\xa1\xb3\xaa\xb4\xd9";

    std::string utf8_result;
    std::string windows949_result;

    ASSERT_TRUE(convertFromUtf8(utf8_string, "windows-949", windows949_result));
    ASSERT_EQ(windows949_string, windows949_result);

    ASSERT_TRUE(convertToUtf8(windows949_string, "windows-949", utf8_result));
    ASSERT_EQ(utf8_string, utf8_result);
}

TEST(ConvertTest, Default2)
{
    std::string utf8_string = "\\"; // "\x5c"
    std::string windows949_string = "\\";

    std::string utf8_result;
    std::string windows949_result;

    ASSERT_TRUE(convertToUtf8(windows949_string, "windows-949", utf8_result));
    ASSERT_EQ(utf8_string, utf8_result);

    ASSERT_TRUE(convertFromUtf8(utf8_string, "windows-949", windows949_result));
    ASSERT_EQ(windows949_string, windows949_result);
}

TEST(ConvertTest, ConvertFalse)
{
    std::string empty;
    ASSERT_FALSE(convertFromUtf8(empty, "unknown", empty));
    ASSERT_FALSE(convertToUtf8(empty, "unknown", empty));
}

