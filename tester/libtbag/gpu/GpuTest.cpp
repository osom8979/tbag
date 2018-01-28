/**
 * @file   GpuTest.cpp
 * @brief  Gpu class tester.
 * @author zer0
 * @date   2018-01-13
 */

#include <gtest/gtest.h>
#include <libtbag/gpu/Gpu.hpp>
#include <libtbag/Unit.hpp>

using namespace libtbag;
using namespace libtbag::gpu;

TEST(GpuTest, Information)
{
    runAllIfSupported([](Gpu & gpu){
        auto plat_info = gpu.atPlatformInfo();
        auto  dev_info = gpu.atDeviceInfo();
        std::cout.setf(std::ios_base::boolalpha);
        std::cout << "GPU Type(" << gpu.getTypeString()
                  << ") Plt("    << gpu.getPlatformId()
                  << ") Dev("    << gpu.getDeviceId()
                  << ") Cxt("    << gpu.getContextId()
                  << ") H("      << gpu.isHost()
                  << ") D("      << gpu.isDevice()
                  << ") S("      << gpu.isStream() << ")\n";
        std::cout << "* Profile(" << plat_info.profile
                  << ") Name("    << plat_info.name
                  << ") Vender("  << plat_info.vendor
                  << ") Version(" << plat_info.version << ")\n";
        std::cout << "* Extensions: " << plat_info.extensions << "\n";
        std::cout << "* Device(" << dev_info.name
                  << ") Ver("    << dev_info.driver_version
                  << ") DevVer(" << dev_info.device_version
                  << ") GMem("   << dev_info.global_memory << ")" << std::endl;
        for (auto & prop : dev_info.properties) {
            std::cout << "** " << prop.first << ": " << prop.second << std::endl;
        }
    });
}

TEST(GpuTest, SyncedMemory_Default)
{
    runAllIfSupported([](Gpu & gpu){
        auto stream = gpu.newStream();
        auto event  = gpu.newEvent(stream);
        SyncedMemory mem(stream, event);
        ASSERT_EQ(SyncedMemory::SyncedHead::SH_UNINITIALIZED, mem.head());

        ASSERT_EQ(Err::E_SUCCESS, mem.resize<char>(8));
        ASSERT_EQ(SyncedMemory::SyncedHead::SH_SYNCED, mem.head());
        ASSERT_EQ(type::TypeTable::TT_CHAR, mem.type());
        ASSERT_EQ(sizeof(char), mem.sizeOfElem());
        ASSERT_EQ(8, mem.countOfElem());
        ASSERT_EQ(sizeof(char) * 8, mem.size());

        ASSERT_EQ(Err::E_SUCCESS, mem.resize<int>(4));
        ASSERT_EQ(SyncedMemory::SyncedHead::SH_SYNCED, mem.head());
        ASSERT_EQ(type::TypeTable::TT_INT, mem.type());
        ASSERT_EQ(sizeof(int), mem.sizeOfElem());
        ASSERT_EQ(4, mem.countOfElem());
        ASSERT_EQ(sizeof(int) * 4, mem.size());

        // Shallow copy.
        SyncedMemory mem2 = mem;
        ASSERT_EQ(mem.getHostData(), mem2.getHostData());
        ASSERT_EQ(mem.getGpuData(), mem2.getGpuData());
        ASSERT_EQ(mem.size(), mem2.size());
    });
}

TEST(GpuTest, SyncedMemory)
{
    runAllIfSupported([](Gpu & gpu){
        std::cout.setf(std::ios_base::boolalpha);
        std::cout << "GPU Type(" << gpu.getTypeString()
                  << ") Plt("    << gpu.getPlatformId()
                  << ") Dev("    << gpu.getDeviceId()
                  << ") Cxt("    << gpu.getContextId() << ")\n";

        using BaseType = int;
        BaseType const TEST_HOST_VALUE1 = 1;
        BaseType const TEST_HOST_VALUE2 = 2;
        BaseType const TEST_GPU_VALUE   = 3;

        std::size_t const BUFFER_SIZE = 1 * MEGA_BYTE_TO_BYTE;
        std::size_t const MEM_SIZE = BUFFER_SIZE * sizeof(BaseType);
        std::vector<BaseType> buffer(BUFFER_SIZE, TEST_HOST_VALUE1);

        ASSERT_LT(0, BUFFER_SIZE);
        ASSERT_EQ(BUFFER_SIZE, buffer.size());
        std::cout << "* Mem size: " << MEM_SIZE / MEGA_BYTE_TO_BYTE << "mb" << std::endl;

        auto stream = gpu.newStream();
        auto event  = gpu.newEvent(stream);
        SyncedMemory mem(stream, event);
        ASSERT_TRUE(mem.empty());
        ASSERT_EQ(SyncedMemory::SyncedHead::SH_UNINITIALIZED, mem.head());

        ASSERT_EQ(Err::E_SUCCESS, mem.resize<BaseType>(BUFFER_SIZE));
        ASSERT_EQ(type::TypeTable::TT_INT, mem.type());

        ASSERT_TRUE(mem.exists());
        ASSERT_FALSE(mem.empty());
        ASSERT_EQ(MEM_SIZE, mem.size());
        ASSERT_EQ(sizeof(BaseType), mem.sizeOfElem());
        ASSERT_EQ(BUFFER_SIZE, mem.countOfElem());

        ASSERT_EQ(Err::E_SUCCESS, mem.assign(buffer.data(), buffer.size()));
        ASSERT_EQ(SyncedMemory::SyncedHead::SH_HEAD_AT_HOST, mem.head());
        ASSERT_EQ(Err::E_SUCCESS, mem.toSync());
        ASSERT_EQ(SyncedMemory::SyncedHead::SH_SYNCED, mem.head());
        ASSERT_EQ(Err::E_SUCCESS, mem.syncEvent());
        std::cout << "* Mem sync Host -> Gpu: " << mem.elapsedEvent() << " millisec." << std::endl;

        auto * host_data_01 = mem.castHostData<BaseType>();
        for (std::size_t i = 0; i < BUFFER_SIZE; ++i) {
            ASSERT_EQ(TEST_HOST_VALUE1, *(host_data_01 + i));
        }
        ASSERT_EQ(SyncedMemory::SyncedHead::SH_SYNCED, mem.head());

        ASSERT_EQ(Err::E_SUCCESS, mem.fillGpu(TEST_GPU_VALUE));
        ASSERT_EQ(SyncedMemory::SyncedHead::SH_HEAD_AT_GPU, mem.head());
        ASSERT_EQ(Err::E_SUCCESS, mem.syncEvent());
        std::cout << "* Mem sync Gpu Fill: " << mem.elapsedEvent() << " millisec." << std::endl;

        ASSERT_EQ(Err::E_SUCCESS, mem.toSync());
        ASSERT_EQ(SyncedMemory::SyncedHead::SH_SYNCED, mem.head());
        ASSERT_EQ(Err::E_SUCCESS, mem.syncEvent());
        std::cout << "* Mem sync Gpu -> Host: " << mem.elapsedEvent() << " millisec." << std::endl;

        auto * host_data_02 = mem.castHostData<BaseType>();
        for (std::size_t i = 0; i < BUFFER_SIZE; ++i) {
            ASSERT_EQ(TEST_GPU_VALUE, *(host_data_02 + i));
        }
        ASSERT_EQ(SyncedMemory::SyncedHead::SH_SYNCED, mem.head());

        ASSERT_EQ(Err::E_SUCCESS, mem.fillHost(TEST_HOST_VALUE2));
        ASSERT_EQ(SyncedMemory::SyncedHead::SH_HEAD_AT_HOST, mem.head());
        ASSERT_EQ(Err::E_SUCCESS, mem.toSync());
        ASSERT_EQ(SyncedMemory::SyncedHead::SH_SYNCED, mem.head());
        ASSERT_EQ(Err::E_SUCCESS, mem.syncEvent());

        auto * host_data_03 = mem.castHostData<BaseType>();
        for (std::size_t i = 0; i < BUFFER_SIZE; ++i) {
            ASSERT_EQ(TEST_HOST_VALUE2, *(host_data_03 + i));
        }
        ASSERT_EQ(SyncedMemory::SyncedHead::SH_SYNCED, mem.head());
    });
}

