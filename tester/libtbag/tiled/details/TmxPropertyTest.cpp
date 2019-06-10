/**
 * @file   TmxPropertyTest.cpp
 * @brief  TmxProperty class tester.
 * @author zer0
 * @date   2019-06-10
 */

#include <gtest/gtest.h>
#include <libtbag/tiled/details/TmxProperty.hpp>
#include <libtbag/util/TestUtils.hpp>

using namespace libtbag;
using namespace libtbag::tiled;
using namespace libtbag::tiled::details;

TEST(TmxPropertyTest, Default)
{
    TEST_DEFAULT_CONSTRUCTOR(TmxProperty, obj1)
    TEST_DEFAULT_ASSIGNMENT(TmxProperty, obj2)
}

