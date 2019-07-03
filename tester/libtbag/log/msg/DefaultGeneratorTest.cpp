/**
 * @file   DefaultGeneratorTest.cpp
 * @brief  DefaultGenerator class tester.
 * @author zer0
 * @date   2019-07-02
 */

#include <gtest/gtest.h>
#include <libtbag/log/msg/DefaultGenerator.hpp>

using namespace libtbag;
using namespace libtbag::log;
using namespace libtbag::log::msg;

TEST(DefaultGeneratorTest, Default)
{
    DefaultGenerator object;
    ASSERT_TRUE(true);
}

