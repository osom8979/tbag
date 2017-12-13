/**
 * @file   BagTest.cpp
 * @brief  Bag class tester.
 * @author zer0
 * @date   2017-12-08
 */

#include <gtest/gtest.h>
#include <libtbag/container/Bag.hpp>

using namespace libtbag;
using namespace libtbag::container;

TEST(BagTest, Default)
{
    Bag object;
    ASSERT_TRUE(true);
}

