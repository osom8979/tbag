/**
 * @file   GpuBackendTest.cpp
 * @brief  GpuBackend class tester.
 * @author zer0
 * @date   2017-08-01
 */

#include <gtest/gtest.h>
#include <libtbag/gpu/GpuBackend.hpp>

using namespace libtbag;
using namespace libtbag::gpu;

TEST(GpuBackendTest, Device)
{
    auto gpu = createSharedGpu();
    ASSERT_TRUE(static_cast<bool>(gpu));
    ASSERT_NE(BackendType::BT_UNKNOWN, gpu->getType());
    ASSERT_TRUE(gpu->isSupport());
    ASSERT_LT(0, gpu->getDeviceCount());
}

