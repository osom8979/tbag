/**
 * @file   OpenCLBackendTest.cpp
 * @brief  OpenCLBackend class tester.
 * @author zer0
 * @date   2017-12-17
 */

#include <gtest/gtest.h>
#include <libtbag/gpu/backend/OpenCLBackend.hpp>

using namespace libtbag;
using namespace libtbag::gpu;
using namespace libtbag::gpu::backend;

TEST(OpenCLBackendTest, Information)
{
    OpenCLBackend device;
    if (device.isSupport() == false) {
        return;
    }

    std::cout << "Supports OpenCL\n"
              << " * Device count: " << device.getDeviceCount() << std::endl;
}

