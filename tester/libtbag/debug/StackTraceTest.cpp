/**
 * @file   StackTraceTest.cpp
 * @brief  StackTrace class tester.
 * @author zer0
 * @date   2017-05-28
 */

#include <gtest/gtest.h>
#include <libtbag/debug/StackTrace.hpp>
#include <iostream>

using namespace libtbag;
using namespace libtbag::debug;

TEST(StackTraceTest, Default)
{
    std::cout << getStackTrace() << std::endl;
}

