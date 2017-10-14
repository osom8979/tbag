/**
 * @file   HttpHeaderTest.cpp
 * @brief  HttpHeader class tester.
 * @author zer0
 * @date   2017-09-29
 */

#include <gtest/gtest.h>
#include <libtbag/network/http/HttpHeader.hpp>
#include <iterator>

using namespace libtbag;
using namespace libtbag::network;
using namespace libtbag::network::http;

TEST(HttpHeaderTest, Default)
{
    HttpHeader header;
    ASSERT_NE(header.end(), header.insert("Host", "localhost"));
    ASSERT_NE(header.end(), header.insert("Sec-WebSocket-Version", "7, 9, 13"));
    ASSERT_NE(header.end(), header.insertIfNotExists("Upgrade", "WebSocket"));
    ASSERT_EQ(header.end(), header.insertIfNotExists("Upgrade", "TEMP"));

    ASSERT_EQ(std::string("localhost"), header.get("Host"));
    ASSERT_EQ(std::string("localhost"), header.getIgnoreCase("host"));
    ASSERT_EQ(std::string(), header.get("host"));

    ASSERT_TRUE(header.exists("Host"));
    ASSERT_FALSE(header.exists("host"));

    ASSERT_TRUE(header.existsInSplitValues("Sec-WebSocket-Version", "7"));
    ASSERT_TRUE(header.existsInSplitValues("Sec-WebSocket-Version", "9"));
    ASSERT_TRUE(header.existsInSplitValues("Sec-WebSocket-Version", "13"));

    ASSERT_FALSE(header.existsInSplitValues("Sec-WebSocket-Version", "20"));
    ASSERT_FALSE(header.existsInSplitValues("sec-websocket-version", "13")); // lower-case

    std::cout << "Header Information:\n" << header.toMessageHeader();

    header.regularization();
    ASSERT_EQ(std::string("localhost"), header.get("host"));
    ASSERT_TRUE(header.existsInSplitValues("sec-websocket-version", "13")); // lower-case
    ASSERT_FALSE(header.existsInSplitValues("Sec-WebSocket-Version", "13"));
}

TEST(HttpHeaderTest, MultiHeader)
{
    HttpHeader header;
    ASSERT_NE(header.end(), header.insert("Host", "localhost"));
    ASSERT_NE(header.end(), header.insert("Sec-WebSocket-Version", "7, 9, 13"));
    ASSERT_NE(header.end(), header.insert("Upgrade", "WebSocket"));
    ASSERT_NE(header.end(), header.insert("Upgrade", "Temp"));
    ASSERT_NE(header.end(), header.insert("Upgrade", "AAAA"));
    ASSERT_NE(header.end(), header.insert("Upgrade", "Temp"));
    ASSERT_EQ(6, header.sizeOfHeaders());

    ASSERT_TRUE(header.exists("Upgrade", "Temp"));

    auto pair = header.equal_range("Upgrade");
    ASSERT_EQ(4, std::distance(pair.first, pair.second));
}

