/**
 * @file   MaskingDeviceTest.cpp
 * @brief  MaskingDevice class tester.
 * @author zer0
 * @date   2017-06-15
 */

#include <gtest/gtest.h>
#include <libtbag/random/MaskingDevice.hpp>

using namespace libtbag;
using namespace libtbag::random;

TEST(MaskingDeviceTest, Default)
{
    MaskingDevice device;
    ASSERT_LT(0, device.gen());
}

