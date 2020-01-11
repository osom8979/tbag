/**
 * @file   CanvasTest.cpp
 * @brief  Canvas class tester.
 * @author zer0
 * @date   2020-01-11
 */

#include <gtest/gtest.h>
#include <libtbag/graphic/Canvas.hpp>

using namespace libtbag;
using namespace libtbag::graphic;

TEST(CanvasTest, Default)
{
    Canvas object;
    ASSERT_TRUE(true);
}

