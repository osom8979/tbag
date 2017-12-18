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

    int const DEVICE_COUNT = device.getDeviceCount();

    std::cout << "Supports OpenCL" << std::endl;
    std::cout << " * Device count: " << DEVICE_COUNT << std::endl;
    for (int id = 0; id < DEVICE_COUNT; ++id) {
        std::cout << " * Device number: " << id << std::endl;
        for (auto & info : device.getPlatformInfo(id + 1)) {
            std::cout << " [" << id << "] " << info.first << ": " << info.second << std::endl;
        }
    }
}

