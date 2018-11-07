/**
 * @file   UxUdpTest.cpp
 * @brief  UxUdp class tester.
 * @author zer0
 * @date   2018-11-07
 */

#include <gtest/gtest.h>
#include <libtbag/uvxx/UxUdp.hpp>

using namespace libtbag;
using namespace libtbag::uvxx;

TEST(UxUdpTest, Default)
{
    UxUdp object;
    ASSERT_TRUE(true);
}

