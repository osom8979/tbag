/**
 * @file   StringTest.cpp
 * @brief  String class tester.
 * @author zer0
 * @date   2016-11-23
 */

#include <gtest/gtest.h>
#include <libtbag/string/String.hpp>

#include <string>

using namespace libtbag;
using namespace libtbag::string;

TEST(StringTest, Default)
{
    String s;
}

TEST(StringTest, StdPerformance)
{
    std::string stds;
    String tbs;
}

