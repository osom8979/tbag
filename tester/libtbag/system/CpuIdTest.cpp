/**
 * @file   CpuIdTest.cpp
 * @brief  CpuId class tester.
 * @author zer0
 * @date   2017-12-05
 */

#include <gtest/gtest.h>
#include <libtbag/system/CpuId.hpp>

using namespace libtbag;
using namespace libtbag::system;

TEST(CpuIdTest, Default)
{
    CpuId object;
    ASSERT_TRUE(true);
}

