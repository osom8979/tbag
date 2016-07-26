/**
 * @file   TimeIdTest.cpp
 * @brief  TimeId class tester.
 * @author zer0
 * @date   2016-07-26
 */

#include <gtest/gtest.h>
#include <libtbag/id/generator/TimeId.hpp>

using namespace libtbag;
using namespace libtbag::id;
using namespace libtbag::id::generator;

TEST(TimeIdTest, Default)
{
    int const TEST_COUNT = 1000;
    for (int i = 0; i < TEST_COUNT; ++i) {
        ASSERT_NE(genTimeId(true), genTimeId(true));
        ASSERT_NE(genTimeId(false), genTimeId(false));
    }
}

