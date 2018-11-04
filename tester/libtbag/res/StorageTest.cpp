/**
 * @file   StorageTest.cpp
 * @brief  Storage class tester.
 * @author zer0
 * @date   2018-11-03
 */

#include <gtest/gtest.h>
#include <libtbag/res/Storage.hpp>

using namespace libtbag;
using namespace libtbag::res;

TEST(StorageTest, Default)
{
    Storage object;
    ASSERT_TRUE(true);
}

