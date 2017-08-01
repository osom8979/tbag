/**
 * @file   CpuBackendTest.cpp
 * @brief  CpuBackend class tester.
 * @author zer0
 * @date   2017-08-01
 */

#include <gtest/gtest.h>
#include <libtbag/gpu/CpuBackend.hpp>

using namespace libtbag;
using namespace libtbag::gpu;

TEST(CpuBackendTest, Device)
{
    CpuBackend cpu;
    if (cpu.isSupport()) {
        ASSERT_LE(0, cpu.getDeviceCount());
    }
}



