/**
 * @file   SysInfoTest.cpp
 * @brief  SysInfo class tester.
 * @author zer0
 * @date   2018-01-08
 */

#include <gtest/gtest.h>
#include <libtbag/system/SysInfo.hpp>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/Unit.hpp>

using namespace libtbag;
using namespace libtbag::system;

TEST(SysInfoTest, GetPageSize)
{
    int const PAGE_SIZE = getPageSize();
    ASSERT_LT(0, PAGE_SIZE);
    std::cout << "Page size: " << PAGE_SIZE << std::endl;
}

TEST(SysInfoTest, GetCacheLineSize)
{
    int const CACHE_LINE_SIZE = getCacheLineSize();
    ASSERT_LT(0, CACHE_LINE_SIZE);
    std::cout << "Cache-line size: " << CACHE_LINE_SIZE << std::endl;
}

TEST(SysInfoTest, GetDiskInfo)
{
    std::string const ROOT_DIR = filesystem::Path::getWorkDir().getRootDir().toString();

    FilesystemStatistics fs_stat;
    ASSERT_TRUE(getFilesystemInfo(ROOT_DIR, fs_stat));

    std::cout << "Filesystem: " << ROOT_DIR;
    std::cout << "\n * Filesystem block size: "      << fs_stat.bsize
              << "\n * Fragment size: "              << fs_stat.frsize
              << "\n * Size of fs in frsize units: " << fs_stat.blocks
              << "\n * Number of free blocks: "      << fs_stat.bfree
              << "\n * Number of free blocks for unprivileged users: " << fs_stat.bavail
              << "\n * Number of inodes: "           << fs_stat.files
              << "\n * Number of free inodes: "      << fs_stat.ffree
              << "\n * Number of free inodes for unprivileged users: " << fs_stat.favail
              << "\n * Filesystem ID: "              << fs_stat.fsid
              << "\n * Mount flags: "                << fs_stat.flag
              << "\n * Maximum filename length: "    << fs_stat.namemax << std::endl;
    std::cout << " * Filesystem total size: " << fs_stat.total_byte << " (" << fs_stat.total_byte / GIGA_BYTE_TO_BYTE << "GB)\n"
              << " * Filesystem free size: "  << fs_stat.free_byte  << " (" << fs_stat.free_byte  / GIGA_BYTE_TO_BYTE << "GB)\n"
              << " * Filesystem used size: "  << fs_stat.used_byte  << " (" << fs_stat.used_byte  / GIGA_BYTE_TO_BYTE << "GB)" << std::endl;
}

TEST(SysInfoTest, ResidentSetMemory)
{
    std::size_t const RSS = getResidentSetMemory();
    ASSERT_LT(0, RSS);
    std::cout << "ResidentSetMemory: " << RSS << std::endl;
}

TEST(SysInfoTest, Uptime)
{
    double const TIME = getUptime();
    ASSERT_LT(0.0f, TIME);
}

TEST(SysInfoTest, CpuInfo)
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

TEST(SysInfoTest, InterfaceAddresses)
{
    auto infos = getInterfaceAddresses();

    int index = 0;
    std::cout.setf(std::ios_base::boolalpha);
    for (auto & cursor : infos) {
        std::cout << "INTERFACE INFO[" << index
                  << "] " << cursor.name
                  << " (Physical: " << convertPhysicalToString(cursor.physical_address)
                  << ", Internal: " << cursor.is_internal << ")\n";

        int const FAMILY = cursor.address.in4.sin_family;
        ASSERT_TRUE(cursor.address.in4.sin_family == cursor.address.in6.sin6_family);
        ASSERT_TRUE(FAMILY == AF_INET || FAMILY == AF_INET6);

        if (cursor.address.in4.sin_family == AF_INET) {
            std::cout << "INET4: " << libtbag::uvpp::getIpName(&cursor.address.in4) << std::endl;
        } else if (cursor.address.in6.sin6_family == AF_INET6) {
            std::cout << "INET6: " << libtbag::uvpp::getIpName(&cursor.address.in6) << std::endl;
        }
        ++index;
    }
}

TEST(SysInfoTest, LoadAverage)
{
    auto load = getLoadAverage();
    if (load.empty()) {
        std::cout << "Empty load_average. Skip this test.\n";
        return;
    }
    ASSERT_EQ(3, load.size());
#if defined(TBAG_PLATFORM_WINDOWS)
    ASSERT_EQ(0, load[0]);
    ASSERT_EQ(0, load[1]);
    ASSERT_EQ(0, load[2]);
#else
    ASSERT_LT(0, load[0]);
    ASSERT_LT(0, load[1]);
    ASSERT_LT(0, load[2]);
#endif
}

TEST(SysInfoTest, Password)
{
    auto pwd = getPassword();
    ASSERT_FALSE(pwd.username.empty());
    ASSERT_FALSE(pwd.homedir.empty());
    std::cout << "Username: " << pwd.username << std::endl
              << "Shell: "    << pwd.shell    << std::endl
              << "Home dir: " << pwd.homedir  << std::endl
              << "UID: "      << pwd.uid      << std::endl
              << "GID: "      << pwd.gid      << std::endl;
}

TEST(SysInfoTest, TotalMemory)
{
    uint64_t const MEM = getTotalMemory();
    ASSERT_LT(0, MEM);
    std::cout << "Total memory: " << MEM << std::endl;
}

TEST(SysInfoTest, HighResolutionTime)
{
    uint64_t const HTIME = getHighResolutionTime();
    ASSERT_LT(0, HTIME);
    std::cout << "High resolution time: " << HTIME << std::endl;
}

