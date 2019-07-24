/**
 * @file   TmxObjectTest.cpp
 * @brief  TmxObject class tester.
 * @author zer0
 * @date   2019-07-24
 */

#include <gtest/gtest.h>
#include <libtbag/tiled/details/TmxObject.hpp>

using namespace libtbag;
using namespace libtbag::tiled;
using namespace libtbag::tiled::details;

TEST(TmxObjectTest, Default)
{
    TmxObject object;
    ASSERT_TRUE(true);
}

