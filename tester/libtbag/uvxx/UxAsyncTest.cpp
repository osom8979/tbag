/**
 * @file   UxAsyncTest.cpp
 * @brief  UxAsync class tester.
 * @author zer0
 * @date   2018-11-07
 */

#include <gtest/gtest.h>
#include <libtbag/uvxx/UxAsync.hpp>

using namespace libtbag;
using namespace libtbag::uvxx;

TEST(UxAsyncTest, Default)
{
    UxAsync object;
    ASSERT_TRUE(true);
}

