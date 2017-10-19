/**
 * @file   PageTest.cpp
 * @brief  Page class tester.
 * @author zer0
 * @date   2017-10-19
 */

#include <gtest/gtest.h>
#include <libtbag/system/Page.hpp>

using namespace libtbag;
using namespace libtbag::system;

TEST(PageTest, Default)
{
    ASSERT_LT(0, getPageSize());
}

