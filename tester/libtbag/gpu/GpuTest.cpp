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

TEST(GpuTest, SyncedMemory)
{
    runAllIfSupported([](Gpu & gpu){
        std::cout.setf(std::ios_base::boolalpha);
        std::cout << "GPU Type(" << gpu.getTypeString()
                  << ") Plt("    << gpu.getPlatformId()
                  << ") Dev("    << gpu.getDeviceId()
                  << ") Cxt("    << gpu.getContextId() << ")\n";

        std::size_t const MEM_SIZE = 100 * MEGA_BYTE_TO_BYTE;
        std::vector<char> buffer(MEGA_BYTE_TO_BYTE, '1');

        auto stream = gpu.newStream();
        auto event  = gpu.newEvent(stream);
        SyncedMemory mem(stream, event);
        ASSERT_TRUE(mem.empty());

        ASSERT_EQ(Err::E_SUCCESS, mem.resize<char>(MEM_SIZE));
        ASSERT_EQ(type::TypeTable::TT_CHAR, mem.type());

        ASSERT_TRUE(mem.exists());

        ASSERT_EQ(MEM_SIZE, mem.size());
        ASSERT_EQ(Err::E_SUCCESS, mem.assign(buffer.data(), buffer.size()));
        ASSERT_EQ(SyncedMemory::SyncedHead::SH_HEAD_AT_HOST, mem.head());
        ASSERT_EQ(Err::E_SUCCESS, mem.toSync());
        ASSERT_EQ(SyncedMemory::SyncedHead::SH_SYNCED, mem.head());
        ASSERT_EQ(Err::E_SUCCESS, mem.syncEvent());
        std::cout << "* Mem sync Host -> Gpu (" << MEM_SIZE / MEGA_BYTE_TO_BYTE << "MB): " << mem.elapsedEvent() << " millisec." << std::endl;

        ASSERT_NE(nullptr, mem.getMutableGpuData());
        ASSERT_EQ(SyncedMemory::SyncedHead::SH_HEAD_AT_GPU, mem.head());
        ASSERT_EQ(Err::E_SUCCESS, mem.toSync());
        ASSERT_EQ(SyncedMemory::SyncedHead::SH_SYNCED, mem.head());
        ASSERT_EQ(Err::E_SUCCESS, mem.syncEvent());
        std::cout << "* Mem sync Gpu -> Host (" << MEM_SIZE / MEGA_BYTE_TO_BYTE << "MB): " << mem.elapsedEvent() << " millisec." << std::endl;
    });
}

