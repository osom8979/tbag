/**
 * @file   Triangle2Test.cpp
 * @brief  Triangle2 class tester.
 * @author zer0
 * @date   2020-01-19
 */

#include <gtest/gtest.h>
#include <libtbag/geometry/Triangle2.hpp>

using namespace libtbag;
using namespace libtbag::geometry;

TEST(Triangle2Test, Default)
{
    Triangle2 triangle;
}

TEST(Triangle2Test, String)
{
    auto const text = Triangle2().toString();
    ASSERT_FALSE(text.empty());
    std::cout << "Triangle2: " << text << std::endl;
}

