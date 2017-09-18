/**
 * @file   WsFrameTest.cpp
 * @brief  WsFrame class tester.
 * @author zer0
 * @date   2017-06-11
 * @date   2017-08-07 (Rename: WebSocketFrameTest -> WsFrameTest)
 */

#include <gtest/gtest.h>
#include <libtbag/network/http/WsFrame.hpp>
#include <libtbag/bitwise/Endian.hpp>
#include <libtbag/random/MaskingDevice.hpp>

using namespace libtbag;
using namespace libtbag::network;
using namespace libtbag::network::http;

TEST(WsFrameTest, GetMaskData)
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
    ASSERT_EQ(OUTPUT, WsFrame::getPayloadData(WsFrame::getMaskingKey(MASKING_KEY), INPUT));
    ASSERT_EQ(INPUT, WsFrame::getPayloadData(WsFrame::getMaskingKey(MASKING_KEY), OUTPUT));
}

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
    ASSERT_EQ(Err::E_SUCCESS, frame.execute(REQUEST_FRAME, sizeof(REQUEST_FRAME)));
    ASSERT_TRUE(frame.fin);
    ASSERT_FALSE(frame.rsv1);
    ASSERT_FALSE(frame.rsv2);
    ASSERT_FALSE(frame.rsv3);
    ASSERT_TRUE(frame.mask);
    ASSERT_EQ(bitwise::toNetwork(0x6c11e8e3), frame.masking_key);

    char const * data = (char const *)frame.getPayloadDataPtr();
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

TEST(WsFrameTest, GetResponseWebSocket)
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

TEST(WsFrameTest, UpgradeWebsocketKey)
{
    std::string const TEST_ORIGINAL = "dGhlIHNhbXBsZSBub25jZQ==";
    std::string const TEST_RESULT   = "s3pPLMBiTxaQ9kYGzzhZRbK+xOo=";
    ASSERT_EQ(TEST_RESULT, getUpgradeWebSocketKey(TEST_ORIGINAL));
}

TEST(WsFrameTest, PayloadLength)
{
    for (uint8_t i = 0; i <= 125; ++i) {
        ASSERT_TRUE(PayloadBit::PL_BIT_7 == WsFrame::getPayloadBit(i));
        ASSERT_TRUE(PayloadBit::PL_BIT_7 == WsFrame::getPayloadBitWithPayloadLength(i));
    }

    ASSERT_TRUE(PayloadBit::PL_BIT_16 == WsFrame::getPayloadBit(126));
    ASSERT_TRUE(PayloadBit::PL_BIT_16 == WsFrame::getPayloadBitWithPayloadLength(126));
    ASSERT_TRUE(PayloadBit::PL_BIT_16 == WsFrame::getPayloadBitWithPayloadLength(65535));

    ASSERT_TRUE(PayloadBit::PL_BIT_64 == WsFrame::getPayloadBit(127));
    ASSERT_TRUE(PayloadBit::PL_BIT_64 == WsFrame::getPayloadBitWithPayloadLength(65536));
    ASSERT_TRUE(PayloadBit::PL_BIT_64 == WsFrame::getPayloadBitWithPayloadLength(100000));
}

TEST(WsFrameTest, LargeData)
{
    WsFrame sender;
    WsFrame receiver;
    random::MaskingDevice device;

    std::size_t const BUFFER_SIZE = 65536;
    WsFrame::WsBuffer test_buffer;
    test_buffer.assign(BUFFER_SIZE, 0x10);
    WsFrame::WsBuffer buffer;

    ASSERT_EQ(Err::E_SUCCESS, sender.binary(test_buffer, device.gen()));
    auto const REQUEST_SIZE = sender.copyTo(buffer);
    ASSERT_EQ(BUFFER_SIZE + 2/*header*/ + 8/*payload_length*/ + 4/*masking*/, REQUEST_SIZE);
    ASSERT_EQ(REQUEST_SIZE, buffer.size());

    ASSERT_EQ(Err::E_SUCCESS, receiver.execute(buffer.data(), REQUEST_SIZE));
    ASSERT_EQ(OpCode::OC_BINARY_FRAME, receiver.opcode);
    ASSERT_TRUE(receiver.fin);
    ASSERT_FALSE(receiver.rsv1);
    ASSERT_FALSE(receiver.rsv2);
    ASSERT_FALSE(receiver.rsv3);
    ASSERT_TRUE(receiver.mask);
    ASSERT_NE(0, receiver.masking_key);

    auto result = receiver.toBinary();
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
    WsFrame::WsBuffer buffer;

    ASSERT_EQ(Err::E_SUCCESS, sender.text(TEST_TEXT, device.gen()));
    auto const REQUEST_SIZE = sender.copyTo(buffer);
    ASSERT_LT(0, REQUEST_SIZE);

    ASSERT_EQ(Err::E_SUCCESS, receiver.execute(buffer.data(), REQUEST_SIZE));
    ASSERT_EQ(OpCode::OC_TEXT_FRAME, receiver.opcode);
    ASSERT_TRUE(receiver.fin);
    ASSERT_FALSE(receiver.rsv1);
    ASSERT_FALSE(receiver.rsv2);
    ASSERT_FALSE(receiver.rsv3);
    ASSERT_TRUE(receiver.mask);
    ASSERT_NE(0, receiver.masking_key);
    ASSERT_EQ(TEST_TEXT, receiver.toText());
}

TEST(WsFrameTest, TextResponse)
{
    WsFrame sender;
    WsFrame receiver;
    random::MaskingDevice device;

    std::string const TEST_TEXT = "TEST ECHO MESSAGE!";
    WsFrame::WsBuffer buffer;

    ASSERT_EQ(Err::E_SUCCESS, sender.text(TEST_TEXT));
    auto const RESPONSE_SIZE = sender.copyTo(buffer);
    ASSERT_LT(0, RESPONSE_SIZE);

    ASSERT_EQ(Err::E_SUCCESS, receiver.execute(buffer.data(), RESPONSE_SIZE));
    ASSERT_EQ(OpCode::OC_TEXT_FRAME, receiver.opcode);
    ASSERT_TRUE(receiver.fin);
    ASSERT_FALSE(receiver.rsv1);
    ASSERT_FALSE(receiver.rsv2);
    ASSERT_FALSE(receiver.rsv3);
    ASSERT_FALSE(receiver.mask);
    ASSERT_EQ(0, receiver.masking_key);
    ASSERT_EQ(TEST_TEXT, receiver.toText());
}

TEST(WsFrameTest, PingRequest)
{
    WsFrame sender;
    WsFrame receiver;
    random::MaskingDevice device;

    std::string const TEST_TEXT = "TEST ECHO MESSAGE!";
    WsFrame::WsBuffer buffer;

    ASSERT_EQ(Err::E_SUCCESS, sender.ping(TEST_TEXT, device.gen()));
    auto const RESPONSE_SIZE = sender.copyTo(buffer);
    ASSERT_LT(0, RESPONSE_SIZE);

    ASSERT_EQ(Err::E_SUCCESS, receiver.execute(buffer.data(), RESPONSE_SIZE));
    ASSERT_EQ(OpCode::OC_DENOTES_PING, receiver.opcode);
    ASSERT_TRUE(receiver.fin);
    ASSERT_FALSE(receiver.rsv1);
    ASSERT_FALSE(receiver.rsv2);
    ASSERT_FALSE(receiver.rsv3);
    ASSERT_TRUE(receiver.mask);
    ASSERT_NE(0, receiver.masking_key);
    ASSERT_EQ(TEST_TEXT, receiver.toText());
}

TEST(WsFrameTest, CloseResponse)
{
    WsFrame sender;
    WsFrame receiver;
    random::MaskingDevice device;
    WsFrame::WsBuffer buffer;

    auto code = WsStatusCode::WSSC_GOING_AWAY;
    ASSERT_EQ(Err::E_SUCCESS, sender.close(code));
    auto const RESPONSE_SIZE = sender.copyTo(buffer);
    ASSERT_LT(0, RESPONSE_SIZE);

    ASSERT_EQ(Err::E_SUCCESS, receiver.execute(buffer.data(), RESPONSE_SIZE));
    ASSERT_EQ(OpCode::OC_CONNECTION_CLOSE, receiver.opcode);
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
    uint8_t const REQUEST_FRAME1[] = {
            0x82, 0xD0, 0xA6, 0xF2, 0xD5, 0x9E, 0xB2, 0xF2, 0xD5, 0x9E, 0xA6, 0xF2, 0xD5, 0x9E, 0xA6, 0xF2,
            0xDF, 0x9E, 0xB6, 0xF2, 0xDD, 0x9E, 0xA1, 0xF2, 0xD9, 0x9E, 0xAC, 0xF2, 0xD5, 0x9E, 0xA6, 0xF2,
            0xD5, 0x9F, 0xBA, 0xF2, 0xD5, 0x9E, 0xAE, 0xF2, 0xD5, 0x9E, 0xA2, 0xF2, 0xD3, 0x9E, 0xA2, 0xF2,
            0xD5, 0x9E, 0xA6, 0xF2, 0xDF, 0x9E, 0xB2, 0xF2, 0xDD, 0x9E, 0xA6, 0xF2, 0xD1, 0x9E, 0xAC, 0xF2,
            0xD5, 0x9E, 0xB6, 0xF2, 0xD5, 0x9E, 0x81, 0x9F, 0x9E, 0x5B, 0x33, 0xD9, 0xD5, 0x9E, 0xA6, 0xF2,
            0xD5, 0x9E, 0xA6, 0xF2, 0xD5, 0x9E};
    uint8_t const REQUEST_FRAME2[] = {
            0x82, 0xE8, 0xA3, 0x2B, 0x69, 0xF9, 0xB7, 0x2B, 0x69, 0xF9, 0xA3, 0x2B, 0x69, 0xF9, 0xA3, 0x2B,
            0x63, 0xF9, 0xB1, 0x2B, 0x61, 0xF9, 0xA4, 0x2B, 0x65, 0xF9, 0xA9, 0x2B, 0x69, 0xF9, 0xA3, 0x2B,
            0x69, 0xFA, 0x97, 0x2B, 0x69, 0xF9, 0xAF, 0x2B, 0x69, 0xF9, 0xA3, 0x2B, 0x6F, 0xF9, 0xAB, 0x2B,
            0x6D, 0xF9, 0xA5, 0x2B, 0x69, 0xF9, 0xA7, 0x2B, 0x69, 0xF9, 0xAF, 0x2B, 0x69, 0xF9, 0xF7, 0x6E,
            0x3A, 0xAD, 0xFC, 0x66, 0x2C, 0xAA, 0xF0, 0x6A, 0x2E, 0xBC, 0xA3, 0x2B, 0x63, 0xF9, 0xB7, 0x2B,
            0x61, 0xF9, 0xA3, 0x2B, 0x6D, 0xF9, 0xA9, 0x2B, 0x69, 0xF9, 0xB3, 0x2B, 0x69, 0xF9, 0x57, 0x13,
            0x9B, 0x4A, 0x35, 0x00, 0x69, 0xF9, 0xA3, 0x2B, 0x69, 0xF9, 0xA3, 0x2B, 0x69, 0xF9};
    uint8_t const REQUEST_FRAME3[] = {
            0x82, 0xD0, 0x4A, 0xFB, 0xE1, 0xF1, 0x5E, 0xFB, 0xE1, 0xF1, 0x4A, 0xFB, 0xE1, 0xF1, 0x4A, 0xFB,
            0xEB, 0xF1, 0x5A, 0xFB, 0xE9, 0xF1, 0x4D, 0xFB, 0xED, 0xF1, 0x40, 0xFB, 0xE1, 0xF1, 0x4A, 0xFB,
            0xE1, 0xF0, 0x56, 0xFB, 0xE1, 0xF1, 0x42, 0xFB, 0xE1, 0xF1, 0x4E, 0xFB, 0xE7, 0xF1, 0x4E, 0xFB,
            0xE1, 0xF1, 0x4A, 0xFB, 0xEB, 0xF1, 0x5E, 0xFB, 0xE9, 0xF1, 0x4A, 0xFB, 0xE5, 0xF1, 0x40, 0xFB,
            0xE1, 0xF1, 0x5A, 0xFB, 0xE1, 0xF1, 0xF8, 0x03, 0x3E, 0x97, 0xDD, 0xD0, 0xE1, 0xF1, 0x4A, 0xFB,
            0xE1, 0xF1, 0x4A, 0xFB, 0xE1, 0xF1, 0xA9, 0x2B, 0x69, 0xF9, 0xB3, 0x2B, 0x69, 0xF9, 0x57, 0x13,
            0x9B, 0x4A, 0x35, 0x00, 0x69, 0xF9, 0xA3, 0x2B, 0x69, 0xF9, 0xA3, 0x2B, 0x69, 0xF9};
    uint8_t const REQUEST_FRAME4[] = {
            0x82, 0xE8, 0x42, 0xCB, 0x6A, 0xB3, 0x56, 0xCB, 0x6A, 0xB3, 0x42, 0xCB, 0x6A, 0xB3, 0x42, 0xCB,
            0x60, 0xB3, 0x50, 0xCB, 0x62, 0xB3, 0x45, 0xCB, 0x66, 0xB3, 0x48, 0xCB, 0x6A, 0xB3, 0x42, 0xCB,
            0x6A, 0xB0, 0x76, 0xCB, 0x6A, 0xB3, 0x4E, 0xCB, 0x6A, 0xB3, 0x42, 0xCB, 0x6C, 0xB3, 0x4A, 0xCB,
            0x6E, 0xB3, 0x44, 0xCB, 0x6A, 0xB3, 0x46, 0xCB, 0x6A, 0xB3, 0x4E, 0xCB, 0x6A, 0xB3, 0x16, 0x8E,
            0x39, 0xE7, 0x1D, 0x86, 0x2F, 0xE0, 0x11, 0x8A, 0x2D, 0xF6, 0x42, 0xCB, 0x60, 0xB3, 0x56, 0xCB,
            0x62, 0xB3, 0x42, 0xCB, 0x6E, 0xB3, 0x48, 0xCB, 0x6A, 0xB3, 0x52, 0xCB, 0x6A, 0xB3, 0xCC, 0x7C,
            0x16, 0xFB, 0xD8, 0xE0, 0x6A, 0xB3, 0x42, 0xCB, 0x6A, 0xB3, 0x42, 0xCB, 0x6A, 0xB3};
    std::vector<char> req1(REQUEST_FRAME1, REQUEST_FRAME1 + sizeof(REQUEST_FRAME1));
    std::vector<char> req2(REQUEST_FRAME2, REQUEST_FRAME2 + sizeof(REQUEST_FRAME2));
    std::vector<char> req3(REQUEST_FRAME3, REQUEST_FRAME3 + sizeof(REQUEST_FRAME3));
    std::vector<char> req4(REQUEST_FRAME4, REQUEST_FRAME4 + sizeof(REQUEST_FRAME4));
    ASSERT_EQ( 86, req1.size());
    ASSERT_EQ(110, req2.size());
    ASSERT_EQ(110, req3.size());
    ASSERT_EQ(110, req4.size());

    WsFrame frame;
    Err execute_code;

    execute_code = frame.execute((uint8_t const *)req1.data(), req1.size());
    ASSERT_EQ(Err::E_SUCCESS, execute_code);
    ASSERT_EQ(OpCode::OC_BINARY_FRAME, frame.opcode);
    ASSERT_TRUE(frame.fin);

    execute_code = frame.execute((uint8_t const *)req2.data(), req2.size());
    ASSERT_EQ(Err::E_SUCCESS, execute_code);
    ASSERT_EQ(OpCode::OC_BINARY_FRAME, frame.opcode);
    ASSERT_TRUE(frame.fin);

    execute_code = frame.execute((uint8_t const *)req3.data(), req3.size());
    ASSERT_EQ(Err::E_SUCCESS, execute_code);
    ASSERT_EQ(OpCode::OC_BINARY_FRAME, frame.opcode);
    ASSERT_TRUE(frame.fin);

    execute_code = frame.execute((uint8_t const *)req4.data(), req4.size());
    ASSERT_EQ(Err::E_SUCCESS, execute_code);
    ASSERT_EQ(OpCode::OC_BINARY_FRAME, frame.opcode);
    ASSERT_TRUE(frame.fin);
}

