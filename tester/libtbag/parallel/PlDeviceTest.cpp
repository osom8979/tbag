/**
 * @file   PlDeviceTest.cpp
 * @brief  PlDevice class tester.
 * @author zer0
 * @date   2019-02-21
 */

#include <gtest/gtest.h>
#include <libtbag/parallel/PlDevice.hpp>

using namespace libtbag;
using namespace libtbag::parallel;

TEST(PlDeviceTest, Default)
{
    PlDevice object;
    ASSERT_TRUE(true);
}

