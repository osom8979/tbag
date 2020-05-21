/**
 * @file   ResourceUsageTest.cpp
 * @brief  ResourceUsage class tester.
 * @author zer0
 * @date   2019-10-20
 */

#include <gtest/gtest.h>
#include <libtbag/system/ResourceUsage.hpp>

using namespace libtbag;
using namespace libtbag::system;

TEST(ResourceUsageTest, ResourceUsage)
{
    auto const result = getResourceUsage();
    ASSERT_EQ(E_SUCCESS, result.code);
    auto const & res = result.val;
    std::cout << "Resource Usage:\n"
              << "utime: "    << res.utime.sec << "." << res.utime.usec << std::endl
              << "stime: "    << res.stime.sec << "." << res.stime.usec << std::endl
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
}

