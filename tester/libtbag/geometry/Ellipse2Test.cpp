/**
 * @file   Ellipse2Test.cpp
 * @brief  Ellipse2 class tester.
 * @author zer0
 * @date   2020-01-19
 */

#include <gtest/gtest.h>
#include <libtbag/geometry/Ellipse2.hpp>

using namespace libtbag;
using namespace libtbag::geometry;

TEST(Ellipse2Test, Default)
{
    Ellipse2 ellipse;
}

TEST(Ellipse2Test, String)
{
    auto const text = Ellipse2().toString();
    ASSERT_FALSE(text.empty());
    std::cout << "Ellipse2: " << text << std::endl;
}

