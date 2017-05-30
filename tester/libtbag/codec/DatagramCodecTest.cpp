/**
 * @file   DatagramCodecTest.cpp
 * @brief  DatagramCodec class tester.
 * @author zer0
 * @date   2016-01-18
 * @date   2017-05-13 (Rename: DatagramAdapterTest -> DatagramCodecTest)
 */

#include <gtest/gtest.h>
#include <libtbag/codec/DatagramCodec.hpp>

using namespace libtbag;
using namespace libtbag::codec;

TEST(DatagramCodecTest, DatagramEncoder)
{
    std::string const STEP_01 = "123456789123456789123456789123456789";
    std::string const STEP_02 = "asdf";
    std::string const STEP_03 = "asdf";

    DatagramEncoder::Size const HEADER_SIZE = DatagramEncoder::DATAGRAM_HEADER_SIZE;

    DatagramEncoder datagram;
    std::string   temp;

    datagram.pushWriteBuffer(&STEP_01[0], STEP_01.size());
    datagram.popWriteBuffer([&](char const * buffer, std::size_t size){
        temp.clear();
        temp.assign(buffer + HEADER_SIZE, buffer + size);
        ASSERT_EQ(STEP_01.size(), DatagramEncoder::parseBufferSize(buffer, size));
        ASSERT_EQ(STEP_01, temp);
    });

    datagram.pushWriteBuffer(&STEP_02[0], STEP_02.size());
    datagram.popWriteBuffer([&](char const * buffer, std::size_t size){
        temp.clear();
        temp.assign(buffer + HEADER_SIZE, buffer + size);
        ASSERT_EQ(STEP_02.size(), DatagramEncoder::parseBufferSize(buffer, size));
        ASSERT_EQ(STEP_02, temp);
    });

    datagram.pushWriteBuffer(&STEP_03[0], STEP_03.size());
    datagram.popWriteBuffer([&](char const * buffer, std::size_t size){
        temp.clear();
        temp.assign(buffer + HEADER_SIZE, buffer + size);
        ASSERT_EQ(STEP_03.size(), DatagramEncoder::parseBufferSize(buffer, size));
        ASSERT_EQ(STEP_03, temp);
    });
}

TEST(DatagramCodecTest, DatagramCodec)
{
    std::string const STEP_01 = "123456789123456789123456789123456789";
    std::string const STEP_02 = "asdf";
    std::string const STEP_03 = "qwer";

    DatagramCodec datagram;
    std::string   temp;

    datagram.pushWriteBuffer(&STEP_01[0], STEP_01.size());
    datagram.pushWriteBuffer(&STEP_02[0], STEP_02.size());
    datagram.pushWriteBuffer(&STEP_03[0], STEP_03.size());

    datagram.popWriteBuffer([&](char const * buffer, std::size_t size){
        datagram.push(buffer, size);
    });
    datagram.popWriteBuffer([&](char const * buffer, std::size_t size){
        datagram.push(buffer, size);
    });
    datagram.popWriteBuffer([&](char const * buffer, std::size_t size){
        datagram.push(buffer, size);
    });

    DatagramCodec::binf info;

    ASSERT_TRUE(datagram.next(&info));
    temp.clear();
    temp.assign(info.buffer, info.buffer + info.size);
    ASSERT_EQ(STEP_01, temp);

    ASSERT_TRUE(datagram.next(&info));
    temp.clear();
    temp.assign(info.buffer, info.buffer + info.size);
    ASSERT_EQ(STEP_02, temp);

    ASSERT_TRUE(datagram.next(&info));
    temp.clear();
    temp.assign(info.buffer, info.buffer + info.size);
    ASSERT_EQ(STEP_03, temp);
}

