/**
 * @file   GpuTest.cpp
 * @brief  Gpu class tester.
 * @author zer0
 * @date   2017-12-17
 */

#include <gtest/gtest.h>
#include <libtbag/gpu/Gpu.hpp>
#include <libtbag/log/Log.hpp>

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
            std::cout << "Platform ID: "    << plat.platform_id     << "\n"
                      << "* profile: "      << plat_info.profile    << "\n"
                      << "* name: "         << plat_info.name       << "\n"
                      << "* vender: "       << plat_info.vendor     << "\n"
                      << "* version: "      << plat_info.version    << "\n"
                      << "* extensions: "   << plat_info.extensions << "\n"
                      << "* Device count: " << gpu->getDeviceCount(plat) << std::endl;
            for (auto & dev : gpu->getDeviceList(plat)) {
                auto dev_info = gpu->getDeviceInfo(dev);
                std::cout << "* Device ID: "       << dev.device_id           << "\n"
                          << "** name: "           << dev_info.name           << "\n"
                          << "** driver version: " << dev_info.driver_version << "\n"
                          << "** device version: " << dev_info.device_version << "\n"
                          << "** global memory: "  << dev_info.global_memory  << std::endl;
                for (auto & prop : dev_info.properties) {
                    std::cout << "** " << prop.first << ": " << prop.second << std::endl;
                }
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
        ::memcpy(host_memory.data, TEST_DATA, TEST_SIZE);

        auto host_read = gpu->mallocHost(context, ALLOC_SIZE);
        ASSERT_TRUE(host_read.existsMemory());
        ASSERT_EQ(ALLOC_SIZE, host_read.size);

        float write_millisec = 0;
        float  read_millisec = 0;
        auto write_event = gpu->createEvent(queue);
        auto  read_event = gpu->createEvent(queue);

        ASSERT_TRUE(gpu->write(queue, gpu_memory, host_memory, TEST_SIZE, &write_event));
        ASSERT_TRUE(gpu->finish(queue));
        ASSERT_TRUE(gpu->syncEvent(write_event));
        ASSERT_TRUE(gpu->elapsedEvent(write_event, &write_millisec));

        ASSERT_TRUE(gpu->read(queue, gpu_memory, host_read, TEST_SIZE, &read_event));
        ASSERT_TRUE(gpu->finish(queue));
        ASSERT_TRUE(gpu->syncEvent(read_event));
        ASSERT_TRUE(gpu->elapsedEvent(read_event, &read_millisec));

        std::cout << "Write: " << write_millisec << " millisec" << "\n"
                  << "Read: "  <<  read_millisec << " millisec" << std::endl;

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

