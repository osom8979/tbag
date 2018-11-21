/**
 * @file   BoxPacketTest.cpp
 * @brief  BoxPacket class tester.
 * @author zer0
 * @date   2018-10-24
 * @date   2018-11-07 (Rename: TbagPacketTest -> BoxPacketTest)
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/proto/BoxPacket.hpp>

using namespace libtbag;
using namespace libtbag::proto;

TEST(BoxPacketTest, UpdateSelf)
{
    uint64_t const TEST_ID   = 10;
    int32_t  const TEST_TYPE = 20;
    int32_t  const TEST_CODE = 30;

    BoxPacket packet;
    ASSERT_EQ(Err::E_SUCCESS, packet.build(TEST_ID, TEST_TYPE, TEST_CODE));
    auto const BUILD_BUFFER = packet.toBuffer();
    ASSERT_FALSE(BUILD_BUFFER.empty());

    ASSERT_EQ(Err::E_SUCCESS, packet.parse(BUILD_BUFFER));
    ASSERT_EQ(TEST_ID, packet.id());
    ASSERT_EQ(TEST_TYPE, packet.type());
    ASSERT_EQ(TEST_CODE, packet.code());
    ASSERT_TRUE(packet.boxes().empty());
}

TEST(BoxPacketTest, UpdateSelf_DataSlice)
{
    uint64_t const TEST_ID   = 10;
    int32_t  const TEST_TYPE = 20;
    int32_t  const TEST_CODE = 30;

    BoxPacket packet;
    ASSERT_EQ(Err::E_SUCCESS, packet.build(TEST_ID, TEST_TYPE, TEST_CODE));
    auto const BUILD_BUFFER = packet.toBuffer();
    ASSERT_FALSE(BUILD_BUFFER.empty());

    auto const BUFFER_SIZE = BUILD_BUFFER.size();
    BoxPacket::Buffer extend_buffer(BUFFER_SIZE + 1);
    std::copy(BUILD_BUFFER.begin(), BUILD_BUFFER.end(), extend_buffer.begin());

    std::size_t computed_size = 0;
    ASSERT_EQ(Err::E_VERIFIER, packet.parse(extend_buffer.data(), BUFFER_SIZE - 1, &computed_size));
    ASSERT_EQ(Err::E_SUCCESS, packet.parse(extend_buffer.data(), BUFFER_SIZE + 0, &computed_size));
    ASSERT_EQ(BUFFER_SIZE, computed_size);

    ASSERT_EQ(Err::E_SUCCESS, packet.parse(extend_buffer.data(), BUFFER_SIZE + 1, &computed_size));
    ASSERT_EQ(BUFFER_SIZE, computed_size);

    ASSERT_EQ(TEST_ID, packet.id());
    ASSERT_EQ(TEST_TYPE, packet.type());
    ASSERT_EQ(TEST_CODE, packet.code());
    ASSERT_TRUE(packet.boxes().empty());
}

TEST(BoxPacketTest, UpdateSelf_BagEx)
{
    using namespace libtbag::container;
    std::string const BAG_KEY = "bag";
    Box bag;
    bag.resize<int32_t>(2, 3);
    ASSERT_EQ(BoxTypeTable::BTT_INT32, bag.getType());
    ASSERT_EQ(2*3, bag.size());
    ASSERT_EQ(2, bag.size(0));
    ASSERT_EQ(3, bag.size(1));
    for (int i = 0; i < bag.size(); ++i) {
        *(bag.cast<int32_t>() + i) = i;
    }

    BoxPacket::BoxMap map;
    map.insert(std::make_pair(BAG_KEY, bag));
    ASSERT_EQ(1, map.size());

    BoxPacket packet1;
    ASSERT_EQ(Err::E_SUCCESS, packet1.build(map));
    auto const BUILD_BUFFER = packet1.toBuffer();
    ASSERT_FALSE(BUILD_BUFFER.empty());

    BoxPacket packet2;
    ASSERT_EQ(Err::E_SUCCESS, packet2.parse(BUILD_BUFFER));
    ASSERT_FALSE(packet2.boxes().empty());
    ASSERT_EQ(1, packet2.boxes().size());

    auto bag_result = packet2.boxes()[BAG_KEY];
    ASSERT_EQ(BoxTypeTable::BTT_INT32, bag_result.getType());
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

TEST(BoxPacketTest, UpdateSelf_String)
{
    std::string const TEST_TEXT = "BoxPacketTest.UpdateSelf_BagEx";
    uint64_t const TEST_ID   = 10;
    int32_t  const TEST_TYPE = 20;
    int32_t  const TEST_CODE = 30;

    BoxPacket packet;
    ASSERT_EQ(Err::E_SUCCESS, packet.build(TEST_TEXT, TEST_ID, TEST_TYPE, TEST_CODE));
    auto const BUILD_BUFFER = packet.toBuffer();
    ASSERT_FALSE(BUILD_BUFFER.empty());

    ASSERT_EQ(Err::E_SUCCESS, packet.parse(BUILD_BUFFER));
    ASSERT_EQ(TEST_ID, packet.id());
    ASSERT_EQ(TEST_TYPE, packet.type());
    ASSERT_EQ(TEST_CODE, packet.code());
    ASSERT_FALSE(packet.boxes().empty());
    ASSERT_EQ(1, packet.boxes().size());
    ASSERT_EQ(TEST_TEXT, packet.boxes().begin()->first);
    ASSERT_FALSE(packet.boxes().begin()->second.exists());
}

TEST(BoxPacketTest, UpdateSelf_KeyValue)
{
    std::string const TEST_KEY = "BoxPacketTest";
    std::string const TEST_VAL = "UpdateSelf_BagEx";
    uint64_t const TEST_ID   = 10;
    int32_t  const TEST_TYPE = 20;
    int32_t  const TEST_CODE = 30;

    BoxPacket packet;
    ASSERT_EQ(Err::E_SUCCESS, packet.build(TEST_KEY, TEST_VAL, TEST_ID, TEST_TYPE, TEST_CODE));
    auto const BUILD_BUFFER = packet.toBuffer();
    ASSERT_FALSE(BUILD_BUFFER.empty());

    ASSERT_EQ(Err::E_SUCCESS, packet.parse(BUILD_BUFFER));
    ASSERT_EQ(TEST_ID, packet.id());
    ASSERT_EQ(TEST_TYPE, packet.type());
    ASSERT_EQ(TEST_CODE, packet.code());
    ASSERT_FALSE(packet.boxes().empty());
    ASSERT_EQ(1, packet.boxes().size());
    ASSERT_EQ(TEST_KEY, packet.boxes().begin()->first);
    ASSERT_TRUE(packet.boxes().begin()->second.exists());
    ASSERT_EQ(TEST_VAL, packet.boxes().begin()->second.toString());
}

TEST(BoxPacketTest, FindKey)
{
    std::string const TEST_KEY = "BoxPacketTest";
    std::string const TEST_VAL = "FindKey";

    BoxPacket packet;
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

TEST(BoxPacketTest, Assign)
{
    uint64_t const TEST_ID   = 1;
    int32_t  const TEST_TYPE = 2;
    int32_t  const TEST_CODE = 3;

    BoxPacket packet;
    ASSERT_EQ(Err::E_SUCCESS, packet.build(TEST_ID, TEST_TYPE, TEST_CODE));
    auto const BUILD_BUFFER1 = packet.toBuffer();
    ASSERT_FALSE(BUILD_BUFFER1.empty());

    ASSERT_EQ(Err::E_SUCCESS, packet.assign(BUILD_BUFFER1));
    auto const BUILD_BUFFER2 = packet.toBuffer();
    ASSERT_FALSE(BUILD_BUFFER2.empty());
    ASSERT_TRUE(std::equal(BUILD_BUFFER1.begin(), BUILD_BUFFER1.end(), BUILD_BUFFER2.begin(), BUILD_BUFFER2.end()));
}

TEST(BoxPacketTest, FileSaveLoad)
{
    tttDir(true, true);
    auto const FILE_PATH = tttDir_Get() / "mats";

    using namespace libtbag::container;
    std::string const BAG1_KEY = "bag1";
    std::string const BAG1_VAL = "Test";
    std::string const BAG2_KEY = "bag2";
    std::vector<int32_t> const BAG2_VAL = {1, 2, 3, 4};

    Box bag1 = BAG1_VAL;
    Box bag2 = BAG2_VAL;
    ASSERT_EQ(BoxTypeTable::BTT_INT8, bag1.getType());
    ASSERT_EQ(BoxTypeTable::BTT_INT32, bag2.getType());

    uint64_t const TEST_ID   = 1;
    int32_t  const TEST_TYPE = 2;
    int32_t  const TEST_CODE = 3;

    BoxPacket::BoxMap map;
    map.insert(std::make_pair(BAG1_KEY, bag1));
    map.insert(std::make_pair(BAG2_KEY, bag2));
    ASSERT_EQ(2, map.size());

    BoxPacket packet1;
    ASSERT_EQ(Err::E_SUCCESS, packet1.build(map, TEST_ID, TEST_TYPE, TEST_CODE));
    ASSERT_FALSE(FILE_PATH.exists());
    ASSERT_EQ(Err::E_SUCCESS, packet1.saveFile(FILE_PATH));
    ASSERT_TRUE(FILE_PATH.exists());

    BoxPacket packet2;
    ASSERT_EQ(Err::E_SUCCESS, packet2.loadFile(FILE_PATH));

    ASSERT_EQ(TEST_ID, packet2.id());
    ASSERT_EQ(TEST_TYPE, packet2.type());
    ASSERT_EQ(TEST_CODE, packet2.code());
    ASSERT_FALSE(packet2.boxes().empty());
    ASSERT_EQ(2, packet2.boxes().size());

    auto bag1_result = packet2.boxes()[BAG1_KEY];
    auto bag2_result = packet2.boxes()[BAG2_KEY];

    ASSERT_EQ(BoxTypeTable::BTT_INT8, bag1_result.getType());
    ASSERT_EQ(BAG1_VAL.size(), bag1_result.size());
    ASSERT_EQ(1, bag1_result.dims());
    ASSERT_EQ(BAG1_VAL, bag1_result.toString());

    ASSERT_EQ(BoxTypeTable::BTT_INT32, bag2_result.getType());
    ASSERT_EQ(BAG2_VAL.size(), bag2_result.size());
    ASSERT_EQ(1, bag2_result.dims());
    auto * bag2_begin = bag2_result.cast<int32_t>();
    auto * bag2_end = bag2_result.cast<int32_t>() + bag2_result.size();
    ASSERT_TRUE(std::equal(BAG2_VAL.begin(), BAG2_VAL.end(), bag2_begin, bag2_end));
}

