/**
 * @file   ConvertTest.cpp
 * @brief  Convert class tester.
 * @author zer0
 * @date   2016-07-03
 */

#include <gtest/gtest.h>
#include <libtbag/text/Convert.hpp>

#include <string>
#include <vector>

using namespace libtbag;
using namespace libtbag::text;

TEST(ConvertTest, Default)
{
    std::string utf8_string = u8"가나다";
    std::string cp949_string = "\xb0\xa1\xb3\xaa\xb4\xd9";

    ASSERT_EQ(convertFromUtf8(utf8_string, "cp949"), cp949_string);
    ASSERT_THROW(text::convertFromUtf8(utf8_string, "unknown"), ConvertException);
    ASSERT_EQ(convertToUtf8(cp949_string, "cp949"), utf8_string);
}

