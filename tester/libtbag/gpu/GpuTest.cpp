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
        std::cout << "GPU type: " << gpu->getTypeString() << std::endl;
        auto context = gpu->createContext(0, 0);
        ASSERT_FALSE(context.isUnknownContext());

        auto queue = gpu->createQueue(context);
        ASSERT_FALSE(queue.isUnknownQueue());
        ASSERT_TRUE(gpu->releaseQueue(queue));

        ASSERT_TRUE(gpu->releaseContext(context));
        ASSERT_TRUE(context.isUnknownContext());
    });
}

TEST(GpuTest, CreateMemory)
{
    __impl::run_all_if_supported([](UniqueGpu & gpu){
        std::cout << "GPU type: " << gpu->getTypeString() << std::endl;
        auto context = gpu->createContext(0, 0);
        ASSERT_FALSE(context.isUnknownContext());

        auto queue = gpu->createQueue(context);
        ASSERT_FALSE(queue.isUnknownQueue());

        char        const TEST_DATA[] = "TEST_DATA";
        std::size_t const TEST_SIZE   = sizeof(TEST_DATA);
        std::size_t const ALLOC_SIZE  = 1024;

        auto gpu_memory = gpu->malloc(context, ALLOC_SIZE);
        ASSERT_TRUE(gpu_memory.existsMemory());
        ASSERT_EQ(ALLOC_SIZE, gpu_memory.size);

        auto host_memory = gpu->mallocHost(context, ALLOC_SIZE);
        ASSERT_TRUE(host_memory.existsMemory());
        ASSERT_EQ(ALLOC_SIZE, host_memory.size);

        auto host_read = gpu->mallocHost(context, ALLOC_SIZE);
        ASSERT_TRUE(host_read.existsMemory());
        ASSERT_EQ(ALLOC_SIZE, host_read.size);

        ::memcpy(host_memory.data, TEST_DATA, TEST_SIZE);
        ASSERT_TRUE(gpu->write(queue, gpu_memory, host_memory, TEST_SIZE));
        ASSERT_TRUE(gpu->finish(queue));

        ASSERT_TRUE(gpu->read(queue, gpu_memory, host_read, TEST_SIZE));
        ASSERT_TRUE(gpu->finish(queue));

        std::string const READ_STRING((char*)host_read.data, TEST_SIZE);
        ASSERT_STREQ(TEST_DATA, READ_STRING.c_str());

        ASSERT_TRUE(gpu->free(gpu_memory));
        ASSERT_TRUE(gpu->freeHost(host_memory));
        ASSERT_TRUE(gpu->freeHost(host_read));

        ASSERT_TRUE(gpu->releaseQueue(queue));
        ASSERT_TRUE(queue.isUnknownQueue());

        ASSERT_TRUE(gpu->releaseContext(context));
        ASSERT_TRUE(context.isUnknownContext());
    });
}

