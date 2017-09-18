/**
 * @file   WsFrameBufferTest.cpp
 * @brief  WsFrameBuffer class tester.
 * @author zer0
 * @date   2017-08-07
 */

#include <gtest/gtest.h>
#include <libtbag/network/http/WsFrameBuffer.hpp>
#include <libtbag/bitwise/Endian.hpp>

#include <vector>

using namespace libtbag;
using namespace libtbag::network;
using namespace libtbag::network::http;

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

    std::size_t hit_count = 0;
    OpCode opcode_result = OpCode::OC_CONTINUATION_FRAME;
    bool finish_result = false;
    std::string result;

    WsFrameBuffer wsbuf;
    std::size_t count;

    count = wsbuf.exec(req1.data(), req1.size(), [&](OpCode code, bool finish, WsFrameBuffer::Buffer & buffer) -> bool{
        opcode_result = code;
        finish_result = finish;
        result.assign(buffer.begin(), buffer.end());
        ++hit_count;
        return true;
    });
    ASSERT_EQ(1, count);
    ASSERT_EQ(1, hit_count);
    ASSERT_EQ(RESULT_STRING1, result);
    ASSERT_EQ(OpCode::OC_TEXT_FRAME, opcode_result);
    ASSERT_FALSE(finish_result);

    hit_count = 0;
    opcode_result = OpCode::OC_CONTINUATION_FRAME;
    finish_result = false;
    result.clear();

    count = wsbuf.exec(req2.data(), req2.size(), [&](OpCode code, bool finish, WsFrameBuffer::Buffer & buffer) -> bool{
        opcode_result = code;
        finish_result = finish;
        result.assign(buffer.begin(), buffer.end());
        ++hit_count;
        return true;
    });
    ASSERT_EQ(1, count);
    ASSERT_EQ(1, hit_count);
    ASSERT_EQ(RESULT_STRING2, result);
    ASSERT_EQ(OpCode::OC_TEXT_FRAME, opcode_result);
    ASSERT_TRUE(finish_result);
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

    std::size_t hit_count = 0;
    OpCode opcode_result = OpCode::OC_CONTINUATION_FRAME;
    bool finish_result = false;
    std::string result;

    WsFrameBuffer wsbuf;
    std::size_t count;

    count = wsbuf.exec(req1.data(), req1.size(), [&](OpCode code, bool finish, WsFrameBuffer::Buffer & buffer) -> bool{
        ++hit_count;
        return true;
    });
    ASSERT_EQ(0, count);
    ASSERT_EQ(0, hit_count);

    hit_count = 0;
    opcode_result = OpCode::OC_CONTINUATION_FRAME;
    finish_result = false;
    result.clear();

    count = wsbuf.exec(req2.data(), req2.size(), [&](OpCode code, bool finish, WsFrameBuffer::Buffer & buffer) -> bool{
        opcode_result = code;
        finish_result = finish;
        result.assign(buffer.begin(), buffer.end());
        ++hit_count;
        return true;
    });
    ASSERT_EQ(1, count);
    ASSERT_EQ(1, hit_count);
    ASSERT_EQ(RESULT_STRING, result);
    ASSERT_EQ(OpCode::OC_TEXT_FRAME, opcode_result);
    ASSERT_TRUE(finish_result);
}

