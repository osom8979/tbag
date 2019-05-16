/**
 * @file   NumberOfArgumentsTest.cpp
 * @brief  NumberOfArguments class tester.
 * @author zer0
 * @date   2019-05-16
 */

#include <gtest/gtest.h>
#include <libtbag/tmp/NumberOfArguments.hpp>

using namespace libtbag;
using namespace libtbag::tmp;

TEST(NumberOfArgumentsTest, Default)
{
    ASSERT_EQ(1, get_number_of_arguments(1));
    ASSERT_EQ(2, get_number_of_arguments("test", "string"));
    ASSERT_EQ(3, get_number_of_arguments("test", "string", 100));
}

