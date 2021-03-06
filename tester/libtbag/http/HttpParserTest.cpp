/**
 * @file   HttpParserTest.cpp
 * @brief  HttpParser class tester.
 * @author zer0
 * @date   2017-10-03
 * @date   2018-12-25 (Change namespace: libtbag::network::http::base -> libtbag::http)
 */

#include <gtest/gtest.h>
#include <libtbag/http/HttpParser.hpp>

using namespace libtbag;
using namespace libtbag::http;

TEST(HttpParserTest, Request)
{
    char const TEST_DATA[] = "POST /joyent/http-parser HTTP/1.1\r\n"
            "Host: github.com\r\n"
            "DNT: 1\r\n"
            "Accept-Encoding: gzip, deflate, sdch\r\n"
            "Accept-Language: ru-RU,ru;q=0.8,en-US;q=0.6,en;q=0.4\r\n"
            "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_10_1) "
            "AppleWebKit/537.36 (KHTML, like Gecko) "
            "Chrome/39.0.2171.65 Safari/537.36\r\n"
            "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,"
            "image/webp,*/*;q=0.8\r\n"
            "Referer: https://github.com/joyent/http-parser\r\n"
            "Connection: keep-alive\r\n"
            "Transfer-Encoding: chunked\r\n"
            "Cache-Control: max-age=0\r\n\r\nb\r\nhello world\r\n0\r\n\r\n";
    std::size_t const TEST_DATA_LENGTH = sizeof(TEST_DATA) - 1;

    HttpParser http;
    std::size_t read_size = 0;

    ASSERT_EQ(E_SUCCESS, http.execute(TEST_DATA, TEST_DATA_LENGTH, &read_size));
    ASSERT_EQ(TEST_DATA_LENGTH, read_size);

    ASSERT_EQ(1, http.property().http_major);
    ASSERT_EQ(1, http.property().http_minor);
    ASSERT_EQ(10U, http.property().header.size());
    ASSERT_STREQ("github.com", getHeaderValue(http.property().header, "Host").c_str());
    ASSERT_STREQ("POST", http.property().method.c_str());
    ASSERT_STREQ("/joyent/http-parser", http.property().path.c_str());
    ASSERT_LT(0, http.property().body.size());
}

TEST(HttpParserTest, Response)
{
    std::vector<std::string> data;
    data.push_back("HTTP/1.1 301 Moved Permanently\r\n");
    data.push_back("Location: http://www.google.com/\r\n");
    data.push_back("Content-Type: text/html; charset=UTF-8\r\n");
    data.push_back("Date: Sun, 26 Apr 2009 11:11:49 GMT\r\n");
    data.push_back("Expires: Tue, 26 May 2009 11:11:49 GMT\r\n");
    data.push_back("X-$PrototypeBI-Version: 1.6.0.3\r\n" /* $ char in header field */);
    data.push_back("Cache-Control: public, max-age=2592000\r\n");
    data.push_back("Server: gws\r\n");
    data.push_back("Content-Length:  219  \r\n");
    data.push_back("\r\n");
    data.push_back("<HTML><HEAD><meta http-equiv=\"content-type\" content=\"text/html;charset=utf-8\">\n");
    data.push_back("<TITLE>301 Moved</TITLE></HEAD><BODY>\n");
    data.push_back("<H1>301 Moved</H1>\n");
    data.push_back("The document has moved\n");
    data.push_back("<A HREF=\"http://www.google.com/\">here</A>.\r\n");
    data.push_back("</BODY></HTML>\r\n");

    std::size_t const SIZE = data.size();
    HttpParser http;

    for (std::size_t i = 0; i < SIZE; ++i) {
        ASSERT_EQ(E_SUCCESS, http.execute(data[i].data(), data[i].size()));

        if (i + 1 == SIZE) {
            ASSERT_TRUE(http.isFinish());
        } else {
            ASSERT_FALSE(http.isFinish());
        }
    }

    ASSERT_EQ(1, http.property().http_major);
    ASSERT_EQ(1, http.property().http_minor);
    ASSERT_EQ(8U, http.property().header.size());
    ASSERT_EQ(301, http.property().code);
    ASSERT_LT(0, http.property().body.size());
}

TEST(HttpParserTest, DefaultRequest)
{
    std::string buffer;
    buffer += "GET / HTTP/1.1\r\n";
    buffer += "Accept:*/*\r\n";
    buffer += "Host:localhost\r\n";
    buffer += "User-Agent:libtbag/0.9.0\r\n";
    buffer += "\r\n";

    HttpParser http;
    ASSERT_EQ(E_SUCCESS, http.execute(buffer.data(), buffer.size()));
    ASSERT_TRUE(http.isFinish());

    ASSERT_EQ(1, http.property().http_major);
    ASSERT_EQ(1, http.property().http_minor);
    ASSERT_EQ(0, http.property().body.size());
}

TEST(HttpParserTest, DefaultResponse)
{
    std::string buffer;
    buffer += "HTTP/1.1 200 OK\r\n";
    buffer += "Content-Type:text/html; charset=utf-8\r\n";
    buffer += "Content-Length:0\r\n";
    buffer += "Server:libtbag/0.9.0\r\n";
    buffer += "\r\n";

    HttpParser http;
    ASSERT_EQ(E_SUCCESS, http.execute(buffer.data(), buffer.size()));
    ASSERT_TRUE(http.isFinish());

    ASSERT_EQ(1, http.property().http_major);
    ASSERT_EQ(1, http.property().http_minor);
    ASSERT_EQ(0, http.property().body.size());
}

