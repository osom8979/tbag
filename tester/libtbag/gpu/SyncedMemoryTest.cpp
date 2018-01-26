/**
 * @file   SyncedMemoryTest.cpp
 * @brief  SyncedMemory class tester.
 * @author zer0
 * @date   2018-01-19
 */

#include <gtest/gtest.h>
#include <libtbag/gpu/Gpu.hpp>
#include <libtbag/gpu/SyncedMemory.hpp>
#include <libtbag/Unit.hpp>

using namespace libtbag;
using namespace libtbag::gpu;

TEST(SyncedMemoryTest, Assign)
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
        ASSERT_EQ(Err::E_SUCCESS, mem.syncMemory());
        ASSERT_EQ(SyncedMemory::SyncedHead::SH_SYNCED, mem.head());
        ASSERT_EQ(Err::E_SUCCESS, mem.sync());
        std::cout << "* Mem sync Host -> Gpu (" << MEM_SIZE / MEGA_BYTE_TO_BYTE << "MB): " << mem.elapsed() << " millisec." << std::endl;

        ASSERT_NE(nullptr, mem.getMutableGpuData());
        ASSERT_EQ(SyncedMemory::SyncedHead::SH_HEAD_AT_GPU, mem.head());
        ASSERT_EQ(Err::E_SUCCESS, mem.syncMemory());
        ASSERT_EQ(SyncedMemory::SyncedHead::SH_SYNCED, mem.head());
        ASSERT_EQ(Err::E_SUCCESS, mem.sync());
        std::cout << "* Mem sync Gpu -> Host (" << MEM_SIZE / MEGA_BYTE_TO_BYTE << "MB): " << mem.elapsed() << " millisec." << std::endl;
    });
}

