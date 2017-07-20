/**
 * @file   ClampTest.cpp
 * @brief  Clamp class tester.
 * @author zer0
 * @date   2017-07-20
 */

#include <gtest/gtest.h>
#include <libtbag/algorithm/Clamp.hpp>

using namespace libtbag;
using namespace libtbag::algorithm;

TEST(ClampTest, Default)
{
    ASSERT_EQ(200, algorithm::clamp(100, 200, 500));
    ASSERT_EQ(500, algorithm::clamp(600, 200, 500));
}

