/**
 * @file   UxTimerTest.cpp
 * @brief  UxTimer class tester.
 * @author zer0
 * @date   2018-11-07
 */

#include <gtest/gtest.h>
#include <libtbag/uvxx/UxTimer.hpp>

using namespace libtbag;
using namespace libtbag::uvxx;

TEST(UxTimerTest, Default)
{
    UxTimer object;
    ASSERT_TRUE(true);
}

