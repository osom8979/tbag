/**
 * @file   WsFrameBufferTest.cpp
 * @brief  WsFrameBuffer class tester.
 * @author zer0
 * @date   2017-10-01
 * @date   2018-12-25 (Change namespace: libtbag::network::http::base -> libtbag::http)
 */

#include <gtest/gtest.h>
#include <libtbag/http/WsFrameBuffer.hpp>
#include <libtbag/bitwise/Endian.hpp>

#include <vector>

using namespace libtbag;
using namespace libtbag::http;

TEST(WsFrameBufferTest, Default)
{
    uint8_t const REQUEST_FRAME1[] = {0x01, 0x9c, 0x6c, 0x11, 0xe8, 0xe3, 0x3e, 0x7e,
                                      0x8b, 0x88, 0x4c, 0x78, 0x9c, 0xc3, 0x1b, 0x78,
                                      0x9c, 0x8b, 0x4c, 0x59, 0xbc, 0xae, 0x20, 0x24,
                                      0xc8, 0xb4, 0x09, 0x73, 0xbb, 0x8c, 0x0f, 0x7a,
                                      0x8d, 0x97};
    uint8_t const REQUEST_FRAME2[] = {0x80, 0x9c, 0x6c, 0x11, 0xe8, 0xe3, 0x3e, 0x7e,
                                      0x8b, 0x88, 0x4c, 0x78, 0x9c, 0xc3, 0x1b, 0x78,
                                      0x9c, 0x8b, 0x4c, 0x59, 0xbc, 0xae, 0x20, 0x24,
                                      0xc8, 0xb4, 0x09, 0x73, 0xbb, 0x8c, 0x0f, 0x7a,
                                      0x8d, 0x97};
    std::vector<char> req1(REQUEST_FRAME1, REQUEST_FRAME1 + sizeof(REQUEST_FRAME1));
    std::vector<char> req2(REQUEST_FRAME2, REQUEST_FRAME2 + sizeof(REQUEST_FRAME2));
    std::string const RESULT_STRING1 = "Rock it with HTML5 WebSocket";
    std::string const RESULT_STRING2 = "Rock it with HTML5 WebSocketRock it with HTML5 WebSocket";

    WsFrameBuffer wsbuf;
    wsbuf.push(req1.data(), req1.size());
    ASSERT_FALSE(wsbuf.next());

    std::size_t hit_count = 0;
    WsOpCode opcode_result = WsOpCode::WSOC_CONTINUATION_FRAME;
    std::string result;

    wsbuf.push(req2.data(), req2.size());
    while (wsbuf.next()) {
        opcode_result = wsbuf.getOpCode();
        result.assign(wsbuf.atPayload().begin(), wsbuf.atPayload().end());
        ++hit_count;
    }

    ASSERT_EQ(1, hit_count);
    ASSERT_EQ(RESULT_STRING2, result);
    ASSERT_EQ(WsOpCode::WSOC_TEXT_FRAME, opcode_result);
}

TEST(WsFrameBufferTest, SplitPayload)
{
    uint8_t const REQUEST_FRAME1[] = {0x81, 0x9c, 0x6c, 0x11, 0xe8, 0xe3, 0x3e, 0x7e,
                                      0x8b, 0x88, 0x4c, 0x78, 0x9c, 0xc3, 0x1b, 0x78};
    uint8_t const REQUEST_FRAME2[] = {0x9c, 0x8b, 0x4c, 0x59, 0xbc, 0xae, 0x20, 0x24,
                                      0xc8, 0xb4, 0x09, 0x73, 0xbb, 0x8c, 0x0f, 0x7a,
                                      0x8d, 0x97};
    std::vector<char> req1(REQUEST_FRAME1, REQUEST_FRAME1 + sizeof(REQUEST_FRAME1));
    std::vector<char> req2(REQUEST_FRAME2, REQUEST_FRAME2 + sizeof(REQUEST_FRAME2));
    std::string const RESULT_STRING = "Rock it with HTML5 WebSocket";

    WsFrameBuffer wsbuf;
    wsbuf.push(req1.data(), req1.size());
    ASSERT_FALSE(wsbuf.next());

    std::size_t hit_count = 0;
    WsOpCode opcode_result = WsOpCode::WSOC_CONTINUATION_FRAME;
    std::string result;

    wsbuf.push(req2.data(), req2.size());
    while (wsbuf.next()) {
        opcode_result = wsbuf.getOpCode();
        result.assign(wsbuf.atPayload().begin(), wsbuf.atPayload().end());
        ++hit_count;
    }

    ASSERT_EQ(1, hit_count);
    ASSERT_EQ(RESULT_STRING, result);
    ASSERT_EQ(WsOpCode::WSOC_TEXT_FRAME, opcode_result);
}

