/**
 * @file   MatrixTest.cpp
 * @brief  Matrix class tester.
 * @author zer0
 * @date   2019-02-20
 */

#include <gtest/gtest.h>
#include <libtbag/geometry/Matrix.hpp>

using namespace libtbag;
using namespace libtbag::geometry;

TEST(MatrixTest, Default)
{
    Matrix4x4 mat = { 11, 12, 13, 14,
                      21, 22, 23, 24,
                      31, 32, 33, 34,
                      41, 42, 43, 44 };
    ASSERT_EQ(11, mat.at(0, 0));
    ASSERT_EQ(12, mat.at(0, 1));
    ASSERT_EQ(13, mat.at(0, 2));
    ASSERT_EQ(14, mat.at(0, 3));

    ASSERT_EQ(21, mat.at(1, 0));
    ASSERT_EQ(22, mat.at(1, 1));
    ASSERT_EQ(23, mat.at(1, 2));
    ASSERT_EQ(24, mat.at(1, 3));

    ASSERT_EQ(31, mat.at(2, 0));
    ASSERT_EQ(32, mat.at(2, 1));
    ASSERT_EQ(33, mat.at(2, 2));
    ASSERT_EQ(34, mat.at(2, 3));

    ASSERT_EQ(41, mat.at(3, 0));
    ASSERT_EQ(42, mat.at(3, 1));
    ASSERT_EQ(43, mat.at(3, 2));
    ASSERT_EQ(44, mat.at(3, 3));

    ASSERT_EQ(4*4, mat.size());
}

