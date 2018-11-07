/**
 * @file   UxStreamTest.cpp
 * @brief  UxStream class tester.
 * @author zer0
 * @date   2018-11-07
 */

#include <gtest/gtest.h>
#include <libtbag/uvxx/UxStream.hpp>

using namespace libtbag;
using namespace libtbag::uvxx;

TEST(UxStreamTest, Default)
{
    UxStream object;
    ASSERT_TRUE(true);
}

