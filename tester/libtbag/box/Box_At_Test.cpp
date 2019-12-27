/**
 * @file   Box_At_Test.cpp
 * @brief  Box class tester.
 * @author zer0
 * @date   2019-12-27
 */

#include <gtest/gtest.h>
#include <libtbag/box/Box.hpp>

using namespace libtbag;
using namespace libtbag::box;

TEST(Box_At_Test, Default)
{
    Box src = {{1, 2}};
    ASSERT_EQ(1, src.at<Box::si32>(0, 0));
    ASSERT_EQ(2, src.at<Box::si32>(0, 1));
}

