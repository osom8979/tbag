/**
 * @file   CudaBackendTest.cpp
 * @brief  CudaBackend class tester.
 * @author zer0
 * @date   2017-08-01
 */

#include <gtest/gtest.h>
#include <libtbag/gpu/CudaBackend.hpp>
#include <iostream>

using namespace libtbag;
using namespace libtbag::gpu;

TEST(CudaBackendTest, Information)
{
    CudaBackend device;
    if (device.isSupport() == false) {
        return;
    }

    std::cout << "Supports CUDA\n"
              << " * Device count: " << device.getDeviceCount() << std::endl;
}

