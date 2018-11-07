/**
 * @file   UxSignalTest.cpp
 * @brief  UxSignal class tester.
 * @author zer0
 * @date   2018-11-07
 */

#include <gtest/gtest.h>
#include <libtbag/uvxx/UxSignal.hpp>

using namespace libtbag;
using namespace libtbag::uvxx;

TEST(UxSignalTest, Default)
{
    UxSignal object;
    ASSERT_TRUE(true);
}

