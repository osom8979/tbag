/**
 * @file   HttpCommonTest.cpp
 * @brief  HttpCommon class tester.
 * @author zer0
 * @date   2018-12-25
 */

#include <gtest/gtest.h>
#include <libtbag/http/HttpCommon.hpp>
#include <iterator>

using namespace libtbag;
using namespace libtbag::http;

TEST(HttpCommonTest, DefaultHeader)
{
    HttpHeaders header;
    ASSERT_EQ(1, insert(header, "Host", "localhost"));
    ASSERT_EQ(1, insert(header, "Sec-WebSocket-Version", "7, 9, 13"));
    ASSERT_EQ(1, insertIfNotExists(header, "Upgrade", "WebSocket"));
    ASSERT_EQ(0, insertIfNotExists(header, "Upgrade", "TEMP"));

    ASSERT_STREQ("localhost", getHeaderValue(header, "Host").c_str());
    ASSERT_STREQ("localhost", getIgnoreCase(header, "host").c_str());
    ASSERT_TRUE(getHeaderValue(header, "host").empty());

    ASSERT_TRUE(exists(header, "Host"));
    ASSERT_FALSE(exists(header, "host"));

    ASSERT_TRUE(existsInSplitValues(header, "Sec-WebSocket-Version", "7"));
    ASSERT_TRUE(existsInSplitValues(header, "Sec-WebSocket-Version", "9"));
    ASSERT_TRUE(existsInSplitValues(header, "Sec-WebSocket-Version", "13"));

    ASSERT_FALSE(existsInSplitValues(header, "Sec-WebSocket-Version", "20"));
    ASSERT_FALSE(existsInSplitValues(header, "sec-websocket-version", "13")); // lower-case

    std::cout << "Header Information:\n" << toMessageHeader(header);

    header = getRegularization(header);
    ASSERT_STREQ("localhost", getHeaderValue(header, "host").c_str());
    ASSERT_TRUE(existsInSplitValues(header, "sec-websocket-version", "13")); // lower-case
    ASSERT_FALSE(existsInSplitValues(header, "Sec-WebSocket-Version", "13"));
}

TEST(HttpCommonTest, MultiHeader)
{
    HttpHeaders header;
    ASSERT_EQ(1, insert(header, "Host", "localhost"));
    ASSERT_EQ(1, insert(header, "Sec-WebSocket-Version", "7, 9, 13"));
    ASSERT_EQ(1, insert(header, "Upgrade", "WebSocket"));
    ASSERT_EQ(1, insert(header, "Upgrade", "Temp"));
    ASSERT_EQ(1, insert(header, "Upgrade", "AAAA"));
    ASSERT_EQ(1, insert(header, "Upgrade", "Temp"));
    ASSERT_EQ(6, header.size());

    ASSERT_TRUE(exists(header, "Upgrade", "Temp"));

    auto pair = header.equal_range("Upgrade");
    ASSERT_EQ(4, std::distance(pair.first, pair.second));
}

TEST(HttpCommonTest, HttpDefaultFilter)
{
    HttpRequest request_post_aaa;
    HttpRequest request_post_bbb;
    HttpRequest request_get_aaa;
    HttpRequest request_get_bbb;

    request_post_aaa.method = "POST";
    request_post_bbb.method = "POST";
    request_get_aaa .method = "GET";
    request_get_bbb .method = "GET";

    request_post_aaa.path = "/aaa";
    request_post_bbb.path = "/bbb";
    request_get_aaa .path = "/aaa";
    request_get_bbb .path = "/bbb";

    HttpBaseFilter f1;
    HttpBaseFilter f2("GET", "/aaa.*");
    HttpBaseFilter f3(std::regex("/bbb.*"));
    HttpBaseFilter f4 = f3;

    ASSERT_FALSE(f1.filter(request_post_aaa));
    ASSERT_FALSE(f1.filter(request_post_bbb));
    ASSERT_FALSE(f1.filter( request_get_aaa));
    ASSERT_FALSE(f1.filter( request_get_bbb));

    ASSERT_FALSE(f2.filter(request_post_aaa));
    ASSERT_FALSE(f2.filter(request_post_bbb));
    ASSERT_TRUE (f2.filter( request_get_aaa));
    ASSERT_FALSE(f2.filter( request_get_bbb));

    ASSERT_FALSE(f3.filter(request_post_aaa));
    ASSERT_TRUE (f3.filter(request_post_bbb));
    ASSERT_FALSE(f3.filter( request_get_aaa));
    ASSERT_TRUE (f3.filter( request_get_bbb));

    ASSERT_FALSE(f4.filter(request_post_aaa));
    ASSERT_TRUE (f4.filter(request_post_bbb));
    ASSERT_FALSE(f4.filter( request_get_aaa));
    ASSERT_TRUE (f4.filter( request_get_bbb));
}

TEST(HttpCommonTest, WsPyloadBit)
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

TEST(HttpCommonTest, GetMaskData)
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

TEST(HttpCommonTest, UpgradeWebsocketKey)
{
    std::string const TEST_ORIGINAL = "dGhlIHNhbXBsZSBub25jZQ==";
    std::string const TEST_RESULT   = "s3pPLMBiTxaQ9kYGzzhZRbK+xOo=";
    ASSERT_EQ(TEST_RESULT, getUpgradeWebSocketKey(TEST_ORIGINAL));
}

