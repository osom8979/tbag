/**
 * @file   TmxTest.cpp
 * @brief  Tmx class tester.
 * @author zer0
 * @date   2019-05-29
 */

#include <gtest/gtest.h>
#include <libtbag/tiled/Tmx.hpp>

using namespace libtbag;
using namespace libtbag::tiled;

TEST(TmxTest, Default)
{
    Tmx object;
    ASSERT_TRUE(true);
}

