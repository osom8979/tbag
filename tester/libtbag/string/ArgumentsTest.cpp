/**
 * @file   ArgumentsTest.cpp
 * @brief  Arguments class tester.
 * @author zer0
 * @date   2016-09-23
 */

#include <gtest/gtest.h>
#include <libtbag/string/Arguments.hpp>

using namespace libtbag;
using namespace libtbag::string;

TEST(ArgumentsTest, Default)
{
    char const * const TEST_ARGS = "test,1,2.1";
    Arguments args(TEST_ARGS);

    ASSERT_EQ(3, args.size());
    ASSERT_FALSE(args.empty());
    ASSERT_STREQ(TEST_ARGS, args.toString().c_str());
    ASSERT_STREQ("test", args.get<std::string>(0).c_str());
    ASSERT_EQ(1, args.get<int>(1));
    ASSERT_EQ(2.1, args.get<double>(2));
}

