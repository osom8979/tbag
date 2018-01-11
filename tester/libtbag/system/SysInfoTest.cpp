/**
 * @file   SysInfoTest.cpp
 * @brief  SysInfo class tester.
 * @author zer0
 * @date   2018-01-08
 */

#include <gtest/gtest.h>
#include <libtbag/system/SysInfo.hpp>

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

