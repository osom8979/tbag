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

TEST(TimeIdTest, GenTimeId_Sleep)
{
    int const TEST_COUNT = 100;
    for (int i = 0; i < TEST_COUNT; ++i) {
        ASSERT_NE(genTimeId(true), genTimeId(true));
    }
}

TEST(TimeIdTest, GenTimeId_Nosleep)
{
    int const TEST_COUNT = 100;
    for (int i = 0; i < TEST_COUNT; ++i) {
        ASSERT_NE(genTimeId(false), genTimeId(false));
    }
}

