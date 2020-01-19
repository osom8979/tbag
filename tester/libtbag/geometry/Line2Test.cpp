/**
 * @file   Line2Test.cpp
 * @brief  Line2 class tester.
 * @author zer0
 * @date   2020-01-19
 */

#include <gtest/gtest.h>
#include <libtbag/geometry/Line2.hpp>

using namespace libtbag;
using namespace libtbag::geometry;

TEST(Line2Test, Default)
{
    Line2 line;
}

TEST(Line2Test, String)
{
    auto const text = Line2().toString();
    ASSERT_FALSE(text.empty());
    std::cout << "Line2: " << text << std::endl;
}

