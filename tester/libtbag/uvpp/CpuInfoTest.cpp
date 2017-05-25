/**
 * @file   CpuInfoTest.cpp
 * @brief  CpuInfo class tester.
 * @author zer0
 * @date   2017-05-25
 */

#include <gtest/gtest.h>
#include <libtbag/uvpp/CpuInfo.hpp>
#include <string>

using namespace libtbag;
using namespace libtbag::uvpp;

TEST(CpuInfoTest, Default)
{
    auto infos = getCpuInfos();
    ASSERT_FALSE(infos.empty());

    int index = 0;
    for (auto & cursor : infos) {
        std::cout << "CPU INFO[" << index
                  << "] " << cursor.model
                  << " (SPEED: " << cursor.speed << ")\n";
        std::cout << " * user(" << cursor.times.user
                  << ")/nice("  << cursor.times.nice
                  << ")/sys("   << cursor.times.sys
                  << ")/idle("  << cursor.times.idle
                  << ")/irq("   << cursor.times.irq << ")\n";
        ++index;
    }
}

