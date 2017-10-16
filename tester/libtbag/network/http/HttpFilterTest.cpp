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
    HttpRequest request_post_aaa;
    HttpRequest request_post_bbb;
    HttpRequest request_get_aaa;
    HttpRequest request_get_bbb;

    request_post_aaa.method = "POST";
    request_post_bbb.method = "POST";
    request_get_aaa .method = "GET";
    request_get_bbb .method = "GET";

    request_post_aaa.path = "/aaa";
    request_post_bbb.path = "/bbb";
    request_get_aaa .path = "/aaa";
    request_get_bbb .path = "/bbb";

    HttpBaseFilter f1;
    HttpBaseFilter f2("GET", "/aaa.*");
    HttpBaseFilter f3(std::regex("/bbb.*"));
    HttpBaseFilter f4 = f3;

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
    ASSERT_TRUE (f4.filter(request_post_bbb));
    ASSERT_FALSE(f4.filter( request_get_aaa));
    ASSERT_TRUE (f4.filter( request_get_bbb));
}

