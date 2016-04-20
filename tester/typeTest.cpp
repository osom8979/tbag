/**
 * @file   typeTest.cpp
 * @brief  type utilities tester.
 * @author zer0
 * @date   2016-04-20
 */

#include <gtest/gtest.h>
#include <libtbag/type.hpp>

using namespace libtbag;

template <typename T>
struct TestPoint
{
    T x, y;
};

TEST(typeTest, Default)
{
    TestPoint<int> p = makeTrivial2<TestPoint>(1, 2);
    ASSERT_EQ(p.x, 1);
    ASSERT_EQ(p.y, 2);
}

