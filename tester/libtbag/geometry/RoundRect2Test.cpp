/**
 * @file   RoundRect2Test.cpp
 * @brief  RoundRect2 class tester.
 * @author zer0
 * @date   2020-01-19
 */

#include <gtest/gtest.h>
#include <libtbag/geometry/RoundRect2.hpp>

using namespace libtbag;
using namespace libtbag::geometry;

TEST(RoundRect2Test, Default)
{
    RoundRect2 rect;
}

TEST(RoundRect2Test, String)
{
    auto const text = RoundRect2().toString();
    ASSERT_FALSE(text.empty());
    std::cout << "RoundRect2: " << text << std::endl;
}

