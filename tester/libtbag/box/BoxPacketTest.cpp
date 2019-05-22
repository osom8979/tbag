/**
 * @file   BoxPacketTest.cpp
 * @brief  BoxPacket class tester.
 * @author zer0
 * @date   2019-05-22
 */

#include <gtest/gtest.h>
#include <libtbag/box/BoxPacket.hpp>

using namespace libtbag;
using namespace libtbag::box;
using namespace libtbag::box::details;

TEST(BoxPacketTest, Default)
{
    box_data box;
    box_clear(&box);

    ASSERT_EQ(E_SUCCESS, box_resize_args(&box, BT_INT32, BOX_DEVICE_CPU, nullptr, 3, 4, 3, 2));
    ui32 i = 0;
    for (i = 0; i < 24; ++i) {
        box_data_set(&box, &i, BT_INT32, BOX_DEVICE_CPU, nullptr, i);
        ASSERT_EQ(i, ((si32*)box.data)[i]);
    }

    ASSERT_EQ(BT_INT32, box.type);
    ASSERT_EQ(BOX_DEVICE_CPU, box.device);
    ASSERT_EQ(0, box.ext[0]);
    ASSERT_EQ(0, box.ext[1]);
    ASSERT_EQ(0, box.ext[2]);
    ASSERT_EQ(0, box.ext[3]);
    ASSERT_NE(nullptr, box.data);
    ASSERT_EQ(4*3*2, box.size);
    ASSERT_EQ(4*3*2*sizeof(ui32), box.total_data_byte);
    ASSERT_NE(nullptr, box.dims);
    ASSERT_EQ(3, box.rank);
    ASSERT_EQ(3*sizeof(ui32), box.total_dims_byte);

    BoxPacket packet;
    ASSERT_EQ(E_SUCCESS, packet.build(&box));
    auto * pointer = packet.point();
    ASSERT_NE(nullptr, pointer);
    auto size = packet.size();
    ASSERT_LT(1, size);

    box_data box2;
    box_clear(&box2);

    BoxPacket packet2;
    std::size_t computed_size = 0;
    ASSERT_EQ(E_SUCCESS, packet2.parse((char const *)pointer, size, &box2, &computed_size));
    ASSERT_LT(1, computed_size);
    ASSERT_EQ(computed_size, size);

    ASSERT_EQ(box.type, box2.type);
    ASSERT_EQ(box.device, box2.device);
    ASSERT_EQ(box.ext[0], box2.ext[0]);
    ASSERT_EQ(box.ext[1], box2.ext[1]);
    ASSERT_EQ(box.ext[2], box2.ext[2]);
    ASSERT_EQ(box.ext[3], box2.ext[3]);
    ASSERT_NE(box.data, box2.data);
    ASSERT_EQ(box.total_data_byte, box2.total_data_byte);
    ASSERT_EQ(box.size, box2.size);
    ASSERT_NE(box.dims, box2.dims);
    ASSERT_EQ(box.total_dims_byte, box2.total_dims_byte);
    ASSERT_EQ(box.rank, box2.rank);

    for (i = 0; i < 24; ++i) {
        si32 box_data;
        box_data_get(&box, &box_data, BT_INT32, BOX_DEVICE_CPU, nullptr, i);
        ASSERT_EQ(i, box_data);

        si32 box2_data;
        box_data_get(&box2, &box2_data, BT_INT32, BOX_DEVICE_CPU, nullptr, i);
        ASSERT_EQ(i, box2_data);
        ASSERT_EQ(i, ((si32*)box2.data)[i]);
    }

    box_free(&box);
    box_free(&box2);
}

