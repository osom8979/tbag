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
        auto gpu = createGpuContext(type);
        if (gpu && gpu->isSupport()) {
            predicated(gpu);
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
    __impl::run_all_if_supported([](UniqueGpu & gpu){
        std::cout << "GPU type: " << gpu->getTypeString() << "\n"
                  << "Platform count: " << gpu->getPlatformCount() << std::endl;
        for (auto & plat : gpu->getPlatformList()) {
            auto plat_info = gpu->getPlatformInfo(plat);
            std::cout << "[" << plat.platform_number << "] profile: "      << plat_info.profile    << "\n"
                      << "[" << plat.platform_number << "] name: "         << plat_info.name       << "\n"
                      << "[" << plat.platform_number << "] vender: "       << plat_info.vendor     << "\n"
                      << "[" << plat.platform_number << "] version: "      << plat_info.version    << "\n"
                      << "[" << plat.platform_number << "] extensions: "   << plat_info.extensions << "\n"
                      << "[" << plat.platform_number << "] Device count: " << gpu->getDeviceCount(plat) << std::endl;
            for (auto & dev : gpu->getDeviceList(plat)) {
                auto dev_info = gpu->getDeviceInfo(dev);
                std::cout << "[" << plat.platform_number << "][" << dev.device_number << "] name: " << dev_info.name << std::endl;
            }
        }
    });
}

