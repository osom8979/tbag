/**
 * @file   TmxTileOffsetTest.cpp
 * @brief  TmxTileOffset class tester.
 * @author zer0
 * @date   2019-06-19
 */

#include <gtest/gtest.h>
#include <libtbag/tiled/details/TmxTileOffset.hpp>
#include <libtbag/util/TestUtils.hpp>

using namespace libtbag;
using namespace libtbag::tiled;
using namespace libtbag::tiled::details;

TEST(TmxTileOffsetTest, Default)
{
    TEST_DEFAULT_CONSTRUCTOR(TmxTileOffset, obj1)
    TEST_DEFAULT_ASSIGNMENT(TmxTileOffset, obj2)
}

