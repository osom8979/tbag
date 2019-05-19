/**
 * @file   BoxTest.cpp
 * @brief  Box class tester.
 * @author zer0
 * @date   2019-05-16
 */

#include <gtest/gtest.h>
#include <libtbag/box/Box.hpp>

using namespace libtbag;
using namespace libtbag::box;

TEST(BoxTest, Default)
{
    Box b0;
    ASSERT_TRUE(b0);

    Box b1_move;
    ASSERT_TRUE(b1_move);
    auto const * b1_data = b1_move.get();
    Box b2 = std::move(b1_move);
    ASSERT_TRUE(b2);
    auto const * b2_data = b2.get();
    ASSERT_EQ(b1_data, b2_data);

    Box b3;
    Box b4 = b3;
    ASSERT_TRUE(b3);
    ASSERT_TRUE(b4);
    auto const * b3_data = b3.get();
    auto const * b4_data = b4.get();
    ASSERT_EQ(b3_data, b4_data);

    Box b5;
    ASSERT_TRUE(b5);
    Box b6(nullptr);
    ASSERT_FALSE(b6);
    b6 = b5;
    auto const * b5_data = b5.get();
    auto const * b6_data = b6.get();
    ASSERT_EQ(b5_data, b6_data);
}

TEST(BoxTest, Reshape)
{
    auto b0 = Box::shape<int>(2, 3);
    ASSERT_TRUE(b0.is_device_cpu());
    ASSERT_TRUE(b0.is_si32());
    ASSERT_EQ(2, b0.rank());
    ASSERT_EQ(2*3, b0.size());
    ASSERT_EQ(2, b0.dim(0));
    ASSERT_EQ(3, b0.dim(1));

    b0.reshape<double>(2);
    ASSERT_TRUE(b0.is_device_cpu());
    ASSERT_TRUE(b0.is_fp64());
    ASSERT_EQ(1, b0.rank());
    ASSERT_EQ(2, b0.size());
    ASSERT_EQ(2, b0.dim(0));
}

TEST(BoxTest, Assign)
{
    Box b0 = {10, 20};
    ASSERT_TRUE(b0.is_device_cpu());
    ASSERT_TRUE(b0.is_si32());
    ASSERT_EQ(1, b0.rank());
    ASSERT_EQ(2, b0.size());
    ASSERT_EQ(2, b0.dim(0));
    ASSERT_EQ(10, b0.at<si32>(0));
    ASSERT_EQ(20, b0.at<si32>(1));

    Box b1 = {{10.0f, 20.0f}, {30.0f, 40.0f}, {50.0f, 60.0f}};
    ASSERT_TRUE(b1.is_device_cpu());
    ASSERT_TRUE(b1.is_fp32());
    ASSERT_EQ(2, b1.rank());
    ASSERT_EQ(6, b1.size());
    ASSERT_EQ(3, b1.dim(0));
    ASSERT_EQ(2, b1.dim(1));
    ASSERT_EQ(10.0f, b1.at<fp32>(0, 0));
    ASSERT_EQ(20.0f, b1.at<fp32>(0, 1));
    ASSERT_EQ(30.0f, b1.at<fp32>(1, 0));
    ASSERT_EQ(40.0f, b1.at<fp32>(1, 1));
    ASSERT_EQ(50.0f, b1.at<fp32>(2, 0));
    ASSERT_EQ(60.0f, b1.at<fp32>(2, 1));

    Box b2 = {{{10.0},{20.0}},{{30.0},{40.0}},{{50.0},{60.0}}};
    ASSERT_TRUE(b2.is_device_cpu());
    ASSERT_TRUE(b2.is_fp64());
    ASSERT_EQ(3, b2.rank());
    ASSERT_EQ(6, b2.size());
    ASSERT_EQ(3, b2.dim(0));
    ASSERT_EQ(2, b2.dim(1));
    ASSERT_EQ(1, b2.dim(2));
    ASSERT_EQ(10.0f, b2.at<fp64>(0, 0, 0));
    ASSERT_EQ(20.0f, b2.at<fp64>(0, 1, 0));
    ASSERT_EQ(30.0f, b2.at<fp64>(1, 0, 0));
    ASSERT_EQ(40.0f, b2.at<fp64>(1, 1, 0));
    ASSERT_EQ(50.0f, b2.at<fp64>(2, 0, 0));
    ASSERT_EQ(60.0f, b2.at<fp64>(2, 1, 0));

    Box b3 = {{{{10},{20}},{{30},{40}},{{50},{60}}},
              {{{11},{21}},{{31},{41}},{{51},{61}}}};
    ASSERT_TRUE(b3.is_device_cpu());
    ASSERT_TRUE(b3.is_si32());
    ASSERT_EQ(4, b3.rank());
    ASSERT_EQ(12, b3.size());
    ASSERT_EQ(2, b3.dim(0));
    ASSERT_EQ(3, b3.dim(1));
    ASSERT_EQ(2, b3.dim(2));
    ASSERT_EQ(1, b3.dim(3));
    ASSERT_EQ(10, b3.at<si32>(0, 0, 0, 0));
    ASSERT_EQ(20, b3.at<si32>(0, 0, 1, 0));
    ASSERT_EQ(30, b3.at<si32>(0, 1, 0, 0));
    ASSERT_EQ(40, b3.at<si32>(0, 1, 1, 0));
    ASSERT_EQ(50, b3.at<si32>(0, 2, 0, 0));
    ASSERT_EQ(60, b3.at<si32>(0, 2, 1, 0));
    ASSERT_EQ(11, b3.at<si32>(1, 0, 0, 0));
    ASSERT_EQ(21, b3.at<si32>(1, 0, 1, 0));
    ASSERT_EQ(31, b3.at<si32>(1, 1, 0, 0));
    ASSERT_EQ(41, b3.at<si32>(1, 1, 1, 0));
    ASSERT_EQ(51, b3.at<si32>(1, 2, 0, 0));
    ASSERT_EQ(61, b3.at<si32>(1, 2, 1, 0));
}

