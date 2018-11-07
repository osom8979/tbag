/**
 * @file   UxTtyTest.cpp
 * @brief  UxTty class tester.
 * @author zer0
 * @date   2018-11-07
 */

#include <gtest/gtest.h>
#include <libtbag/uvxx/UxTty.hpp>

using namespace libtbag;
using namespace libtbag::uvxx;

TEST(UxTtyTest, Default)
{
    UxTty object;
    ASSERT_TRUE(true);
}

