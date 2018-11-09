/**
 * @file   PowerTest.cpp
 * @brief  Power class tester.
 * @author zer0
 * @date   2016-04-20
 */

#include <gtest/gtest.h>
#include <libtbag/tmp/Power.hpp>

using namespace libtbag;
using namespace libtbag::tmp;

TEST(PowerTest, Default)
{
    int64_t test_value = 0;

    test_value = Power<7, 0>::value;
    ASSERT_EQ(1, test_value);

    test_value = Power<7, 1>::value;
    ASSERT_EQ(7, test_value);

    test_value = Power<7, 2>::value;
    ASSERT_EQ(49, test_value);

    test_value = Power<7, 3>::value;
    ASSERT_EQ(343, test_value);
}

TEST(PowerTest, IsPowerOf2)
{
    ASSERT_TRUE(is_power_of_2<1>::value);
    ASSERT_TRUE(is_power_of_2<2>::value);
    ASSERT_TRUE(is_power_of_2<4>::value);
    ASSERT_TRUE(is_power_of_2<8>::value);
    ASSERT_TRUE(is_power_of_2<16>::value);
    ASSERT_TRUE(is_power_of_2<32>::value);
    ASSERT_TRUE(is_power_of_2<64>::value);
    ASSERT_TRUE(is_power_of_2<128>::value);

    ASSERT_FALSE(is_power_of_2<3>::value);
    ASSERT_FALSE(is_power_of_2<5>::value);
    ASSERT_FALSE(is_power_of_2<7>::value);
    ASSERT_FALSE(is_power_of_2<10>::value);
    ASSERT_FALSE(is_power_of_2<17>::value);
    ASSERT_FALSE(is_power_of_2<30>::value);
    ASSERT_FALSE(is_power_of_2<80>::value);
    ASSERT_FALSE(is_power_of_2<140>::value);
}

