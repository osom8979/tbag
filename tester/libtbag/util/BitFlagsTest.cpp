/**
 * @file   BitFlagsTest.cpp
 * @brief  BitFlags class tester.
 * @author zer0
 * @date   2017-04-05
 */

#include <gtest/gtest.h>
#include <libtbag/util/BitFlags.hpp>

using namespace libtbag;
using namespace libtbag::util;

TEST(BitFlagsTest, Default)
{
    BitFlags object;
    ASSERT_TRUE(true);
}

