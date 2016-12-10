/**
 * @file   RandomTest.cpp
 * @brief  Random utilities tester.
 * @author zer0
 * @date   2016-04-07
 * @date   2016-12-10 (Move package: libtbag -> libtbag/random)
 */

#include <gtest/gtest.h>
#include <libtbag/random/Random.hpp>

using namespace libtbag;
using namespace libtbag::random;

TEST(RandomTest, Gen)
{
    int min = 0;
    int max = 10;
    int current = 0;
    int test_count = 200;

    for (int i = 0; i < test_count; ++i) {
        current = random::gen(min, max);
        ASSERT_LE(min, current);
        ASSERT_GE(max, current);
    }
}

