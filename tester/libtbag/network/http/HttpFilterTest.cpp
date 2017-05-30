/**
 * @file   HttpFilterTest.cpp
 * @brief  HttpFilter class tester.
 * @author zer0
 * @date   2017-05-30
 */

#include <gtest/gtest.h>
#include <libtbag/network/http/HttpFilter.hpp>

using namespace libtbag;
using namespace libtbag::network;
using namespace libtbag::network::http;

TEST(HttpFilterTest, HttpDefaultFilter)
{
    HttpDefaultFilter f1;
    HttpDefaultFilter f2("GET", "/aaa.*");
    HttpDefaultFilter f3("/bbb.*");
    HttpDefaultFilter f4 = f3;
    HttpDefaultFilter f5 = std::move(f4);

    char const POST_DATA_AAA[] = "POST /aaa HTTP/1.1\r\n\r\n";
    char const POST_DATA_BBB[] = "POST /bbb HTTP/1.1\r\n\r\n";
    char const  GET_DATA_AAA[] = "GET  /aaa HTTP/1.1\r\n\r\n";
    char const  GET_DATA_BBB[] = "GET  /bbb HTTP/1.1\r\n\r\n";

    HttpParser request_post_aaa;
    HttpParser request_post_bbb;
    HttpParser request_get_aaa;
    HttpParser request_get_bbb;

    ASSERT_LT(0, request_post_aaa.execute(POST_DATA_AAA, sizeof(POST_DATA_AAA)));
    ASSERT_LT(0, request_post_bbb.execute(POST_DATA_BBB, sizeof(POST_DATA_BBB)));
    ASSERT_LT(0, request_get_aaa .execute( GET_DATA_AAA, sizeof( GET_DATA_AAA)));
    ASSERT_LT(0, request_get_bbb .execute( GET_DATA_BBB, sizeof( GET_DATA_BBB)));

    ASSERT_FALSE(f1.filter(request_post_aaa));
    ASSERT_FALSE(f1.filter(request_post_bbb));
    ASSERT_FALSE(f1.filter( request_get_aaa));
    ASSERT_FALSE(f1.filter( request_get_bbb));

    ASSERT_FALSE(f2.filter(request_post_aaa));
    ASSERT_FALSE(f2.filter(request_post_bbb));
    ASSERT_TRUE (f2.filter( request_get_aaa));
    ASSERT_FALSE(f2.filter( request_get_bbb));

    ASSERT_FALSE(f3.filter(request_post_aaa));
    ASSERT_TRUE (f3.filter(request_post_bbb));
    ASSERT_FALSE(f3.filter( request_get_aaa));
    ASSERT_TRUE (f3.filter( request_get_bbb));

    ASSERT_FALSE(f4.filter(request_post_aaa));
    ASSERT_FALSE(f4.filter(request_post_bbb));
    ASSERT_FALSE(f4.filter( request_get_aaa));
    ASSERT_FALSE(f4.filter( request_get_bbb));

    ASSERT_FALSE(f5.filter(request_post_aaa));
    ASSERT_TRUE (f5.filter(request_post_bbb));
    ASSERT_FALSE(f5.filter( request_get_aaa));
    ASSERT_TRUE (f5.filter( request_get_bbb));
}

