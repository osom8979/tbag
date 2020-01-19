/**
 * @file   Arc2Test.cpp
 * @brief  Arc2 class tester.
 * @author zer0
 * @date   2020-01-19
 */

#include <gtest/gtest.h>
#include <libtbag/geometry/Arc2.hpp>

using namespace libtbag;
using namespace libtbag::geometry;

TEST(Arc2Test, Default)
{
    Arc2 arc;
}

TEST(Arc2Test, String)
{
    auto const text = Arc2().toString();
    ASSERT_FALSE(text.empty());
    std::cout << "Arc2: " << text << std::endl;
}

