/**
 * @file   OffsetOfTest.cpp
 * @brief  OffsetOf class tester.
 * @author zer0
 * @date   2020-01-12
 */

#include <gtest/gtest.h>
#include <libtbag/util/OffsetOf.hpp>

using namespace libtbag;

struct __offset_of_test
{
    std::uint32_t v0;
    std::uint32_t v1;
    double v2;
    float v3;
};

TEST(OffsetOfTest, Default)
{
    ASSERT_EQ( 0, (_TBAG_OFFSET_OF(__offset_of_test, v0)));
    ASSERT_EQ( 4, (_TBAG_OFFSET_OF(__offset_of_test, v1)));
    ASSERT_EQ( 8, (_TBAG_OFFSET_OF(__offset_of_test, v2)));
    ASSERT_EQ(16, (_TBAG_OFFSET_OF(__offset_of_test, v3)));

    ASSERT_EQ( 0, (TBAG_OFFSET_OF(__offset_of_test, v0)));
    ASSERT_EQ( 4, (TBAG_OFFSET_OF(__offset_of_test, v1)));
    ASSERT_EQ( 8, (TBAG_OFFSET_OF(__offset_of_test, v2)));
    ASSERT_EQ(16, (TBAG_OFFSET_OF(__offset_of_test, v3)));
}

