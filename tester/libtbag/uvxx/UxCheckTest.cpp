/**
 * @file   UxCheckTest.cpp
 * @brief  UxCheck class tester.
 * @author zer0
 * @date   2018-11-07
 */

#include <gtest/gtest.h>
#include <libtbag/uvxx/UxCheck.hpp>

using namespace libtbag;
using namespace libtbag::uvxx;

TEST(UxCheckTest, Default)
{
    UxCheck object;
    ASSERT_TRUE(true);
}

