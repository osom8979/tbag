/**
 * @file   DefaultColorGeneratorTest.cpp
 * @brief  DefaultColorGenerator class tester.
 * @author zer0
 * @date   2019-07-02
 */

#include <gtest/gtest.h>
#include <libtbag/log/msg/DefaultColorGenerator.hpp>

using namespace libtbag;
using namespace libtbag::log;
using namespace libtbag::log::msg;

TEST(DefaultColorGeneratorTest, Default)
{
    DefaultColorGenerator object;
    ASSERT_TRUE(true);
}

