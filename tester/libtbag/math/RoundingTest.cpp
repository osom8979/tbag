/**
 * @file   RoundingTest.cpp
 * @brief  Rounding class tester.
 * @author zer0
 * @date   2018-07-11
 */

#include <gtest/gtest.h>
#include <libtbag/math/Rounding.hpp>

using namespace libtbag;
using namespace libtbag::math;

template <typename T>
struct RoundingCastTest : public testing::Test
{
    using TestType   = RoundingCast<T>;
    using ResultType = typename TestType::ResultType;

    virtual void SetUp() override
    {
        // EMPTY.
    }

    virtual void TearDown() override
    {
        // EMPTY.
    }

    template <typename Arg>
    ResultType testRound(Arg v)
    {
        return TestType::round(v);
    }

    template <typename Arg>
    ResultType testRint(Arg v)
    {
        return TestType::rint(v);
    }
};

using RoundingCastTypes = testing::Types<int, unsigned int, long int, long long int>;
TYPED_TEST_CASE(RoundingCastTest, RoundingCastTypes);

TYPED_TEST(RoundingCastTest, RoundAndRint)
{
    ASSERT_EQ( 2, this->testRound( 2.3));
    ASSERT_EQ( 3, this->testRound( 2.5));
    ASSERT_EQ( 4, this->testRound( 3.8));
    ASSERT_EQ(-2, this->testRound(-2.3));
    ASSERT_EQ(-3, this->testRound(-2.5));
    ASSERT_EQ(-4, this->testRound(-3.8));

    ASSERT_EQ( 2, this->testRint( 2.3));
    ASSERT_EQ( 2, this->testRint( 2.5));
    ASSERT_EQ( 4, this->testRint( 3.8));
    ASSERT_EQ(-2, this->testRint(-2.3));
    ASSERT_EQ(-2, this->testRint(-2.5));
    ASSERT_EQ(-4, this->testRint(-3.8));
}

TEST(RoundingTest, Default)
{
    ASSERT_EQ( 2, libtbag::math::round<int>( 2.3));
    ASSERT_EQ( 3, libtbag::math::round<int>( 2.5));
    ASSERT_EQ( 4, libtbag::math::round<int>( 3.8));
    ASSERT_EQ(-2, libtbag::math::round<int>(-2.3));
    ASSERT_EQ(-3, libtbag::math::round<int>(-2.5));
    ASSERT_EQ(-4, libtbag::math::round<int>(-3.8));

    ASSERT_EQ( 2, libtbag::math::rint<int>( 2.3));
    ASSERT_EQ( 2, libtbag::math::rint<int>( 2.5));
    ASSERT_EQ( 4, libtbag::math::rint<int>( 3.8));
    ASSERT_EQ(-2, libtbag::math::rint<int>(-2.3));
    ASSERT_EQ(-2, libtbag::math::rint<int>(-2.5));
    ASSERT_EQ(-4, libtbag::math::rint<int>(-3.8));
}
