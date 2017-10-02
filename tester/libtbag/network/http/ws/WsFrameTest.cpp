/**
 * @file   WsFrameTest.cpp
 * @brief  WsFrame class tester.
 * @author zer0
 * @date   2017-10-01
 */

#include <gtest/gtest.h>
#include <libtbag/network/http/ws/WsFrame.hpp>

#include <libtbag/bitwise/Endian.hpp>
#include <libtbag/random/MaskingDevice.hpp>

using namespace libtbag;
using namespace libtbag::network;
using namespace libtbag::network::http;
using namespace libtbag::network::http::ws;

TEST(WsFrameTest, RequestFrame)
{
    uint8_t const REQUEST_FRAME[] = {0x81, 0x9c, 0x6c, 0x11, 0xe8, 0xe3, 0x3e, 0x7e,
                                     0x8b, 0x88, 0x4c, 0x78, 0x9c, 0xc3, 0x1b, 0x78,
                                     0x9c, 0x8b, 0x4c, 0x59, 0xbc, 0xae, 0x20, 0x24,
                                     0xc8, 0xb4, 0x09, 0x73, 0xbb, 0x8c, 0x0f, 0x7a,
                                     0x8d, 0x97};
    std::string const RESULT_STRING = "Rock it with HTML5 WebSocket";

    // Request buffer to frame.
    WsFrame frame;
    ASSERT_EQ(Err::E_SUCCESS, frame.execute((char const *)REQUEST_FRAME, sizeof(REQUEST_FRAME)));
    ASSERT_TRUE(frame.fin);
    ASSERT_FALSE(frame.rsv1);
    ASSERT_FALSE(frame.rsv2);
    ASSERT_FALSE(frame.rsv3);
    ASSERT_TRUE(frame.mask);
    ASSERT_EQ(bitwise::toNetwork(0x6c11e8e3), frame.masking_key);

    char const * data = frame.getPayloadPtr();
    std::size_t size = frame.getPayloadSize();
    std::string payload(data, data + size);
    ASSERT_EQ(size, frame.getPayloadSize());
    ASSERT_EQ(RESULT_STRING, payload);

    // Frame to request buffer.
    std::vector<char> buffer;
    buffer.resize(sizeof(REQUEST_FRAME));
    ASSERT_EQ(sizeof(REQUEST_FRAME), frame.copyTo(buffer));

    std::string const lh(REQUEST_FRAME, REQUEST_FRAME + sizeof(REQUEST_FRAME));
    std::string const rh(buffer.begin(), buffer.end());
    ASSERT_EQ(lh, rh);
}

//TEST(WsFrameTest, GetResponseWebSocket)
//{
//    std::string const TEST_REQUEST = "GET /chat HTTP/1.1\r\n"
//            "Host: server.example.com\r\n"
//            "Upgrade: websocket\r\n"
//            "Connection: Upgrade\r\n"
//            "Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==\r\n" // Base64 original key: "the sample nonce"
//            "Origin: http://example.com\r\n"
//            "Sec-WebSocket-Protocol: chat, superchat\r\n"
//            "Sec-WebSocket-Version: 13\r\n\r\n";
//
//    std::string const TEST_RESPONSE = "HTTP/1.1 101 Switching Protocols\r\n"
//            "Upgrade: WebSocket\r\n"
//            "Connection: Upgrade\r\n"
//            "Sec-WebSocket-Accept: s3pPLMBiTxaQ9kYGzzhZRbK+xOo=\r\n\r\n";
//
//    HttpParser request;
//    request.execute(TEST_REQUEST.c_str(), TEST_REQUEST.size());
//    ASSERT_TRUE(request.isComplete());
//
//    HttpBuilder response;
//    ASSERT_EQ(Err::E_SUCCESS, updateResponseWebSocket(request, response));
//
//    HttpParser test_response;
//    test_response.execute(TEST_RESPONSE.c_str(), TEST_RESPONSE.size());
//    ASSERT_TRUE(test_response.isComplete());
//
//    ASSERT_EQ(test_response.getHttpMajor(), response.getMajor());
//    ASSERT_EQ(test_response.getHttpMinor(), response.getMinor());
//    ASSERT_EQ(test_response.getStatusCode(), response.getStatus());
//    ASSERT_EQ(test_response.getHeader(HEADER_UPGRADE), response.getHeader(HEADER_UPGRADE));
//    ASSERT_EQ(test_response.getHeader(HEADER_CONNECTION), response.getHeader(HEADER_CONNECTION));
//    ASSERT_EQ(test_response.getHeader(HEADER_SEC_WEBSOCKET_ACCEPT), response.getHeader(HEADER_SEC_WEBSOCKET_ACCEPT));
//}

TEST(WsFrameTest, LargeData)
{
    WsFrame sender;
    WsFrame receiver;
    random::MaskingDevice device;

    std::size_t const BUFFER_SIZE = 65536;
    util::Buffer test_buffer;
    test_buffer.assign(BUFFER_SIZE, 0x10);
    util::Buffer buffer;

    sender.binary(test_buffer, device.gen());
    auto const REQUEST_SIZE = sender.copyTo(buffer);
    ASSERT_EQ(BUFFER_SIZE + 2/*header*/ + 8/*payload_length*/ + 4/*masking*/, REQUEST_SIZE);
    ASSERT_EQ(REQUEST_SIZE, buffer.size());

    ASSERT_EQ(Err::E_SUCCESS, receiver.execute(buffer.data(), REQUEST_SIZE));
    ASSERT_EQ(WsOpCode::WSOC_BINARY_FRAME, receiver.opcode);
    ASSERT_TRUE(receiver.fin);
    ASSERT_FALSE(receiver.rsv1);
    ASSERT_FALSE(receiver.rsv2);
    ASSERT_FALSE(receiver.rsv3);
    ASSERT_TRUE(receiver.mask);
    ASSERT_NE(0, receiver.masking_key);

    auto result = receiver.toPayloadBinary();
    ASSERT_EQ(test_buffer.size(), result.size());
    for (std::size_t i = 0; i < BUFFER_SIZE; ++i) {
        ASSERT_EQ(test_buffer[i], result[i]);
    }
}

TEST(WsFrameTest, TextRequest)
{
    WsFrame sender;
    WsFrame receiver;
    random::MaskingDevice device;

    std::string const TEST_TEXT = "TEST ECHO MESSAGE!";
    util::Buffer buffer;

    sender.text(TEST_TEXT, device.gen());
    auto const REQUEST_SIZE = sender.copyTo(buffer);
    ASSERT_LT(0, REQUEST_SIZE);

    ASSERT_EQ(Err::E_SUCCESS, receiver.execute(buffer.data(), REQUEST_SIZE));
    ASSERT_EQ(WsOpCode::WSOC_TEXT_FRAME, receiver.opcode);
    ASSERT_TRUE(receiver.fin);
    ASSERT_FALSE(receiver.rsv1);
    ASSERT_FALSE(receiver.rsv2);
    ASSERT_FALSE(receiver.rsv3);
    ASSERT_TRUE(receiver.mask);
    ASSERT_NE(0, receiver.masking_key);
    ASSERT_EQ(TEST_TEXT, receiver.toPayloadString());
}

TEST(WsFrameTest, TextResponse)
{
    WsFrame sender;
    WsFrame receiver;
    random::MaskingDevice device;

    std::string const TEST_TEXT = "TEST ECHO MESSAGE!";
    util::Buffer buffer;

    sender.text(TEST_TEXT);
    auto const RESPONSE_SIZE = sender.copyTo(buffer);
    ASSERT_LT(0, RESPONSE_SIZE);

    ASSERT_EQ(Err::E_SUCCESS, receiver.execute(buffer.data(), RESPONSE_SIZE));
    ASSERT_EQ(WsOpCode::WSOC_TEXT_FRAME, receiver.opcode);
    ASSERT_TRUE(receiver.fin);
    ASSERT_FALSE(receiver.rsv1);
    ASSERT_FALSE(receiver.rsv2);
    ASSERT_FALSE(receiver.rsv3);
    ASSERT_FALSE(receiver.mask);
    ASSERT_EQ(0, receiver.masking_key);
    ASSERT_EQ(TEST_TEXT, receiver.toPayloadString());
}

TEST(WsFrameTest, PingRequest)
{
    WsFrame sender;
    WsFrame receiver;
    random::MaskingDevice device;

    std::string const TEST_TEXT = "TEST ECHO MESSAGE!";
    util::Buffer buffer;

    sender.ping(TEST_TEXT, device.gen());
    auto const RESPONSE_SIZE = sender.copyTo(buffer);
    ASSERT_LT(0, RESPONSE_SIZE);

    ASSERT_EQ(Err::E_SUCCESS, receiver.execute(buffer.data(), RESPONSE_SIZE));
    ASSERT_EQ(WsOpCode::WSOC_DENOTES_PING, receiver.opcode);
    ASSERT_TRUE(receiver.fin);
    ASSERT_FALSE(receiver.rsv1);
    ASSERT_FALSE(receiver.rsv2);
    ASSERT_FALSE(receiver.rsv3);
    ASSERT_TRUE(receiver.mask);
    ASSERT_NE(0, receiver.masking_key);
    ASSERT_EQ(TEST_TEXT, receiver.toPayloadString());
}

TEST(WsFrameTest, CloseResponse)
{
    WsFrame sender;
    WsFrame receiver;
    random::MaskingDevice device;
    util::Buffer buffer;

    auto code = WsStatusCode::WSSC_GOING_AWAY;
    sender.close(code);
    auto const RESPONSE_SIZE = sender.copyTo(buffer);
    ASSERT_LT(0, RESPONSE_SIZE);

    ASSERT_EQ(Err::E_SUCCESS, receiver.execute(buffer.data(), RESPONSE_SIZE));
    ASSERT_EQ(WsOpCode::WSOC_CONNECTION_CLOSE, receiver.opcode);
    ASSERT_TRUE(receiver.fin);
    ASSERT_FALSE(receiver.rsv1);
    ASSERT_FALSE(receiver.rsv2);
    ASSERT_FALSE(receiver.rsv3);
    ASSERT_FALSE(receiver.mask);
    ASSERT_EQ(0, receiver.masking_key);

    ASSERT_EQ(getWsStatusCodeNumber(code), receiver.getStatusCode());
    ASSERT_STREQ(getWsStatusCodeName(code), receiver.getReason().c_str());
}

TEST(WsFrameTest, Case01)
{
    uint8_t const REQUEST_FRAME[] = {
            0x82, 0xD0, 0xA6, 0xF2, 0xD5, 0x9E, 0xB2, 0xF2, 0xD5, 0x9E, 0xA6, 0xF2, 0xD5, 0x9E, 0xA6, 0xF2,
            0xDF, 0x9E, 0xB6, 0xF2, 0xDD, 0x9E, 0xA1, 0xF2, 0xD9, 0x9E, 0xAC, 0xF2, 0xD5, 0x9E, 0xA6, 0xF2,
            0xD5, 0x9F, 0xBA, 0xF2, 0xD5, 0x9E, 0xAE, 0xF2, 0xD5, 0x9E, 0xA2, 0xF2, 0xD3, 0x9E, 0xA2, 0xF2,
            0xD5, 0x9E, 0xA6, 0xF2, 0xDF, 0x9E, 0xB2, 0xF2, 0xDD, 0x9E, 0xA6, 0xF2, 0xD1, 0x9E, 0xAC, 0xF2,
            0xD5, 0x9E, 0xB6, 0xF2, 0xD5, 0x9E, 0x81, 0x9F, 0x9E, 0x5B, 0x33, 0xD9, 0xD5, 0x9E, 0xA6, 0xF2,
            0xD5, 0x9E, 0xA6, 0xF2, 0xD5, 0x9E};
    std::vector<char> req(REQUEST_FRAME, REQUEST_FRAME + sizeof(REQUEST_FRAME));
    ASSERT_EQ(86, req.size());

    WsFrame frame;
    ASSERT_EQ(Err::E_SUCCESS, frame.execute(req.data(), req.size()));
    ASSERT_EQ(WsOpCode::WSOC_BINARY_FRAME, frame.opcode);
    ASSERT_EQ(req.size() - 6, frame.payload_length);
    ASSERT_TRUE(frame.fin);
}

