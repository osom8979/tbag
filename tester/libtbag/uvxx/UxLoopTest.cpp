/**
 * @file   UxLoopTest.cpp
 * @brief  UxLoop class tester.
 * @author zer0
 * @date   2018-12-05
 */

#include <gtest/gtest.h>
#include <libtbag/uvxx/UxLoop.hpp>

using namespace libtbag;
using namespace libtbag::uvxx;

TEST(UxLoopTest, Default)
{
    UxLoop loop;
    ASSERT_TRUE(true);
}

