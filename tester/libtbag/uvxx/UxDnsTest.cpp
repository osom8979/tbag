/**
 * @file   UxDnsTest.cpp
 * @brief  UxDns class tester.
 * @author zer0
 * @date   2018-11-07
 */

#include <gtest/gtest.h>
#include <libtbag/uvxx/UxDns.hpp>

using namespace libtbag;
using namespace libtbag::uvxx;

TEST(UxDnsTest, Default)
{
    UxDns object;
    ASSERT_TRUE(true);
}

