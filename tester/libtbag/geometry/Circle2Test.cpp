/**
 * @file   Circle2Test.cpp
 * @brief  Circle2 class tester.
 * @author zer0
 * @date   2020-01-19
 */

#include <gtest/gtest.h>
#include <libtbag/geometry/Circle2.hpp>

using namespace libtbag;
using namespace libtbag::geometry;

TEST(Circle2Test, Default)
{
    Circle2 circle;
}

TEST(Circle2Test, String)
{
    auto const text = Circle2().toString();
    ASSERT_FALSE(text.empty());
    std::cout << "Circle2: " << text << std::endl;
}

