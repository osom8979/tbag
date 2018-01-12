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

    auto const TOTAL_SIZE = getTotalDisk(fs_stat);
    auto const  FREE_SIZE =  getFreeDisk(fs_stat);
    auto const  USED_SIZE =  getUsedDisk(fs_stat);

    ASSERT_LT(0, TOTAL_SIZE);
    ASSERT_LT(0, FREE_SIZE);
    ASSERT_LT(0, USED_SIZE);

    std::cout << "DISK: " << ROOT_DIR;
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
    std::cout << " * Disk total size: " << TOTAL_SIZE << " (" << TOTAL_SIZE / GIGA_BYTE_TO_BYTE << "GB)\n"
              << " * Disk free size: "  <<  FREE_SIZE << " (" <<  FREE_SIZE / GIGA_BYTE_TO_BYTE << "GB)\n"
              << " * Disk used size: "  <<  USED_SIZE << " (" <<  USED_SIZE / GIGA_BYTE_TO_BYTE << "GB)" << std::endl;
}

