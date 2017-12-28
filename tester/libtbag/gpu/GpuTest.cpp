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
static void run_if_supported(std::vector<GpuBackendType> const & types, Predicated predicated)
{
    for (auto & type : types) {
        auto context = createGpuContext(type);
        if (context && context->isSupport()) {
            predicated(context);
        }
    }
}

template <typename Predicated>
static void run_all_if_supported(Predicated predicated)
{
    run_if_supported({GpuBackendType::GBT_CPU,
                      GpuBackendType::GBT_ACCEL,
                      GpuBackendType::GBT_CUDA,
                      GpuBackendType::GBT_OPENCL}, predicated);
}

// ------------------
} // namespace __impl
// ------------------

TEST(GpuTest, Information)
{
    __impl::run_all_if_supported([](UniqueGpu & context){
        std::cout << "GPU Device type: " << context->getTypeString() << std::endl
                  << "Device count: " << context->getDeviceCount() << std::endl;
        for (auto & device : context->getDeviceList()) {
            for (auto & info : context->getPlatformInfo(device)) {
                std::cout << "[" << device.number << "]" << info.first << ": " << info.second << std::endl;
            }
        }
    });
}

