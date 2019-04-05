/**
 * @file   UvUtilsTest.cpp
 * @brief  Test uvpp miscellaneous utilities.
 * @author zer0
 * @date   2017-05-27
 */

#include <gtest/gtest.h>
#include <libtbag/uvpp/UvUtils.hpp>

#include <iostream>
#include <string>

using namespace libtbag;
using namespace libtbag::uvpp;

TEST(UvUtilsTest, ResidentSetMemory)
{
    std::size_t const RSS = getResidentSetMemory();
    ASSERT_LT(0, RSS);
    std::cout << "ResidentSetMemory: " << RSS << std::endl;
}

TEST(UvUtilsTest, Uptime)
{
    double const TIME = getUptime();
    ASSERT_LT(0.0f, TIME);
}

TEST(UvUtilsTest, ResourceUsage)
{
    auto res = getResourceUsage();
#if !defined(TBAG_PLATFORM_WINDOWS)
    std::cout << "Resource Usage:\n"
              << "utime: " << res.utime.sec << "." << res.utime.usec << std::endl
              << "stime: " << res.stime.sec << "." << res.stime.usec << std::endl
              << "maxrss: "   << res.maxrss   << std::endl
              << "ixrss: "    << res.ixrss    << std::endl
              << "idrss: "    << res.idrss    << std::endl
              << "isrss: "    << res.isrss    << std::endl
              << "minflt: "   << res.minflt   << std::endl
              << "majflt: "   << res.majflt   << std::endl
              << "nswap: "    << res.nswap    << std::endl
              << "inblock: "  << res.inblock  << std::endl
              << "oublock: "  << res.oublock  << std::endl
              << "msgsnd: "   << res.msgsnd   << std::endl
              << "msgrcv: "   << res.msgrcv   << std::endl
              << "nsignals: " << res.nsignals << std::endl
              << "nvcsw: "    << res.nvcsw    << std::endl
              << "nivcsw: "   << res.nivcsw   << std::endl;
#endif
}

TEST(UvUtilsTest, CpuInfo)
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

TEST(UvUtilsTest, InterfaceAddresses)
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
            std::cout << "INET4: " << getIpName(&cursor.address.in4) << std::endl;
        } else if (cursor.address.in6.sin6_family == AF_INET6) {
            std::cout << "INET6: " << getIpName(&cursor.address.in6) << std::endl;
        }
        ++index;
    }
}

TEST(UvUtilsTest, LoadAverage)
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

TEST(UvUtilsTest, Password)
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

TEST(UvUtilsTest, TotalMemory)
{
    uint64_t const MEM = getTotalMemory();
    ASSERT_LT(0, MEM);
    std::cout << "Total memory: " << MEM << std::endl;
}

TEST(UvUtilsTest, HighResolutionTime)
{
    uint64_t const HTIME = getHighResolutionTime();
    ASSERT_LT(0, HTIME);
    std::cout << "High resolution time: " << HTIME << std::endl;
}

TEST(UvUtilsTest, GetEnv)
{
    std::string path;
    ASSERT_EQ(E_SUCCESS, getEnv("PATH", path));
    ASSERT_FALSE(path.empty());
}

