/**
 * @file   RandomTest.cpp
 * @brief  Random class tester.
 * @author zer0
 * @date   2016-04-07
 */

#include <gtest/gtest.h>
#include <libtbag/Random.hpp>

using namespace libtbag;

TEST(RandomTest, gen)
{
    int min = 0;
    int max = 10;
    int current = 0;
    int test_count = 200;

    for (int i = 0; i < test_count; ++i) {
        current = Random::gen(min, max);
        ASSERT_GE(current, min);
        ASSERT_LE(current, max);
    }
}

