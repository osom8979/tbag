/**
 * @file   UxRequestTest.cpp
 * @brief  UxRequest class tester.
 * @author zer0
 * @date   2018-11-07
 */

#include <gtest/gtest.h>
#include <libtbag/uvxx/UxRequest.hpp>

using namespace libtbag;
using namespace libtbag::uvxx;

TEST(UxRequestTest, Default)
{
    UxRequest object;
    ASSERT_TRUE(true);
}

