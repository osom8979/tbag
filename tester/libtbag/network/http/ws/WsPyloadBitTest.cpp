/**
 * @file   WsPyloadBitTest.cpp
 * @brief  WsPyloadBit class tester.
 * @author zer0
 * @date   2017-10-01
 */

#include <gtest/gtest.h>
#include <libtbag/network/http/ws/WsPyloadBit.hpp>

using namespace libtbag;
using namespace libtbag::network;
using namespace libtbag::network::http;
using namespace libtbag::network::http::ws;

TEST(WsPyloadBitTest, Default)
{
    for (uint8_t i = 0; i <= 125; ++i) {
        ASSERT_TRUE(WsPayloadBit::WSPL_BIT_7 == getWsPayloadBit(i));
        ASSERT_TRUE(WsPayloadBit::WSPL_BIT_7 == getWsPayloadBitWithPayloadLength(i));
    }

    ASSERT_TRUE(WsPayloadBit::WSPL_BIT_16 == getWsPayloadBit(126));
    ASSERT_TRUE(WsPayloadBit::WSPL_BIT_16 == getWsPayloadBitWithPayloadLength(126));
    ASSERT_TRUE(WsPayloadBit::WSPL_BIT_16 == getWsPayloadBitWithPayloadLength(65535));

    ASSERT_TRUE(WsPayloadBit::WSPL_BIT_64 == getWsPayloadBit(127));
    ASSERT_TRUE(WsPayloadBit::WSPL_BIT_64 == getWsPayloadBitWithPayloadLength(65536));
    ASSERT_TRUE(WsPayloadBit::WSPL_BIT_64 == getWsPayloadBitWithPayloadLength(100000));
}

TEST(WsPyloadBitTest, GetMaskData)
{
    uint8_t const MASKING_KEY[] = {0xb0, 0xad, 0xf0, 0xa7};
    uint8_t const ORIGINAL_DATA[] = {
            0xe2, 0xc2, 0x93, 0xcc, 0x90, 0xc4, 0x84, 0x87, 0xc7, 0xc4, 0x84, 0xcf, 0x90, 0xe5, 0xa4, 0xea,
            0xfc, 0x98, 0xd0, 0xf0, 0xd5, 0xcf, 0xa3, 0xc8, 0xd3, 0xc6, 0x95, 0xd3};
    uint8_t const RESULT_DATA[] = {
            0x52, 0x6f, 0x63, 0x6b, 0x20, 0x69, 0x74, 0x20, 0x77, 0x69, 0x74, 0x68, 0x20, 0x48, 0x54, 0x4d,
            0x4c, 0x35, 0x20, 0x57, 0x65, 0x62, 0x53, 0x6f, 0x63, 0x6b, 0x65, 0x74};
    std::string const RESULT_STRING = "Rock it with HTML5 WebSocket";

    std::string const INPUT(ORIGINAL_DATA, ORIGINAL_DATA + sizeof(ORIGINAL_DATA));
    std::string const OUTPUT(RESULT_DATA, RESULT_DATA + sizeof(RESULT_DATA));

    ASSERT_EQ(RESULT_STRING, OUTPUT);
    ASSERT_EQ(OUTPUT, getPayloadData(copyMaskingKeyFromBuffer((char const *)MASKING_KEY), INPUT));
    ASSERT_EQ( INPUT, getPayloadData(copyMaskingKeyFromBuffer((char const *)MASKING_KEY), OUTPUT));
}

