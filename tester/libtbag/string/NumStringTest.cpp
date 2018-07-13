/**
 * @file   NumStringTest.cpp
 * @brief  NumString class tester.
 * @author zer0
 * @date   2018-07-14
 */

#include <gtest/gtest.h>
#include <libtbag/string/NumString.hpp>

using namespace libtbag;
using namespace libtbag::string;

TEST(NumStringTest, Default)
{
    NumString val;
    val(10);
    ASSERT_EQ(10, val.get<int>());

    val(10.5);
    ASSERT_EQ(10.5, val.get<float>());

    NumString val1(10);
    NumString val2(10.5);
    NumString val3 = val1;
    ASSERT_EQ((  int)val1, (  int)val3);
    ASSERT_EQ((float)val1, (float)val3);
    ASSERT_EQ(( bool)val1, ( bool)val3);
    ASSERT_NE((float)val2, (float)val3);
    ASSERT_EQ((  int)val2, (  int)val3);
}

TEST(NumStringTest, String)
{
    NumString val;
    val("10");
    ASSERT_EQ("10", val.get());
    ASSERT_EQ(10, val.get<int>());

    val(std::string("10.5"));
    ASSERT_EQ("10.5", val.get());
    ASSERT_EQ(10.5, val.get<float>());
}

TEST(NumStringTest, Operator)
{
    NumString val;
    val(10);
    ASSERT_EQ(10, (int)val);
}

