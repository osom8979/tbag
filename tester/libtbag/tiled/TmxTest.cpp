/**
 * @file   TmxTest.cpp
 * @brief  Tmx class tester.
 * @author zer0
 * @date   2019-05-29
 */

#include <gtest/gtest.h>
#include <libtbag/tiled/Tmx.hpp>
#include <libtbag/util/TestUtils.hpp>

using namespace libtbag;
using namespace libtbag::tiled;

TEST(TmxTest, Default)
{
    TEST_DEFAULT_CONSTRUCTOR(Tmx, obj1)
    TEST_DEFAULT_ASSIGNMENT(Tmx, obj2)
}

