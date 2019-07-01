/**
 * @file   StringTest.cpp
 * @brief  String class tester.
 * @author zer0
 * @date   2016-11-23
 */

#include <gtest/gtest.h>
#include <libtbag/string/String.hpp>

#include <string>

using namespace libtbag;
using namespace libtbag::string;

TEST(StringTest, Default)
{
    rstr s0;
    ASSERT_TRUE(s0);

    rstr s1_move;
    ASSERT_TRUE(s1_move);
    auto const * s1_data = s1_move.get();
    rstr s2 = std::move(s1_move);
    ASSERT_TRUE(s2);
    auto const * s2_data = s2.get();
    ASSERT_EQ(s1_data, s2_data);

    rstr s3;
    rstr s4 = s3;
    ASSERT_TRUE(s3);
    ASSERT_TRUE(s4);
    auto const * s3_data = s3.get();
    auto const * s4_data = s4.get();
    ASSERT_EQ(s3_data, s4_data);

    rstr s5;
    ASSERT_TRUE(s5);
    rstr s6(nullptr);
    ASSERT_FALSE(s6);
    s6 = s5;
    auto const * s5_data = s5.get();
    auto const * s6_data = s6.get();
    ASSERT_EQ(s5_data, s6_data);
}


