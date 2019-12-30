/**
 * @file   TypeTest.cpp
 * @brief  Type utilities tester.
 * @author zer0
 * @date   2016-04-20
 */

#include <gtest/gtest.h>
#include <libtbag/Type.hpp>

using namespace libtbag;

template <typename T>
struct TestPoint
{
    T x, y;
};

TEST(TypeTest, Default)
{
    TestPoint<int> p = makeTrivial2<TestPoint>(1, 2);
    ASSERT_EQ(1, p.x);
    ASSERT_EQ(2, p.y);
}

TEST(TypeTest, RemoveCpr)
{
    ASSERT_TRUE((std::is_same<int, typename remove_cpr<int const * &&>::type>::value));
    ASSERT_TRUE((std::is_same<int, typename remove_cpr<int const * &>::type>::value));
    ASSERT_TRUE((std::is_same<int, typename remove_cpr<int const *>::type>::value));
    ASSERT_TRUE((std::is_same<int, typename remove_cpr<int const &&>::type>::value));
    ASSERT_TRUE((std::is_same<int, typename remove_cpr<int const &>::type>::value));
    ASSERT_TRUE((std::is_same<int, typename remove_cpr<int * &&>::type>::value));
    ASSERT_TRUE((std::is_same<int, typename remove_cpr<int * &>::type>::value));
    ASSERT_TRUE((std::is_same<int, typename remove_cpr<int *>::type>::value));
    ASSERT_TRUE((std::is_same<int, typename remove_cpr<int &&>::type>::value));
    ASSERT_TRUE((std::is_same<int, typename remove_cpr<int &>::type>::value));
    ASSERT_TRUE((std::is_same<int, typename remove_cpr<int>::type>::value));
}

