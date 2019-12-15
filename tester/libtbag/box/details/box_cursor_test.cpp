/**
 * @file   box_cursor_test.cpp
 * @brief  box_cursor class tester.
 * @author zer0
 * @date   2019-12-14
 */

#include <gtest/gtest.h>
#include <libtbag/box/details/box_api.hpp>

using namespace libtbag;
using namespace libtbag::box;
using namespace libtbag::box::details;

static int const DIM0 = 4;
static int const DIM1 = 3;
static int const DIM2 = 2;

struct box_cursor_fixture : public testing::Test
{
    box_data box;

    void SetUp() override
    {
        box.resize_args(BT_INT32, BD_CPU, nullptr, 3, DIM0, DIM1, DIM2);
        for (int i = 0; i < DIM0*DIM1*DIM2; ++i) {
            box_data_set(&box, &i, BT_INT32, BD_CPU, nullptr, i);
        }
    }

    void TearDown() override
    {
        // EMPTY.
    }
};

TEST_F(box_cursor_fixture, Default)
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

TEST_F(box_cursor_fixture, Cursor_all__all__all)
{
    int i = 0;
    int loop_counter0 = 0;

    auto err_cursor0 = box.init_cursor();
    ASSERT_TRUE(err_cursor0);
    auto cursor0 = err_cursor0.value;

    do {
        int loop_counter1 = 0;

        auto err_cursor1 = cursor0.init_cursor(1);
        ASSERT_TRUE(err_cursor1);
        auto cursor1 = err_cursor1.value;

        do {
            int loop_counter2 = 0;

            auto err_cursor2 = cursor1.init_cursor(2);
            ASSERT_TRUE(err_cursor2);
            auto cursor2 = err_cursor2.value;

            do {
                ASSERT_EQ(i, *(si32*)(cursor2.begin));
                ++i;
                ++loop_counter2;

            } while (cursor2.next());
            ASSERT_EQ(2, loop_counter2);
            ++loop_counter1;

        } while (cursor1.next());
        ASSERT_EQ(3, loop_counter1);
        ++loop_counter0;

    } while (cursor0.next());
    ASSERT_EQ(4, loop_counter0);
    ASSERT_EQ(24, i);
}

