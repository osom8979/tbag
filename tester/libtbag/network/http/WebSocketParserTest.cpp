/**
 * @file   WebSocketParserTest.cpp
 * @brief  WebSocketParser class tester.
 * @author zer0
 * @date   2017-06-11
 */

#include <gtest/gtest.h>
#include <libtbag/network/http/WebSocketParser.hpp>

using namespace libtbag;
using namespace libtbag::network;
using namespace libtbag::network::http;

TEST(WebSocketParserTest, GetResponseWebSocket)
{
    std::string const TEST_REQUEST = "GET /chat HTTP/1.1\r\n"
            "Host: server.example.com\r\n"
            "Upgrade: websocket\r\n"
            "Connection: Upgrade\r\n"
            "Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==\r\n"
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
    ASSERT_EQ(Err::E_SUCCESS, getResponseWebSocket(request, response));

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

TEST(WebSocketParserTest, UpgradeWebsocketKey)
{
    std::string const TEST_ORIGINAL = "dGhlIHNhbXBsZSBub25jZQ==";
    std::string const TEST_RESULT   = "s3pPLMBiTxaQ9kYGzzhZRbK+xOo=";
    ASSERT_EQ(TEST_RESULT, upgradeWebSocketKey(TEST_ORIGINAL));
}

