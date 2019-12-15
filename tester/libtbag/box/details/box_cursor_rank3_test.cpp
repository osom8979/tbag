/**
 * @file   box_cursor_rank3_test.cpp
 * @brief  box_cursor_rank3 class tester.
 * @author zer0
 * @date   2019-12-14
 */

#include <gtest/gtest.h>
#include <libtbag/box/details/box_api.hpp>

using namespace libtbag;
using namespace libtbag::box;
using namespace libtbag::box::details;

static int const DIM0 = 3;
static int const DIM1 = 2;
static int const DIM2 = 2;

struct box_cursor_rank3_fixture : public testing::Test
{
    box_data box;

    void SetUp() override
    {
        box.resize_args(BT_INT32, BD_CPU, nullptr, 3, DIM0, DIM1, DIM2);
        for (int i = 0; i < DIM0*DIM1*DIM2; ++i) {
            box.set_data(&i, BT_INT32, BD_CPU, nullptr, i);
        }
    }

    void TearDown() override
    {
        // EMPTY.
    }
};

TEST_F(box_cursor_rank3_fixture, Default)
{
    ASSERT_EQ(box.type, BT_INT32);
    ASSERT_EQ(box.device, BD_CPU);
    ASSERT_EQ(box.ext[0], 0);
    ASSERT_EQ(box.ext[1], 0);
    ASSERT_EQ(box.ext[2], 0);
    ASSERT_EQ(box.ext[3], 0);
    ASSERT_NE(box.data, nullptr);
    ASSERT_GE(box.total_data_byte, DIM0*DIM1*DIM2*sizeof(si32));
    ASSERT_EQ(box.size, DIM0*DIM1*DIM2);
    ASSERT_NE(box.dims, nullptr);
    ASSERT_GE(box.total_dims_byte, 3);
    ASSERT_EQ(box.rank, 3);
}

TEST_F(box_cursor_rank3_fixture, Cursor_all__all__all)
{
    auto err_cursor0 = box.init_cursor();
    ASSERT_TRUE(err_cursor0);
    auto cursor0 = err_cursor0.value;

    auto err_cursor1 = cursor0.init_cursor(1);
    ASSERT_TRUE(err_cursor1);
    auto cursor1 = err_cursor1.value;

    auto err_cursor2 = cursor1.init_cursor(2);
    ASSERT_TRUE(err_cursor2);
    auto cursor2 = err_cursor2.value;

    ASSERT_EQ(0, *(si32*)(cursor2.begin));
    ASSERT_TRUE(cursor2.next());
    ASSERT_EQ(1, *(si32*)(cursor2.begin));
    ASSERT_FALSE(cursor2.next());
    ASSERT_TRUE(cursor1.next());

    err_cursor2 = cursor1.init_cursor(2);
    ASSERT_TRUE(err_cursor2);
    cursor2 = err_cursor2.value;

    ASSERT_EQ(2, *(si32*)(cursor2.begin));
    ASSERT_TRUE(cursor2.next());
    ASSERT_EQ(3, *(si32*)(cursor2.begin));
    ASSERT_FALSE(cursor2.next());
    ASSERT_FALSE(cursor1.next());
    ASSERT_TRUE(cursor0.next());

    err_cursor1 = cursor0.init_cursor(1);
    ASSERT_TRUE(err_cursor1);
    cursor1 = err_cursor1.value;

    err_cursor2 = cursor1.init_cursor(2);
    ASSERT_TRUE(err_cursor2);
    cursor2 = err_cursor2.value;

    ASSERT_EQ(4, *(si32*)(cursor2.begin));
    ASSERT_TRUE(cursor2.next());
    ASSERT_EQ(5, *(si32*)(cursor2.begin));
    ASSERT_FALSE(cursor2.next());
    ASSERT_TRUE(cursor1.next());

    err_cursor2 = cursor1.init_cursor(2);
    ASSERT_TRUE(err_cursor2);
    cursor2 = err_cursor2.value;

    ASSERT_EQ(6, *(si32*)(cursor2.begin));
    ASSERT_TRUE(cursor2.next());
    ASSERT_EQ(7, *(si32*)(cursor2.begin));
    ASSERT_FALSE(cursor2.next());
    ASSERT_FALSE(cursor1.next());
    ASSERT_TRUE(cursor0.next());

    err_cursor1 = cursor0.init_cursor(1);
    ASSERT_TRUE(err_cursor1);
    cursor1 = err_cursor1.value;

    err_cursor2 = cursor1.init_cursor(2);
    ASSERT_TRUE(err_cursor2);
    cursor2 = err_cursor2.value;

    ASSERT_EQ(8, *(si32*)(cursor2.begin));
    ASSERT_TRUE(cursor2.next());
    ASSERT_EQ(9, *(si32*)(cursor2.begin));
    ASSERT_FALSE(cursor2.next());
    ASSERT_TRUE(cursor1.next());

    err_cursor2 = cursor1.init_cursor(2);
    ASSERT_TRUE(err_cursor2);
    cursor2 = err_cursor2.value;

    ASSERT_EQ(10, *(si32*)(cursor2.begin));
    ASSERT_TRUE(cursor2.next());
    ASSERT_EQ(11, *(si32*)(cursor2.begin));
    ASSERT_FALSE(cursor2.next());
    ASSERT_FALSE(cursor1.next());
    ASSERT_FALSE(cursor0.next());
}

