/**
 * @file   HazardPointerTest.cpp
 * @brief  HazardPointer class tester.
 * @author zer0
 * @date   2017-07-31
 */

#include <gtest/gtest.h>
#include <libtbag/memory/HazardPointer.hpp>

using namespace libtbag;
using namespace libtbag::memory;

TEST(HazardPointerTest, Default)
{
    HazardPointer object;
    ASSERT_TRUE(true);
}

