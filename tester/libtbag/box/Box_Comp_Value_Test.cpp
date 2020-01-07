/**
 * @file   Box_Comp_Value_Test.cpp
 * @brief  Box class tester.
 * @author zer0
 * @date   2020-01-07
 */

#include <gtest/gtest.h>
#include <libtbag/box/Box.hpp>

using namespace libtbag;
using namespace libtbag::box;

struct Box_Comp_Value_Test_Fixture : public testing::Test
{
    Box src;

    void SetUp() override
    {
        src = { { 1, 2, 3 },
                { 4, 5, 6 },
                { 7, 8, 9 } };
    }

    void TearDown() override
    {
        src = nullptr;
    }
};

TEST_F(Box_Comp_Value_Test_Fixture, EqualTo)
{
    auto const err_result = src.eq(5);
    ASSERT_EQ(E_SUCCESS, err_result.code);

    auto const & result = err_result.value;
    ASSERT_TRUE(result.is_bool());
    ASSERT_TRUE(result.is_device_cpu());
    ASSERT_EQ(0, result.ext0());
    ASSERT_EQ(0, result.ext1());
    ASSERT_EQ(0, result.ext2());
    ASSERT_EQ(0, result.ext3());
    ASSERT_EQ(2, result.rank());
    ASSERT_EQ(3, result.dim(0));
    ASSERT_EQ(3, result.dim(1));
    ASSERT_EQ(9, result.size());

    ASSERT_FALSE(result.at<bool>(0));
    ASSERT_FALSE(result.at<bool>(1));
    ASSERT_FALSE(result.at<bool>(2));
    ASSERT_FALSE(result.at<bool>(3));
    ASSERT_TRUE(result.at<bool>(4));
    ASSERT_FALSE(result.at<bool>(5));
    ASSERT_FALSE(result.at<bool>(6));
    ASSERT_FALSE(result.at<bool>(7));
    ASSERT_FALSE(result.at<bool>(8));
}

TEST_F(Box_Comp_Value_Test_Fixture, NotEqual)
{
    auto const err_result = src.ne(5);
    ASSERT_EQ(E_SUCCESS, err_result.code);

    auto const & result = err_result.value;
    ASSERT_TRUE(result.is_bool());
    ASSERT_TRUE(result.is_device_cpu());
    ASSERT_EQ(0, result.ext0());
    ASSERT_EQ(0, result.ext1());
    ASSERT_EQ(0, result.ext2());
    ASSERT_EQ(0, result.ext3());
    ASSERT_EQ(2, result.rank());
    ASSERT_EQ(3, result.dim(0));
    ASSERT_EQ(3, result.dim(1));
    ASSERT_EQ(9, result.size());

    ASSERT_TRUE(result.at<bool>(0));
    ASSERT_TRUE(result.at<bool>(1));
    ASSERT_TRUE(result.at<bool>(2));
    ASSERT_TRUE(result.at<bool>(3));
    ASSERT_FALSE(result.at<bool>(4));
    ASSERT_TRUE(result.at<bool>(5));
    ASSERT_TRUE(result.at<bool>(6));
    ASSERT_TRUE(result.at<bool>(7));
    ASSERT_TRUE(result.at<bool>(8));
}

TEST_F(Box_Comp_Value_Test_Fixture, LessThan)
{
    auto const err_result = src.lt(5);
    ASSERT_EQ(E_SUCCESS, err_result.code);

    auto const & result = err_result.value;
    ASSERT_TRUE(result.is_bool());
    ASSERT_TRUE(result.is_device_cpu());
    ASSERT_EQ(0, result.ext0());
    ASSERT_EQ(0, result.ext1());
    ASSERT_EQ(0, result.ext2());
    ASSERT_EQ(0, result.ext3());
    ASSERT_EQ(2, result.rank());
    ASSERT_EQ(3, result.dim(0));
    ASSERT_EQ(3, result.dim(1));
    ASSERT_EQ(9, result.size());

    ASSERT_TRUE(result.at<bool>(0));
    ASSERT_TRUE(result.at<bool>(1));
    ASSERT_TRUE(result.at<bool>(2));
    ASSERT_TRUE(result.at<bool>(3));
    ASSERT_FALSE(result.at<bool>(4));
    ASSERT_FALSE(result.at<bool>(5));
    ASSERT_FALSE(result.at<bool>(6));
    ASSERT_FALSE(result.at<bool>(7));
    ASSERT_FALSE(result.at<bool>(8));
}

TEST_F(Box_Comp_Value_Test_Fixture, LessEqual)
{
    auto const err_result = src.le(5);
    ASSERT_EQ(E_SUCCESS, err_result.code);

    auto const & result = err_result.value;
    ASSERT_TRUE(result.is_bool());
    ASSERT_TRUE(result.is_device_cpu());
    ASSERT_EQ(0, result.ext0());
    ASSERT_EQ(0, result.ext1());
    ASSERT_EQ(0, result.ext2());
    ASSERT_EQ(0, result.ext3());
    ASSERT_EQ(2, result.rank());
    ASSERT_EQ(3, result.dim(0));
    ASSERT_EQ(3, result.dim(1));
    ASSERT_EQ(9, result.size());

    ASSERT_TRUE(result.at<bool>(0));
    ASSERT_TRUE(result.at<bool>(1));
    ASSERT_TRUE(result.at<bool>(2));
    ASSERT_TRUE(result.at<bool>(3));
    ASSERT_TRUE(result.at<bool>(4));
    ASSERT_FALSE(result.at<bool>(5));
    ASSERT_FALSE(result.at<bool>(6));
    ASSERT_FALSE(result.at<bool>(7));
    ASSERT_FALSE(result.at<bool>(8));
}

TEST_F(Box_Comp_Value_Test_Fixture, GreaterThan)
{
    auto const err_result = src.gt(5);
    ASSERT_EQ(E_SUCCESS, err_result.code);

    auto const & result = err_result.value;
    ASSERT_TRUE(result.is_bool());
    ASSERT_TRUE(result.is_device_cpu());
    ASSERT_EQ(0, result.ext0());
    ASSERT_EQ(0, result.ext1());
    ASSERT_EQ(0, result.ext2());
    ASSERT_EQ(0, result.ext3());
    ASSERT_EQ(2, result.rank());
    ASSERT_EQ(3, result.dim(0));
    ASSERT_EQ(3, result.dim(1));
    ASSERT_EQ(9, result.size());

    ASSERT_FALSE(result.at<bool>(0));
    ASSERT_FALSE(result.at<bool>(1));
    ASSERT_FALSE(result.at<bool>(2));
    ASSERT_FALSE(result.at<bool>(3));
    ASSERT_FALSE(result.at<bool>(4));
    ASSERT_TRUE(result.at<bool>(5));
    ASSERT_TRUE(result.at<bool>(6));
    ASSERT_TRUE(result.at<bool>(7));
    ASSERT_TRUE(result.at<bool>(8));
}

TEST_F(Box_Comp_Value_Test_Fixture, GreaterEqual)
{
    auto const err_result = src.ge(5);
    ASSERT_EQ(E_SUCCESS, err_result.code);

    auto const & result = err_result.value;
    ASSERT_TRUE(result.is_bool());
    ASSERT_TRUE(result.is_device_cpu());
    ASSERT_EQ(0, result.ext0());
    ASSERT_EQ(0, result.ext1());
    ASSERT_EQ(0, result.ext2());
    ASSERT_EQ(0, result.ext3());
    ASSERT_EQ(2, result.rank());
    ASSERT_EQ(3, result.dim(0));
    ASSERT_EQ(3, result.dim(1));
    ASSERT_EQ(9, result.size());

    ASSERT_FALSE(result.at<bool>(0));
    ASSERT_FALSE(result.at<bool>(1));
    ASSERT_FALSE(result.at<bool>(2));
    ASSERT_FALSE(result.at<bool>(3));
    ASSERT_TRUE(result.at<bool>(4));
    ASSERT_TRUE(result.at<bool>(5));
    ASSERT_TRUE(result.at<bool>(6));
    ASSERT_TRUE(result.at<bool>(7));
    ASSERT_TRUE(result.at<bool>(8));
}

