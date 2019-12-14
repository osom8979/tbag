/**
 * @file   BoxPacketTest.cpp
 * @brief  BoxPacket class tester.
 * @author zer0
 * @date   2019-05-22
 */

#include <gtest/gtest.h>
#include <libtbag/dom/json/JsonUtils.hpp>
#include <libtbag/box/BoxPacket.hpp>
#include <iterator>

using namespace libtbag;
using namespace libtbag::box;
using namespace libtbag::box::details;

TEST(BoxPacketTest, Default)
{
    box_data box;

    ASSERT_EQ(E_SUCCESS, box_resize_args(&box, BT_INT32, BD_CPU, nullptr, 3, 4, 3, 2));
    ui32 i = 0;
    for (i = 0; i < 24; ++i) {
        box_data_set(&box, &i, BT_INT32, BD_CPU, nullptr, i);
        ASSERT_EQ(i, ((si32*)box.data)[i]);
    }

    ASSERT_EQ(BT_INT32, box.type);
    ASSERT_EQ(BD_CPU, box.device);
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
        box_data_get(&box, &box_data, BT_INT32, BD_CPU, nullptr, i);
        ASSERT_EQ(i, box_data);

        si32 box2_data;
        box_data_get(&box2, &box2_data, BT_INT32, BD_CPU, nullptr, i);
        ASSERT_EQ(i, box2_data);
        ASSERT_EQ(i, ((si32*)box2.data)[i]);
    }

    box_free(&box);
    box_free(&box2);
}

TEST(BoxPacketTest, ToJson)
{
    box_data box;

    ASSERT_EQ(E_SUCCESS, box_resize_args(&box, BT_INT32, BD_CPU, nullptr, 3, 4, 3, 2));
    ui32 i = 0;
    for (i = 0; i < 24; ++i) {
        box_data_set(&box, &i, BT_INT32, BD_CPU, nullptr, i);
        ASSERT_EQ(i, ((si32*)box.data)[i]);
    }

    BoxPacket packet(BoxPacket::Options{});
    ASSERT_EQ(E_SUCCESS, packet.build(&box));
    auto const text = packet.toJsonString();
    ASSERT_FALSE(text.empty());
    // std::cout << text << std::endl;

    Json::Value root;
    ASSERT_TRUE(libtbag::dom::json::parse(packet.toJsonString(), root));
    ASSERT_TRUE(root["dims"].isArray());
    ASSERT_EQ(3, root["dims"].size());
    ASSERT_EQ(4, root["dims"][0].asInt());
    ASSERT_EQ(3, root["dims"][1].asInt());
    ASSERT_EQ(2, root["dims"][2].asInt());

    box_data box2;
    BoxPacketParser parser;
    ASSERT_EQ(E_SUCCESS, parser.parseJson(text, &box2));

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
        ASSERT_EQ(((si32*)box.data)[i], ((si32*)box2.data)[i]);
    }

    box_free(&box);
    box_free(&box2);
}

