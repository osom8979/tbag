/**
 * @file   NngSocketTest.cpp
 * @brief  NngSocket class tester.
 * @author zer0
 * @date   2019-05-29
 */

#include <gtest/gtest.h>
#include <libtbag/mq/NngSocket.hpp>
#include <libtbag/util/BufferInfo.hpp>

using namespace libtbag;
using namespace libtbag::mq;

TEST(NngSocketTest, Default)
{
    NngSocket res;
    NngSocket req;

    ASSERT_EQ(E_SUCCESS, res.open(NngSocket::SocketType::ST_REP0));
    ASSERT_EQ(E_SUCCESS, req.open(NngSocket::SocketType::ST_REQ0));

    char const * const URL = "inproc://NngSocketTest_Default";
    ASSERT_EQ(E_SUCCESS, res.listen(URL));
    ASSERT_EQ(E_SUCCESS, req.dial(URL));

    BLOCK_DO("req -> res") {
        std::string const REQUEST_MESSAGE = "request_message";
        ASSERT_EQ(E_SUCCESS, req.send((void*)&REQUEST_MESSAGE[0], REQUEST_MESSAGE.size()));

        libtbag::util::Buffer buffer(100);
        std::size_t buffer_size = buffer.size();
        ASSERT_EQ(100, buffer_size);
        ASSERT_EQ(E_SUCCESS, res.recv((void*)&buffer[0], &buffer_size));
        ASSERT_EQ(REQUEST_MESSAGE.size(), buffer_size);
        ASSERT_EQ(REQUEST_MESSAGE, std::string(buffer.begin(), buffer.begin()+buffer_size));
    } BLOCK_END();

    BLOCK_DO("res -> req") {
        std::string const RESPONSE_MESSAGE = "response_message";
        ASSERT_EQ(E_SUCCESS, res.send((void*)&RESPONSE_MESSAGE[0], RESPONSE_MESSAGE.size()));

        libtbag::util::Buffer buffer(100);
        std::size_t buffer_size = buffer.size();
        ASSERT_EQ(100, buffer_size);
        ASSERT_EQ(E_SUCCESS, req.recv((void*)&buffer[0], &buffer_size));
        ASSERT_EQ(RESPONSE_MESSAGE.size(), buffer_size);
        ASSERT_EQ(RESPONSE_MESSAGE, std::string(buffer.begin(), buffer.begin()+buffer_size));
    } BLOCK_END();
}

