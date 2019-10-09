/**
 * @file   ArgumentParserTest.cpp
 * @brief  ArgumentParser class tester.
 * @author zer0
 * @date   2019-10-09
 */

#include <gtest/gtest.h>
#include <libtbag/string/ArgumentParser.hpp>

using namespace libtbag;
using namespace libtbag::string;

TEST(ArgumentParserTest, Default)
{
    ArgumentParser object;
    ASSERT_TRUE(true);
}

