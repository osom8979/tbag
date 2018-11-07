/**
 * @file   UxProcessTest.cpp
 * @brief  UxProcess class tester.
 * @author zer0
 * @date   2018-11-07
 */

#include <gtest/gtest.h>
#include <libtbag/uvxx/UxProcess.hpp>

using namespace libtbag;
using namespace libtbag::uvxx;

TEST(UxProcessTest, Default)
{
    UxProcess object;
    ASSERT_TRUE(true);
}

