/**
 * @file   TmxGridTest.cpp
 * @brief  TmxGrid class tester.
 * @author zer0
 * @date   2019-06-19
 */

#include <gtest/gtest.h>
#include <libtbag/tiled/details/TmxGrid.hpp>
#include <libtbag/util/TestUtils.hpp>

using namespace libtbag;
using namespace libtbag::tiled;
using namespace libtbag::tiled::details;

TEST(TmxGridTest, Default)
{
    TEST_DEFAULT_CONSTRUCTOR(TmxGrid, obj1)
    TEST_DEFAULT_ASSIGNMENT(TmxGrid, obj2)
}

