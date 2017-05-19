/**
 * @file   HttpParserTest.cpp
 * @brief  HttpParser class tester.
 * @author zer0
 * @date   2017-05-18
 */

#include <gtest/gtest.h>
#include <libtbag/network/http/HttpParser.hpp>

#include <iostream>

using namespace libtbag;
using namespace libtbag::network;
using namespace libtbag::network::http;

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
    ASSERT_EQ(TEST_DATA_LENGTH, http.execute(TEST_DATA, TEST_DATA_LENGTH));

    ASSERT_EQ(1, http.getHttpMajor());
    ASSERT_EQ(1, http.getHttpMinor());
    ASSERT_STREQ("github.com", http.getHeader("Host").c_str());
    ASSERT_STREQ("POST", http.getMethodName().c_str());
    ASSERT_STREQ("/joyent/http-parser", http.getUrl().c_str());

    std::cout << "Body:" << http.getBody() << std::endl;
}

TEST(HttpParserTest, Response)
{
    char const TEST_DATA[] = "HTTP/1.1 301 Moved Permanently\r\n"
            "Location: http://www.google.com/\r\n"
            "Content-Type: text/html; charset=UTF-8\r\n"
            "Date: Sun, 26 Apr 2009 11:11:49 GMT\r\n"
            "Expires: Tue, 26 May 2009 11:11:49 GMT\r\n"
            "X-$PrototypeBI-Version: 1.6.0.3\r\n" /* $ char in header field */
            "Cache-Control: public, max-age=2592000\r\n"
            "Server: gws\r\n"
            "Content-Length:  219  \r\n"
            "\r\n"
            "<HTML><HEAD><meta http-equiv=\"content-type\" content=\"text/html;charset=utf-8\">\n"
            "<TITLE>301 Moved</TITLE></HEAD><BODY>\n"
            "<H1>301 Moved</H1>\n"
            "The document has moved\n"
            "<A HREF=\"http://www.google.com/\">here</A>.\r\n"
            "</BODY></HTML>\r\n";
    std::size_t const TEST_DATA_LENGTH = sizeof(TEST_DATA) - 1;

    HttpParser http;
    ASSERT_EQ(TEST_DATA_LENGTH, http.execute(TEST_DATA, TEST_DATA_LENGTH));

    ASSERT_EQ(1, http.getHttpMajor());
    ASSERT_EQ(1, http.getHttpMinor());
    ASSERT_EQ(301, http.getStatusCode());

    std::cout << "Body:" << http.getBody() << std::endl;
}
