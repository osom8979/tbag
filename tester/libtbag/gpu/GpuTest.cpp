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
        std::cout << "GPU type: " << context->getTypeString() << "\n"
                  << "Platform count: " << context->getPlatformCount() << std::endl;
        for (auto & plat : context->getPlatformList()) {
            std::cout << "[" << plat.number << "] profile: "    << plat.profile    << "\n"
                      << "[" << plat.number << "] name: "       << plat.name       << "\n"
                      << "[" << plat.number << "] vender: "     << plat.vendor     << "\n"
                      << "[" << plat.number << "] version: "    << plat.version    << "\n"
                      << "[" << plat.number << "] extensions: " << plat.extensions << std::endl;
        }
    });
}

