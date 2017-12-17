/**
 * @file   CpuBackendTest.cpp
 * @brief  CpuBackend class tester.
 * @author zer0
 * @date   2017-12-17
 */

#include <gtest/gtest.h>
#include <libtbag/gpu/backend/CpuBackend.hpp>

using namespace libtbag;
using namespace libtbag::gpu;
using namespace libtbag::gpu::backend;

TEST(CpuBackendTest, Default)
{
    CpuBackend cpu;
    if (cpu.isSupport()) {
        ASSERT_LE(0, cpu.getDeviceCount());
    }
}

