/**
 * @file   ParallelDeviceTest.cpp
 * @brief  ParallelDevice class tester.
 * @author zer0
 * @date   2019-02-22
 */

#include <gtest/gtest.h>
#include <libtbag/parallel/ParallelDevice.hpp>

using namespace libtbag;
using namespace libtbag::parallel;

TEST(ParallelDevice, Default)
{
    ParallelDevice object;
    ASSERT_TRUE(true);
}

