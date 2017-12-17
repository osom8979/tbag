/**
 * @file   AccelBackendTest.cpp
 * @brief  AccelBackend class tester.
 * @author zer0
 * @date   2017-12-17
 */

#include <gtest/gtest.h>
#include <libtbag/gpu/backend/AccelBackend.hpp>

using namespace libtbag;
using namespace libtbag::gpu;
using namespace libtbag::gpu::backend;

TEST(AccelBackendTest, Default)
{
    AccelBackend accel;
    if (accel.isSupport()) {
        ASSERT_LE(0, accel.getDeviceCount());
    }
}

