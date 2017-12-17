/**
 * @file   CudaBackendTest.cpp
 * @brief  CudaBackend class tester.
 * @author zer0
 * @date   2017-12-17
 */

#include <gtest/gtest.h>
#include <libtbag/gpu/backend/CudaBackend.hpp>

using namespace libtbag;
using namespace libtbag::gpu;
using namespace libtbag::gpu::backend;

TEST(CudaBackendTest, Information)
{
    CudaBackend device;
    if (device.isSupport() == false) {
        return;
    }

    std::cout << "Supports CUDA\n"
              << " * Device count: " << device.getDeviceCount() << std::endl;
}

