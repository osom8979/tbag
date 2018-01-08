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
    ASSERT_LT(0, getPageSize());
}

