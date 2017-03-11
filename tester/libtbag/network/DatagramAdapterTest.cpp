/**
 * @file   DatagramAdapterTest.cpp
 * @brief  DatagramAdapter class tester.
 * @author zer0
 * @date   2016-01-18
 */

#include <gtest/gtest.h>
#include <libtbag/network/DatagramAdapter.hpp>

using namespace libtbag;
using namespace libtbag::network;

TEST(DatagramAdapterTest, DatagramInterface)
{
     int16_t const HOST_S16 = 10;
    uint16_t const HOST_U16 = 11;
     int32_t const HOST_S32 = 12;
    uint32_t const HOST_U32 = 13;
     int64_t const HOST_S64 = 14;
    uint64_t const HOST_U64 = 15;

    ASSERT_EQ(HOST_S16, DatagramInterface::toHost(DatagramInterface::toNetwork(HOST_S16)));
    ASSERT_EQ(HOST_U16, DatagramInterface::toHost(DatagramInterface::toNetwork(HOST_U16)));
    ASSERT_EQ(HOST_S32, DatagramInterface::toHost(DatagramInterface::toNetwork(HOST_S32)));
    ASSERT_EQ(HOST_U32, DatagramInterface::toHost(DatagramInterface::toNetwork(HOST_U32)));
    //ASSERT_EQ(HOST_S64, DatagramInterface::toHost(DatagramInterface::toNetwork(HOST_S64)));
    //ASSERT_EQ(HOST_U64, DatagramInterface::toHost(DatagramInterface::toNetwork(HOST_U64)));
}

TEST(DatagramAdapterTest, DatagramWrite_01)
{
    std::string const STEP_01 = "123456789123456789123456789123456789";
    std::string const STEP_02 = "asdf";
    std::string const STEP_03 = "asdf";

    DatagramWrite::Size const HEADER_SIZE = DatagramWrite::DATAGRAM_HEADER_SIZE;

    DatagramWrite datagram;
    std::string   temp;

    datagram.pushWriteBuffer(&STEP_01[0], STEP_01.size());
    datagram.popWriteBuffer([&](char const * buffer, std::size_t size){
        temp.clear();
        temp.assign(buffer + HEADER_SIZE, buffer + size);
        ASSERT_EQ(STEP_01.size(), DatagramWrite::parseBufferSize(buffer, size));
        ASSERT_EQ(STEP_01, temp);
    });

    datagram.pushWriteBuffer(&STEP_02[0], STEP_02.size());
    datagram.popWriteBuffer([&](char const * buffer, std::size_t size){
        temp.clear();
        temp.assign(buffer + HEADER_SIZE, buffer + size);
        ASSERT_EQ(STEP_02.size(), DatagramWrite::parseBufferSize(buffer, size));
        ASSERT_EQ(STEP_02, temp);
    });

    datagram.pushWriteBuffer(&STEP_03[0], STEP_03.size());
    datagram.popWriteBuffer([&](char const * buffer, std::size_t size){
        temp.clear();
        temp.assign(buffer + HEADER_SIZE, buffer + size);
        ASSERT_EQ(STEP_03.size(), DatagramWrite::parseBufferSize(buffer, size));
        ASSERT_EQ(STEP_03, temp);
    });
}

TEST(DatagramAdapterTest, DatagramAdapter)
{
    std::string const STEP_01 = "123456789123456789123456789123456789";
    std::string const STEP_02 = "asdf";
    std::string const STEP_03 = "qwer";

    DatagramAdapter datagram;
    std::string     temp;

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

    DatagramAdapter::binf info;

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

