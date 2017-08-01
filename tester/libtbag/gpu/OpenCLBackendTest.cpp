/**
 * @file   OpenCLBackendTest.cpp
 * @brief  OpenCLBackend class tester.
 * @author zer0
 * @date   2017-04-18
 */

#include <gtest/gtest.h>
#include <libtbag/gpu/OpenCLBackend.hpp>
#include <iostream>

using namespace libtbag;
using namespace libtbag::gpu;

TEST(OpenCLBackendTest, Information)
{
    OpenCLBackend device;
    if (device.isSupport() == false) {
        return;
    }

    std::cout << "Supports OpenCL\n"
              << " * Device count: " << device.getDeviceCount() << std::endl;
}

