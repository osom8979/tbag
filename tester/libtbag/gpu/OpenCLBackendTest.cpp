/**
 * @file   OpenCLBackendTest.cpp
 * @brief  OpenCLBackend class tester.
 * @author zer0
 * @date   2017-04-18
 */

#include <gtest/gtest.h>
#include <libtbag/gpu/OpenCLBackend.hpp>

using namespace libtbag;
using namespace libtbag::gpu;

TEST(OpenCLBackendTest, Default)
{
#if defined(USE_OPENCL)
    ASSERT_TRUE(OpenCLBackend().isSupport());
#else
    ASSERT_FALSE(OpenCLBackend().isSupport());
#endif
}

TEST(OpenCLBackendTest, Device)
{
    OpenCLBackend cl;
    if (cl.isSupport()) {
        ASSERT_LE(0, cl.getDeviceCount());
    }
}

