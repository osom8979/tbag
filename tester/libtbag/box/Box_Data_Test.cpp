/**
 * @file   Box_Data_Test.cpp
 * @brief  Box class tester.
 * @author zer0
 * @date   2019-12-24
 */

#include <gtest/gtest.h>
#include <libtbag/box/Box.hpp>

using namespace libtbag;
using namespace libtbag::box;

TEST(Box_Data_Test, Default)
{
    Box b0;
    ASSERT_TRUE(isSuccess(b0.setData("TEST")));
    ASSERT_TRUE(b0.is_device_cpu());
    ASSERT_TRUE(b0.is_si8());
    ASSERT_EQ(1, b0.rank());
    ASSERT_EQ(4, b0.size());
    ASSERT_EQ(4, b0.dim(0));
    ASSERT_STREQ("TEST", b0.getDataString().c_str());
}

