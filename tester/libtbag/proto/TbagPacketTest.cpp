/**
 * @file   TbagPacketTest.cpp
 * @brief  TbagPacket class tester.
 * @author zer0
 * @date   2018-10-24
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/proto/TbagPacket.hpp>

using namespace libtbag;
using namespace libtbag::proto;

TEST(TbagPacketTest, UpdateSelf)
{
    uint64_t const TEST_ID   = 10;
    int32_t  const TEST_TYPE = 20;
    int32_t  const TEST_CODE = 30;

    TbagPacket packet;
    ASSERT_EQ(Err::E_SUCCESS, packet.build(TEST_ID, TEST_TYPE, TEST_CODE));
    auto const BUILD_BUFFER = packet.toBuffer();
    ASSERT_FALSE(BUILD_BUFFER.empty());

    ASSERT_EQ(Err::E_SUCCESS, packet.update(BUILD_BUFFER));
    ASSERT_EQ(TEST_ID, packet.id());
    ASSERT_EQ(TEST_TYPE, packet.type());
    ASSERT_EQ(TEST_CODE, packet.code());
    ASSERT_TRUE(packet.bags().empty());
}

TEST(TbagPacketTest, UpdateSelf_BagEx)
{
    using namespace libtbag::container;
    std::string const BAG_KEY = "bag";
    Bex bag;
    bag.resize<int32_t>(2, 3);
    ASSERT_EQ(BexType::BT_INT32, bag.getType());
    ASSERT_EQ(2*3, bag.size());
    ASSERT_EQ(2, bag.size(0));
    ASSERT_EQ(3, bag.size(1));
    for (int i = 0; i < bag.size(); ++i) {
        *(bag.cast<int32_t>() + i) = i;
    }

    TbagPacket::BagExMap map;
    map.insert(std::make_pair(BAG_KEY, bag));
    ASSERT_EQ(1, map.size());

    TbagPacket packet1;
    ASSERT_EQ(Err::E_SUCCESS, packet1.build(map));
    auto const BUILD_BUFFER = packet1.toBuffer();
    ASSERT_FALSE(BUILD_BUFFER.empty());

    TbagPacket packet2;
    ASSERT_EQ(Err::E_SUCCESS, packet2.update(BUILD_BUFFER));
    ASSERT_FALSE(packet2.bags().empty());
    ASSERT_EQ(1, packet2.bags().size());

    auto bag_result = packet2.bags()[BAG_KEY];
    ASSERT_EQ(BexType::BT_INT32, bag_result.getType());
    ASSERT_EQ(2*3, bag_result.size());
    ASSERT_EQ(2, bag_result.size(0));
    ASSERT_EQ(3, bag_result.size(1));
    ASSERT_EQ(0, bag_result.at<int32_t>(0, 0));
    ASSERT_EQ(1, bag_result.at<int32_t>(0, 1));
    ASSERT_EQ(2, bag_result.at<int32_t>(0, 2));
    ASSERT_EQ(3, bag_result.at<int32_t>(1, 0));
    ASSERT_EQ(4, bag_result.at<int32_t>(1, 1));
    ASSERT_EQ(5, bag_result.at<int32_t>(1, 2));
}

TEST(TbagPacketTest, UpdateSelf_String)
{
    std::string const TEST_TEXT = "TbagPacketTest.UpdateSelf_BagEx";
    uint64_t const TEST_ID   = 10;
    int32_t  const TEST_TYPE = 20;
    int32_t  const TEST_CODE = 30;

    TbagPacket packet;
    ASSERT_EQ(Err::E_SUCCESS, packet.build(TEST_TEXT, TEST_ID, TEST_TYPE, TEST_CODE));
    auto const BUILD_BUFFER = packet.toBuffer();
    ASSERT_FALSE(BUILD_BUFFER.empty());

    ASSERT_EQ(Err::E_SUCCESS, packet.update(BUILD_BUFFER));
    ASSERT_EQ(TEST_ID, packet.id());
    ASSERT_EQ(TEST_TYPE, packet.type());
    ASSERT_EQ(TEST_CODE, packet.code());
    ASSERT_FALSE(packet.bags().empty());
    ASSERT_EQ(1, packet.bags().size());
    ASSERT_EQ(TEST_TEXT, packet.bags().begin()->first);
    ASSERT_FALSE(packet.bags().begin()->second.exists());
}

TEST(TbagPacketTest, UpdateSelf_KeyValue)
{
    std::string const TEST_KEY = "TbagPacketTest";
    std::string const TEST_VAL = "UpdateSelf_BagEx";
    uint64_t const TEST_ID   = 10;
    int32_t  const TEST_TYPE = 20;
    int32_t  const TEST_CODE = 30;

    TbagPacket packet;
    ASSERT_EQ(Err::E_SUCCESS, packet.build(TEST_KEY, TEST_VAL, TEST_ID, TEST_TYPE, TEST_CODE));
    auto const BUILD_BUFFER = packet.toBuffer();
    ASSERT_FALSE(BUILD_BUFFER.empty());

    ASSERT_EQ(Err::E_SUCCESS, packet.update(BUILD_BUFFER));
    ASSERT_EQ(TEST_ID, packet.id());
    ASSERT_EQ(TEST_TYPE, packet.type());
    ASSERT_EQ(TEST_CODE, packet.code());
    ASSERT_FALSE(packet.bags().empty());
    ASSERT_EQ(1, packet.bags().size());
    ASSERT_EQ(TEST_KEY, packet.bags().begin()->first);
    ASSERT_TRUE(packet.bags().begin()->second.exists());
    ASSERT_EQ(TEST_VAL, packet.bags().begin()->second.toString());
}

TEST(TbagPacketTest, FindKey)
{
    std::string const TEST_KEY = "TbagPacketTest";
    std::string const TEST_VAL = "FindKey";

    TbagPacket packet;
    ASSERT_EQ(Err::E_SUCCESS, packet.build(TEST_KEY, TEST_VAL));
    auto const BUILD_BUFFER = packet.toBuffer();
    ASSERT_FALSE(BUILD_BUFFER.empty());

    Err code = Err::E_UNKNOWN;
    auto bag1 = packet.findKey(BUILD_BUFFER, TEST_KEY, &code);
    ASSERT_EQ(Err::E_SUCCESS, code);
    ASSERT_TRUE(bag1.exists());
    ASSERT_EQ(TEST_VAL, bag1.toString());

    code = Err::E_UNKNOWN;
    auto bag2 = packet.findKey(BUILD_BUFFER, "NotFoundKey", &code);
    ASSERT_EQ(Err::E_ENFOUND, code);
    ASSERT_FALSE(bag2.exists());
}

TEST(TbagPacketTest, Assign)
{
    uint64_t const TEST_ID   = 1;
    int32_t  const TEST_TYPE = 2;
    int32_t  const TEST_CODE = 3;

    TbagPacket packet;
    ASSERT_EQ(Err::E_SUCCESS, packet.build(TEST_ID, TEST_TYPE, TEST_CODE));
    auto const BUILD_BUFFER1 = packet.toBuffer();
    ASSERT_FALSE(BUILD_BUFFER1.empty());

    ASSERT_EQ(Err::E_SUCCESS, packet.assign(BUILD_BUFFER1));
    auto const BUILD_BUFFER2 = packet.toBuffer();
    ASSERT_FALSE(BUILD_BUFFER2.empty());
    ASSERT_TRUE(std::equal(BUILD_BUFFER1.begin(), BUILD_BUFFER1.end(), BUILD_BUFFER2.begin(), BUILD_BUFFER2.end()));
}

TEST(TbagPacketTest, FileSaveLoad)
{
    tttDir(true, true);
    auto const FILE_PATH = tttDir_Get() / "mats";

    using namespace libtbag::container;
    std::string const BAG1_KEY = "bag1";
    std::string const BAG1_VAL = "Test";
    std::string const BAG2_KEY = "bag2";
    std::vector<int32_t> const BAG2_VAL = {1, 2, 3, 4};

    Bex bag1 = BAG1_VAL;
    Bex bag2 = BAG2_VAL;
    ASSERT_EQ(BexType::BT_INT8, bag1.getType());
    ASSERT_EQ(BexType::BT_INT32, bag2.getType());

    uint64_t const TEST_ID   = 1;
    int32_t  const TEST_TYPE = 2;
    int32_t  const TEST_CODE = 3;

    TbagPacket::BagExMap map;
    map.insert(std::make_pair(BAG1_KEY, bag1));
    map.insert(std::make_pair(BAG2_KEY, bag2));
    ASSERT_EQ(2, map.size());

    TbagPacket packet1;
    ASSERT_EQ(Err::E_SUCCESS, packet1.build(map, TEST_ID, TEST_TYPE, TEST_CODE));
    ASSERT_FALSE(FILE_PATH.exists());
    ASSERT_EQ(Err::E_SUCCESS, packet1.saveFile(FILE_PATH));
    ASSERT_TRUE(FILE_PATH.exists());

    TbagPacket packet2;
    ASSERT_EQ(Err::E_SUCCESS, packet2.loadFile(FILE_PATH));

    ASSERT_EQ(TEST_ID, packet2.id());
    ASSERT_EQ(TEST_TYPE, packet2.type());
    ASSERT_EQ(TEST_CODE, packet2.code());
    ASSERT_FALSE(packet2.bags().empty());
    ASSERT_EQ(2, packet2.bags().size());

    auto bag1_result = packet2.bags()[BAG1_KEY];
    auto bag2_result = packet2.bags()[BAG2_KEY];

    ASSERT_EQ(BexType::BT_INT8, bag1_result.getType());
    ASSERT_EQ(BAG1_VAL.size(), bag1_result.size());
    ASSERT_EQ(1, bag1_result.dims());
    ASSERT_EQ(BAG1_VAL, bag1_result.toString());

    ASSERT_EQ(BexType::BT_INT32, bag2_result.getType());
    ASSERT_EQ(BAG2_VAL.size(), bag2_result.size());
    ASSERT_EQ(1, bag2_result.dims());
    auto * bag2_begin = bag2_result.cast<int32_t>();
    auto * bag2_end = bag2_result.cast<int32_t>() + bag2_result.size();
    ASSERT_TRUE(std::equal(BAG2_VAL.begin(), BAG2_VAL.end(), bag2_begin, bag2_end));
}

