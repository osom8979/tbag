/**
 * @file   DimensionTest.cpp
 * @brief  Dimension class tester.
 * @author zer0
 * @date   2018-07-28
 */

#include <gtest/gtest.h>
#include <libtbag/container/bag/Dimension.hpp>

using namespace libtbag;
using namespace libtbag::container;
using namespace libtbag::container::bag;

TEST(DimensionTest, Constructor)
{
    Dimension<4> dims0;
    ASSERT_EQ(0, dims0.size());

    Dimension<4> dims1 = {1, 2, 3, 4};
    Dimension<4> dims2 = dims1;

    ASSERT_EQ(4, dims1.size());
    ASSERT_EQ(1, dims1[0]);
    ASSERT_EQ(2, dims1[1]);
    ASSERT_EQ(3, dims1[2]);
    ASSERT_EQ(4, dims1[3]);

    ASSERT_EQ(4, dims2.size());
    ASSERT_EQ(1, dims2[0]);
    ASSERT_EQ(2, dims2[1]);
    ASSERT_EQ(3, dims2[2]);
    ASSERT_EQ(4, dims2[3]);

    Dimension<4> dims3 = std::move(dims1);
    ASSERT_EQ(0, dims1.size());

    ASSERT_EQ(4, dims3.size());
    ASSERT_EQ(1, dims3[0]);
    ASSERT_EQ(2, dims3[1]);
    ASSERT_EQ(3, dims3[2]);
    ASSERT_EQ(4, dims3[3]);

    Dimension<4> dims4;
    ASSERT_EQ(4, dims4.buffer_size());
    ASSERT_NE(0, dims4.max_size());
    ASSERT_EQ(0, dims4.size());
    ASSERT_TRUE(dims4.empty());
}

TEST(DimensionTest, Compare)
{
    Dimension<4> dims0;
    Dimension<4> dims1 = {1, 2, 3, 4};
    Dimension<4> dims2 = dims1;

    ASSERT_NE(dims0, dims1);
    ASSERT_EQ(dims1, dims2);
}

TEST(DimensionTest, Resize)
{
    Dimension<4> dims;
    ASSERT_EQ(0, dims.size());

    std::vector<int> dims_resize = {3, 4};
    dims.resize(dims_resize.begin(), dims_resize.end());
    ASSERT_EQ(2, dims.size());
    ASSERT_EQ(3, dims[0]);
    ASSERT_EQ(4, dims[1]);

    dims_resize.clear();
    dims.resize(dims_resize.begin(), dims_resize.end());
    ASSERT_EQ(0, dims.size());

    dims_resize = {1, 2, 3, 4, 5};
    dims.resize(dims_resize.begin(), dims_resize.end());
    ASSERT_EQ(4, dims.size());
    ASSERT_EQ(1, dims[0]);
    ASSERT_EQ(2, dims[1]);
    ASSERT_EQ(3, dims[2]);
    ASSERT_EQ(4, dims[3]);

    Dimension<4>::DimValue array_resize[3] = {5, 6, 7};
    dims.resize(array_resize, sizeof(array_resize) / sizeof(array_resize[0]));
    ASSERT_EQ(3, dims.size());
    ASSERT_EQ(5, dims[0]);
    ASSERT_EQ(6, dims[1]);
    ASSERT_EQ(7, dims[2]);

    dims.resize({1, 2, 3});
    ASSERT_EQ(3, dims.size());
    ASSERT_EQ(1, dims[0]);
    ASSERT_EQ(2, dims[1]);
    ASSERT_EQ(3, dims[2]);

    dims = {8, 9};
    ASSERT_EQ(2, dims.size());
    ASSERT_EQ(8, dims[0]);
    ASSERT_EQ(9, dims[1]);
}

TEST(DimensionTest, Total)
{
    Dimension<4> dims;
    ASSERT_EQ(0, dims.total());

    dims = {10};
    ASSERT_EQ(10, dims.total());

    dims = {2, 3};
    ASSERT_EQ(2*3, dims.total());

    dims = {2, 3, 4, 5};
    // clang-format off
    ASSERT_EQ(2*3*4*5, dims.total());
    ASSERT_EQ(  3*4*5, dims.subTotal(0));
    ASSERT_EQ(    4*5, dims.subTotal(1));
    ASSERT_EQ(      5, dims.subTotal(2));
    // clang-format on
}

TEST(DimensionTest, Offset)
{
    Dimension<4> dims = {2, 3, 4, 5};
    std::vector<int> dims_offset;
    Dimension<4>::DimValue offset;

    dims_offset = {0, 0, 0, 0};
    offset = dims.offset(dims_offset.begin(), dims_offset.end());
    ASSERT_EQ(0, offset);

    dims_offset = {0};
    offset = dims.offset(dims_offset.begin(), dims_offset.end());
    ASSERT_EQ(0, offset);

    dims_offset = {1};
    offset = dims.offset(dims_offset.begin(), dims_offset.end());
    ASSERT_EQ(3*4*5, offset);

    Dimension<4>::DimValue array_offset[2] = {0, 1};
    offset = dims.offset(array_offset, sizeof(array_offset) / sizeof(array_offset[0]));
    ASSERT_EQ(4*5, offset);

    ASSERT_EQ(5, dims.offset({0, 0, 1}));
    ASSERT_EQ(1, dims.offset({0, 0, 0, 1}));
    ASSERT_EQ(1, dims.offset({0, 0, 0, 1, /* Overflow */1}));
    ASSERT_EQ((3*4*5) + (4*5), dims.offset({1, 1}));
    ASSERT_EQ((3*4*5) + (4*5) + (5) + (1), dims.offset({1, 1, 1, 1}));
    ASSERT_EQ((3*4*5) + (4*5) + (5*0) + (1), dims.offset({1, 1, 0, 1}));
    ASSERT_EQ((3*4*5*0) + (4*5) + (5*0) + (1), dims.offset({0, 1, 0, 1}));
}

