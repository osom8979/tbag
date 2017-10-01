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

