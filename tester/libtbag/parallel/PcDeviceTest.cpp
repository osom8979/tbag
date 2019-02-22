/**
 * @file   PcDeviceTest.cpp
 * @brief  PcDevice class tester.
 * @author zer0
 * @date   2019-02-22
 */

#include <gtest/gtest.h>
#include <libtbag/parallel/PcDevice.hpp>

using namespace libtbag;
using namespace libtbag::parallel;

TEST(PcDeviceTest, Default)
{
    PcDevice object;
    ASSERT_TRUE(true);
}

