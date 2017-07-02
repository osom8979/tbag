/**
 * @file   WebSocketFrameTest.cpp
 * @brief  WebSocketFrame class tester.
 * @author zer0
 * @date   2017-06-11
 */

#include <gtest/gtest.h>
#include <libtbag/network/http/WebSocketFrame.hpp>
#include <libtbag/bitwise/Endian.hpp>

using namespace libtbag;
using namespace libtbag::network;
using namespace libtbag::network::http;

TEST(WebSocketFrameTest, GetMaskData)
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
    ASSERT_EQ(OUTPUT, WebSocketFrame::getPayloadData(WebSocketFrame::getMaskingKey(MASKING_KEY), INPUT));
    ASSERT_EQ(INPUT, WebSocketFrame::getPayloadData(WebSocketFrame::getMaskingKey(MASKING_KEY), OUTPUT));
}

TEST(WebSocketFrameTest, RequestFrame)
{
    uint8_t const REQUEST_FRAME[] = {0x81, 0x9c, 0x6c, 0x11, 0xe8, 0xe3, 0x3e, 0x7e,
                                     0x8b, 0x88, 0x4c, 0x78, 0x9c, 0xc3, 0x1b, 0x78,
                                     0x9c, 0x8b, 0x4c, 0x59, 0xbc, 0xae, 0x20, 0x24,
                                     0xc8, 0xb4, 0x09, 0x73, 0xbb, 0x8c, 0x0f, 0x7a,
                                     0x8d, 0x97};
    std::string const RESULT_STRING = "Rock it with HTML5 WebSocket";

    // Request buffer to frame.
    WebSocketFrame frame;
    ASSERT_EQ(Err::E_SUCCESS, frame.execute(REQUEST_FRAME, sizeof(REQUEST_FRAME)));
    ASSERT_TRUE(frame.fin);
    ASSERT_FALSE(frame.rsv1);
    ASSERT_FALSE(frame.rsv2);
    ASSERT_FALSE(frame.rsv3);
    ASSERT_TRUE(frame.mask);
    ASSERT_EQ(bitwise::toNetwork(0x6c11e8e3), frame.masking_key);

    char const * data = (char const *)frame.getPayloadData();
    std::size_t size = frame.getPayloadSize();
    std::string payload(data, data + size);
    ASSERT_EQ(size, frame.getPayloadSize());
    ASSERT_EQ(RESULT_STRING, payload);

    // Frame to request buffer.
    std::vector<uint8_t> buffer;
    buffer.resize(sizeof(REQUEST_FRAME));
    ASSERT_EQ(sizeof(REQUEST_FRAME), frame.copyTo(buffer));

    std::string const lh(REQUEST_FRAME, REQUEST_FRAME + sizeof(REQUEST_FRAME));
    std::string const rh(buffer.begin(), buffer.end());
    ASSERT_EQ(lh, rh);
}

TEST(WebSocketFrameTest, GetResponseWebSocket)
{
    std::string const TEST_REQUEST = "GET /chat HTTP/1.1\r\n"
            "Host: server.example.com\r\n"
            "Upgrade: websocket\r\n"
            "Connection: Upgrade\r\n"
            "Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==\r\n" // Base64 original key: "the sample nonce"
            "Origin: http://example.com\r\n"
            "Sec-WebSocket-Protocol: chat, superchat\r\n"
            "Sec-WebSocket-Version: 13\r\n\r\n";

    std::string const TEST_RESPONSE = "HTTP/1.1 101 Switching Protocols\r\n"
            "Upgrade: websocket\r\n"
            "Connection: Upgrade\r\n"
            "Sec-WebSocket-Accept: s3pPLMBiTxaQ9kYGzzhZRbK+xOo=\r\n\r\n";

    HttpParser request;
    request.execute(TEST_REQUEST.c_str(), TEST_REQUEST.size());
    ASSERT_TRUE(request.isComplete());

    HttpBuilder response;
    ASSERT_EQ(Err::E_SUCCESS, updateResponseWebSocket(request, response));

    HttpParser test_response;
    test_response.execute(TEST_RESPONSE.c_str(), TEST_RESPONSE.size());
    ASSERT_TRUE(test_response.isComplete());

    ASSERT_EQ(test_response.getHttpMajor(), response.getMajor());
    ASSERT_EQ(test_response.getHttpMinor(), response.getMinor());
    ASSERT_EQ(test_response.getStatusCode(), response.getStatus());
    ASSERT_EQ(test_response.getHeader(HEADER_UPGRADE), response.getHeader(HEADER_UPGRADE));
    ASSERT_EQ(test_response.getHeader(HEADER_CONNECTION), response.getHeader(HEADER_CONNECTION));
    ASSERT_EQ(test_response.getHeader(HEADER_SEC_WEBSOCKET_ACCEPT), response.getHeader(HEADER_SEC_WEBSOCKET_ACCEPT));
}

TEST(WebSocketFrameTest, UpgradeWebsocketKey)
{
    std::string const TEST_ORIGINAL = "dGhlIHNhbXBsZSBub25jZQ==";
    std::string const TEST_RESULT   = "s3pPLMBiTxaQ9kYGzzhZRbK+xOo=";
    ASSERT_EQ(TEST_RESULT, getUpgradeWebSocketKey(TEST_ORIGINAL));
}

