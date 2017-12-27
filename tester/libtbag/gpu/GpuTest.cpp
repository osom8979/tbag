/**
 * @file   GpuTest.cpp
 * @brief  Gpu class tester.
 * @author zer0
 * @date   2017-12-17
 */

#include <gtest/gtest.h>
#include <libtbag/gpu/Gpu.hpp>

using namespace libtbag;
using namespace libtbag::gpu;

// ---------------
namespace __impl {
// ---------------

template <typename Predicated>
static void run_if_supported(std::vector<Gpu::BackendType> const & types, Predicated predicated)
{
    for (auto & type : types) {
        Gpu device(type);
        if (device.isSupport()) {
            predicated(device);
        }
    }
}

template <typename Predicated>
static void run_all_if_supported(Predicated predicated)
{
    run_if_supported({Gpu::BackendType::GBT_CPU,
                      Gpu::BackendType::GBT_ACCEL,
                      Gpu::BackendType::GBT_CUDA,
                      Gpu::BackendType::GBT_OPENCL}, predicated);
}

// ------------------
} // namespace __impl
// ------------------

TEST(GpuTest, Information)
{
    __impl::run_all_if_supported([](Gpu & device){
        std::cout << "GPU Device type: " << device.getTypeString() << std::endl
                  << "Device count: " << device.getDeviceCount() << std::endl;
        for (auto & d : device.getDeviceList()) {
            for (auto & info : device.getPlatformInfo(d)) {
                std::cout << "[" << d.number << "]" << info.first << ": " << info.second << std::endl;
            }
        }
    });
}

