/**
 * @file   UxTcpTest.cpp
 * @brief  UxTcp class tester.
 * @author zer0
 * @date   2018-11-07
 */

#include <gtest/gtest.h>
#include <libtbag/uvxx/UxTcp.hpp>

using namespace libtbag;
using namespace libtbag::uvxx;

TEST(UxTcpTest, Default)
{
    UxTcp object;
    ASSERT_TRUE(true);
}

