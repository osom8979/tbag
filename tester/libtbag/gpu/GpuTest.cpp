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
            std::cout << "[" << plat.platform_id << "] profile: "      << plat_info.profile    << "\n"
                      << "[" << plat.platform_id << "] name: "         << plat_info.name       << "\n"
                      << "[" << plat.platform_id << "] vender: "       << plat_info.vendor     << "\n"
                      << "[" << plat.platform_id << "] version: "      << plat_info.version    << "\n"
                      << "[" << plat.platform_id << "] extensions: "   << plat_info.extensions << "\n"
                      << "[" << plat.platform_id << "] Device count: " << gpu->getDeviceCount(plat) << std::endl;
            for (auto & dev : gpu->getDeviceList(plat)) {
                auto dev_info = gpu->getDeviceInfo(dev);
                std::cout << "[" << plat.platform_id << "][" << dev.device_id << "] name: " << dev_info.name << std::endl;

                auto context = gpu->createContext(dev);
                std::cout << "[" << plat.platform_id << "][" << dev.device_id << "] Context: " << context.context_id << std::endl;
                ASSERT_TRUE(gpu->releaseContext(context));
                ASSERT_TRUE(context.isUnknownContext());
            }
        }
    });
}

TEST(GpuTest, CreateQueue)
{
    __impl::run_all_if_supported([](UniqueGpu & gpu){
        auto context = gpu->createContext(0, 0);
        ASSERT_FALSE(context.isUnknownContext());

        auto queue = gpu->createQueue(context);
        ASSERT_FALSE(queue.isUnknownQueue());
        ASSERT_TRUE(gpu->releaseQueue(queue));
    });
}

TEST(GpuTest, CreateMemory)
{
    __impl::run_all_if_supported([](UniqueGpu & gpu){
        auto context = gpu->createContext(0, 0);
        ASSERT_FALSE(context.isUnknownContext());

        std::size_t const ALLOC_SIZE = 1024;
        auto memory = gpu->malloc(context, ALLOC_SIZE);

        ASSERT_TRUE(memory.existsMemory());
        ASSERT_EQ(ALLOC_SIZE, memory.size);
        ASSERT_TRUE(gpu->free(memory));
    });
}

