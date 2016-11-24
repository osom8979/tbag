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
    std::string utf8_string = "\xea\xb0\x80\xeb\x82\x98\xeb\x8b\xa4"; // Don't use u8 literal.
    std::string cp949_string = "\xb0\xa1\xb3\xaa\xb4\xd9";

    std::string utf8_result;
    std::string cp949_result;

    ASSERT_TRUE(convertFromUtf8(utf8_string, "cp949", cp949_result));
    ASSERT_EQ(cp949_string, cp949_result);

    ASSERT_TRUE(convertToUtf8(cp949_string, "cp949", utf8_result));
    ASSERT_EQ(utf8_string, utf8_result);
}

TEST(ConvertTest, ConvertFalse)
{
    std::string empty;
    ASSERT_FALSE(convertFromUtf8(empty, "unknown", empty));
    ASSERT_FALSE(convertToUtf8(empty, "unknown", empty));
}

