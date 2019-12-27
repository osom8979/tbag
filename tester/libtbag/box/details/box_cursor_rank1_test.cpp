/**
 * @file   box_cursor_rank1_test.cpp
 * @brief  box_cursor_rank1 class tester.
 * @author zer0
 * @date   2019-12-14
 */

#include <gtest/gtest.h>
#include <libtbag/box/details/box_api.hpp>

using namespace libtbag;
using namespace libtbag::box;
using namespace libtbag::box::details;

static int const DIM0 = 10;

struct box_cursor_rank1_fixture : public testing::Test
{
    box_data box;

     void SetUp() override
     {
         box.resize_args(BT_INT32, BD_CPU, nullptr, 1, DIM0);
         for (int i = 0; i < DIM0; ++i) {
             box.set_data(&i, BT_INT32, BD_CPU, nullptr, i);
         }
     }

     void TearDown() override
     {
         // EMPTY.
     }
};

TEST_F(box_cursor_rank1_fixture, Default)
{
    ASSERT_EQ(box.type, BT_INT32);
    ASSERT_EQ(box.device, BD_CPU);
    ASSERT_EQ(box.ext[0], 0);
    ASSERT_EQ(box.ext[1], 0);
    ASSERT_EQ(box.ext[2], 0);
    ASSERT_EQ(box.ext[3], 0);
    ASSERT_NE(box.data, nullptr);
    ASSERT_GE(box.total_data_byte, DIM0*sizeof(si32));
    ASSERT_EQ(box.size, DIM0);
    ASSERT_NE(box.dims, nullptr);
    ASSERT_GE(box.total_dims_byte, 1);
    ASSERT_EQ(box.rank, 1);
}

TEST_F(box_cursor_rank1_fixture, Cursor_all)
{
    auto err_cursor = box.init_cursor();
    ASSERT_TRUE(err_cursor);
    auto cursor = err_cursor.value;

    ASSERT_EQ(0, *(si32*)(cursor.begin));
    ASSERT_TRUE(cursor.next());
    ASSERT_EQ(1, *(si32*)(cursor.begin));
    ASSERT_TRUE(cursor.next());
    ASSERT_EQ(2, *(si32*)(cursor.begin));
    ASSERT_TRUE(cursor.next());
    ASSERT_EQ(3, *(si32*)(cursor.begin));
    ASSERT_TRUE(cursor.next());
    ASSERT_EQ(4, *(si32*)(cursor.begin));
    ASSERT_TRUE(cursor.next());
    ASSERT_EQ(5, *(si32*)(cursor.begin));
    ASSERT_TRUE(cursor.next());
    ASSERT_EQ(6, *(si32*)(cursor.begin));
    ASSERT_TRUE(cursor.next());
    ASSERT_EQ(7, *(si32*)(cursor.begin));
    ASSERT_TRUE(cursor.next());
    ASSERT_EQ(8, *(si32*)(cursor.begin));
    ASSERT_TRUE(cursor.next());
    ASSERT_EQ(9, *(si32*)(cursor.begin));
    ASSERT_FALSE(cursor.next());
}

TEST_F(box_cursor_rank1_fixture, Cursor_m1_0_m1)
{
    auto err_cursor = box.init_cursor(0, -1, 0, -1);
    ASSERT_TRUE(err_cursor);
    auto cursor = err_cursor.value;

    ASSERT_EQ(9, *(si32*)(cursor.begin));
    ASSERT_TRUE(cursor.next());
    ASSERT_EQ(8, *(si32*)(cursor.begin));
    ASSERT_TRUE(cursor.next());
    ASSERT_EQ(7, *(si32*)(cursor.begin));
    ASSERT_TRUE(cursor.next());
    ASSERT_EQ(6, *(si32*)(cursor.begin));
    ASSERT_TRUE(cursor.next());
    ASSERT_EQ(5, *(si32*)(cursor.begin));
    ASSERT_TRUE(cursor.next());
    ASSERT_EQ(4, *(si32*)(cursor.begin));
    ASSERT_TRUE(cursor.next());
    ASSERT_EQ(3, *(si32*)(cursor.begin));
    ASSERT_TRUE(cursor.next());
    ASSERT_EQ(2, *(si32*)(cursor.begin));
    ASSERT_TRUE(cursor.next());
    ASSERT_EQ(1, *(si32*)(cursor.begin));
    ASSERT_FALSE(cursor.next());
}

TEST_F(box_cursor_rank1_fixture, Cursor_nop_nop_m1)
{
    auto err_cursor = box.init_cursor(0, box_nop, box_nop, -1);
    ASSERT_TRUE(err_cursor);
    auto cursor = err_cursor.value;

    ASSERT_EQ(9, *(si32*)(cursor.begin));
    ASSERT_TRUE(cursor.next());
    ASSERT_EQ(8, *(si32*)(cursor.begin));
    ASSERT_TRUE(cursor.next());
    ASSERT_EQ(7, *(si32*)(cursor.begin));
    ASSERT_TRUE(cursor.next());
    ASSERT_EQ(6, *(si32*)(cursor.begin));
    ASSERT_TRUE(cursor.next());
    ASSERT_EQ(5, *(si32*)(cursor.begin));
    ASSERT_TRUE(cursor.next());
    ASSERT_EQ(4, *(si32*)(cursor.begin));
    ASSERT_TRUE(cursor.next());
    ASSERT_EQ(3, *(si32*)(cursor.begin));
    ASSERT_TRUE(cursor.next());
    ASSERT_EQ(2, *(si32*)(cursor.begin));
    ASSERT_TRUE(cursor.next());
    ASSERT_EQ(1, *(si32*)(cursor.begin));
    ASSERT_TRUE(cursor.next());
    ASSERT_EQ(0, *(si32*)(cursor.begin));
    ASSERT_FALSE(cursor.next());
}

TEST_F(box_cursor_rank1_fixture, Cursor_0_m1)
{
    auto err_cursor = box.init_cursor(0, 0, -1);
    ASSERT_TRUE(err_cursor);
    auto cursor = err_cursor.value;

    ASSERT_EQ(0, *(si32*)(cursor.begin));
    ASSERT_TRUE(cursor.next());
    ASSERT_EQ(1, *(si32*)(cursor.begin));
    ASSERT_TRUE(cursor.next());
    ASSERT_EQ(2, *(si32*)(cursor.begin));
    ASSERT_TRUE(cursor.next());
    ASSERT_EQ(3, *(si32*)(cursor.begin));
    ASSERT_TRUE(cursor.next());
    ASSERT_EQ(4, *(si32*)(cursor.begin));
    ASSERT_TRUE(cursor.next());
    ASSERT_EQ(5, *(si32*)(cursor.begin));
    ASSERT_TRUE(cursor.next());
    ASSERT_EQ(6, *(si32*)(cursor.begin));
    ASSERT_TRUE(cursor.next());
    ASSERT_EQ(7, *(si32*)(cursor.begin));
    ASSERT_TRUE(cursor.next());
    ASSERT_EQ(8, *(si32*)(cursor.begin));
    ASSERT_FALSE(cursor.next());
}

TEST_F(box_cursor_rank1_fixture, Cursor_m2_m1)
{
    auto err_cursor = box.init_cursor(0, -2, -1);
    ASSERT_TRUE(err_cursor);
    auto cursor = err_cursor.value;

    ASSERT_EQ(8, *(si32*)(cursor.begin));
    ASSERT_FALSE(cursor.next());
}

TEST_F(box_cursor_rank1_fixture, Cursor_m8_0_m1)
{
    auto err_cursor = box.init_cursor(0, -8, 0, -1);
    ASSERT_TRUE(err_cursor);
    auto cursor = err_cursor.value;

    ASSERT_EQ(2, *(si32*)(cursor.begin));
    ASSERT_TRUE(cursor.next());
    ASSERT_EQ(1, *(si32*)(cursor.begin));
    ASSERT_FALSE(cursor.next());
}

TEST_F(box_cursor_rank1_fixture, Cursor_5)
{
    auto err_cursor = box.init_cursor(0, 5);
    ASSERT_TRUE(err_cursor);
    auto cursor = err_cursor.value;

    ASSERT_EQ(5, *(si32*)(cursor.begin));
    ASSERT_TRUE(cursor.next());
    ASSERT_EQ(6, *(si32*)(cursor.begin));
    ASSERT_TRUE(cursor.next());
    ASSERT_EQ(7, *(si32*)(cursor.begin));
    ASSERT_TRUE(cursor.next());
    ASSERT_EQ(8, *(si32*)(cursor.begin));
    ASSERT_TRUE(cursor.next());
    ASSERT_EQ(9, *(si32*)(cursor.begin));
    ASSERT_FALSE(cursor.next());
}

TEST_F(box_cursor_rank1_fixture, Cursor_5_8)
{
    auto err_cursor = box.init_cursor(0, 5, 8);
    ASSERT_TRUE(err_cursor);
    auto cursor = err_cursor.value;

    ASSERT_EQ(5, *(si32*)(cursor.begin));
    ASSERT_TRUE(cursor.next());
    ASSERT_EQ(6, *(si32*)(cursor.begin));
    ASSERT_TRUE(cursor.next());
    ASSERT_EQ(7, *(si32*)(cursor.begin));
    ASSERT_FALSE(cursor.next());
}

TEST_F(box_cursor_rank1_fixture, Cursor_5_8_2)
{
    auto err_cursor = box.init_cursor(0, 5, 8, 2);
    ASSERT_TRUE(err_cursor);
    auto cursor = err_cursor.value;

    ASSERT_EQ(5, *(si32*)(cursor.begin));
    ASSERT_TRUE(cursor.next());
    ASSERT_EQ(7, *(si32*)(cursor.begin));
    ASSERT_FALSE(cursor.next());
}

TEST_F(box_cursor_rank1_fixture, Cursor_5_8_3)
{
    auto err_cursor = box.init_cursor(0, 5, 8, 3);
    ASSERT_TRUE(err_cursor);
    auto cursor = err_cursor.value;

    ASSERT_EQ(5, *(si32*)(cursor.begin));
    ASSERT_FALSE(cursor.next());
}

TEST_F(box_cursor_rank1_fixture, Cursor_1_4_10)
{
    auto err_cursor = box.init_cursor(0, 1, 4, 10);
    ASSERT_TRUE(err_cursor);
    auto cursor = err_cursor.value;

    ASSERT_EQ(1, *(si32*)(cursor.begin));
    ASSERT_FALSE(cursor.next());
}

TEST_F(box_cursor_rank1_fixture, Cursor_1_4_m1)
{
    auto err_cursor = box.init_cursor(0, 1, 4, -1);
    ASSERT_FALSE(err_cursor);
}

TEST_F(box_cursor_rank1_fixture, Cursor_4_1)
{
    auto err_cursor = box.init_cursor(0, 4, 1);
    ASSERT_FALSE(err_cursor);
}

TEST_F(box_cursor_rank1_fixture, Cursor_4_1_m1)
{
    auto err_cursor = box.init_cursor(0, 4, 1, -1);
    ASSERT_TRUE(err_cursor);
    auto cursor = err_cursor.value;

    ASSERT_EQ(4, *(si32*)(cursor.begin));
    ASSERT_TRUE(cursor.next());
    ASSERT_EQ(3, *(si32*)(cursor.begin));
    ASSERT_TRUE(cursor.next());
    ASSERT_EQ(2, *(si32*)(cursor.begin));
    ASSERT_FALSE(cursor.next());
}

TEST_F(box_cursor_rank1_fixture, Cursor_4_1_m2)
{
    auto err_cursor = box.init_cursor(0, 4, 1, -2);
    ASSERT_TRUE(err_cursor);
    auto cursor = err_cursor.value;

    ASSERT_EQ(4, *(si32*)(cursor.begin));
    ASSERT_TRUE(cursor.next());
    ASSERT_EQ(2, *(si32*)(cursor.begin));
    ASSERT_FALSE(cursor.next());
}

TEST_F(box_cursor_rank1_fixture, Cursor_4_1_m10)
{
    auto err_cursor = box.init_cursor(0, 4, 1, -10);
    ASSERT_TRUE(err_cursor);
    auto cursor = err_cursor.value;

    ASSERT_EQ(4, *(si32*)(cursor.begin));
    ASSERT_FALSE(cursor.next());
}

