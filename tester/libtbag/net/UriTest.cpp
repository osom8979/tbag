/**
 * @file   UriTest.cpp
 * @brief  Uri class tester.
 * @author zer0
 * @date   2017-05-19
 * @date   2019-01-19 (Move: libtbag/network -> libtbag/net)
 */

#include <gtest/gtest.h>
#include <libtbag/net/Uri.hpp>
#include <libtbag/net/Ip.hpp>

using namespace libtbag;
using namespace libtbag::net;

TEST(UriTest, _is_triple_slash)
{
    int const ERR = _uri_state_result_error;
    int const NO = _uri_state_result_no_slash;

    ASSERT_EQ(ERR, _is_triple_slash2(nullptr));
    ASSERT_EQ(ERR, _is_triple_slash2(""));
    ASSERT_EQ( NO, _is_triple_slash2("test"));
    ASSERT_EQ( NO, _is_triple_slash2("test:"));
    ASSERT_EQ( NO, _is_triple_slash2("test://"));
    ASSERT_EQ(ERR, _is_triple_slash2("test_://"));
    ASSERT_EQ(ERR, _is_triple_slash2("test_:///"));
    ASSERT_EQ(  7, _is_triple_slash2("test:///"));
    ASSERT_EQ( NO, _is_triple_slash2("http://"));
    ASSERT_EQ( NO, _is_triple_slash2("http://hoho"));
    ASSERT_EQ(  7, _is_triple_slash2("http:///hoho"));
    ASSERT_EQ(  8, _is_triple_slash2("https:///hoho"));
    ASSERT_EQ(  8, _is_triple_slash2("https:/// "));
    ASSERT_EQ(ERR, _is_triple_slash2("https:// /"));
    ASSERT_EQ(  3, _is_triple_slash2(":///"));
    ASSERT_EQ( NO, _is_triple_slash2("://"));
    ASSERT_EQ(ERR, _is_triple_slash2(" ://"));
}

TEST(UriTest, EnableTripleSlash)
{
    Uri uri;
    bool const PARSE_RESULT = uri.parse("http:///hoho?q1=a1&q2=a2#fragment");
    ASSERT_TRUE(PARSE_RESULT);
    auto const QUERIES = uri.getQueryMap();
    ASSERT_STREQ("a1", QUERIES.at("q1").c_str());
    ASSERT_STREQ("a2", QUERIES.at("q2").c_str());
    ASSERT_STREQ("fragment", uri.getFragment().c_str());
    ASSERT_STREQ("http", uri.getSchema().c_str());
    ASSERT_STREQ("/hoho", uri.getPath().c_str());
    ASSERT_TRUE(uri.getHost().empty());
}

TEST(UriTest, Default)
{
    char const * const TEST_URI = "http://a:b@host.com:8080/p/a/t/h?query=string#hash";

    Uri::FieldInfo const RESULT_SCHEMA  ( 0,  4, true);
    Uri::FieldInfo const RESULT_HOST    (11,  8, true);
    Uri::FieldInfo const RESULT_PORT    (20,  4, true);
    Uri::FieldInfo const RESULT_PATH    (24,  8, true);
    Uri::FieldInfo const RESULT_QUERY   (33, 12, true);
    Uri::FieldInfo const RESULT_FRAGMENT(46,  4, true);
    Uri::FieldInfo const RESULT_USERINFO( 7,  3, true);

    Uri uri;
    ASSERT_TRUE(uri.parse(TEST_URI));
    ASSERT_EQ(RESULT_SCHEMA  , uri.getSchemaInfo  ());
    ASSERT_EQ(RESULT_HOST    , uri.getHostInfo    ());
    ASSERT_EQ(RESULT_PORT    , uri.getPortInfo    ());
    ASSERT_EQ(RESULT_PATH    , uri.getPathInfo    ());
    ASSERT_EQ(RESULT_QUERY   , uri.getQueryInfo   ());
    ASSERT_EQ(RESULT_FRAGMENT, uri.getFragmentInfo());
    ASSERT_EQ(RESULT_USERINFO, uri.getUserinfoInfo());

    ASSERT_TRUE(uri.isSchema  ());
    ASSERT_TRUE(uri.isHost    ());
    ASSERT_TRUE(uri.isPort    ());
    ASSERT_TRUE(uri.isPath    ());
    ASSERT_TRUE(uri.isQuery   ());
    ASSERT_TRUE(uri.isFragment());
    ASSERT_TRUE(uri.isUserinfo());

    ASSERT_STREQ("http"        , uri.getSchema  ().c_str());
    ASSERT_STREQ("host.com"    , uri.getHost    ().c_str());
    ASSERT_STREQ("8080"        , uri.getPort    ().c_str());
    ASSERT_STREQ("/p/a/t/h"    , uri.getPath    ().c_str());
    ASSERT_STREQ("query=string", uri.getQuery   ().c_str());
    ASSERT_STREQ("hash"        , uri.getFragment().c_str());
    ASSERT_STREQ("a:b"         , uri.getUserinfo().c_str());

    ASSERT_EQ(8080, uri.getPortNumber());
}

TEST(UriTest, AddrInfo)
{
    std::string host;
    int port;

    Uri uri1("http://localhost:8080/");
    ASSERT_EQ(E_SUCCESS, uri1.requestAddrInfo(host, port));

    ASSERT_TRUE(libtbag::net::isIpv4(host) || libtbag::net::isIpv6(host));
    ASSERT_EQ(8080, port);

    Uri uri2("http://localhost/");
    ASSERT_EQ(E_SUCCESS, uri2.requestAddrInfo(host, port));

    ASSERT_TRUE(libtbag::net::isIpv4(host) || libtbag::net::isIpv6(host));
    ASSERT_EQ(80, port);
}

TEST(UriTest, QueryMap)
{
    char const * const TEST_URI = "https://www.google.co.kr/search?q=SPS+PPS+IDR&oq=SPS+PPS+IDR+&chrome&&ie=UTF-8";
    Uri uri(TEST_URI);
    ASSERT_STREQ("https", uri.getSchema().c_str());
    ASSERT_STREQ("www.google.co.kr", uri.getHost().c_str());
    ASSERT_STREQ("/search", uri.getPath().c_str());
    ASSERT_TRUE(uri.getPort().empty());
    ASSERT_TRUE(uri.getUserinfo().empty());
    ASSERT_TRUE(uri.getFragment().empty());

    ASSERT_STREQ("q=SPS+PPS+IDR&oq=SPS+PPS+IDR+&chrome&&ie=UTF-8", uri.getQuery().c_str());
    auto const QUERY_MAP = uri.getQueryMap();
    ASSERT_EQ(4, QUERY_MAP.size());

    auto itr = QUERY_MAP.find("q");
    ASSERT_STREQ("q", itr->first.c_str());
    ASSERT_STREQ("SPS+PPS+IDR", itr->second.c_str());

    itr = QUERY_MAP.find("oq");
    ASSERT_STREQ("oq", itr->first.c_str());
    ASSERT_STREQ("SPS+PPS+IDR+", itr->second.c_str());

    itr = QUERY_MAP.find("chrome");
    ASSERT_STREQ("chrome", itr->first.c_str());
    ASSERT_TRUE(itr->second.empty());

    itr = QUERY_MAP.find("ie");
    ASSERT_STREQ("ie", itr->first.c_str());
    ASSERT_STREQ("UTF-8", itr->second.c_str());
}

TEST(UriTest, EncodeDecode)
{
    std::string const ENCODED = "https://temp:pa%5Css@translate.google.co.kr:8080/a%5Ca/?h%20l=ko&temp=A%5CB#en/ko/Test%5CMessage";
    std::string const DECODED = "https://temp:pa\\ss@translate.google.co.kr:8080/a\\a/?h l=ko&temp=A\\B#en/ko/Test\\Message";
    ASSERT_EQ(ENCODED, Uri::encodePercent(DECODED));
    ASSERT_EQ(DECODED, Uri::decodePercent(ENCODED));

    Uri uri;
    ASSERT_TRUE(uri.encodeParse(DECODED));
    ASSERT_STREQ("https", uri.getSchema().c_str());
    ASSERT_STREQ("temp:pa\\ss", uri.decodeUserinfo().c_str());
    ASSERT_STREQ("translate.google.co.kr", uri.getHost().c_str());
    ASSERT_STREQ("8080", uri.getPort().c_str());
    ASSERT_STREQ("h l=ko&temp=A\\B", uri.decodeQuery().c_str());
    ASSERT_STREQ("/a\\a/", uri.decodePath().c_str());
    ASSERT_STREQ("en/ko/Test\\Message", uri.decodeFragment().c_str());

    auto const QUERIES = uri.decodeQueryMap();
    ASSERT_STREQ("ko", QUERIES.at("h l").c_str());
    ASSERT_STREQ("A\\B", QUERIES.at("temp").c_str());
}

TEST(UriTest, Request)
{
    std::string address;
    int port;

    ASSERT_TRUE(requestAddrInfo("http://localhost", address, port));
    ASSERT_EQ(libtbag::net::LOOPBACK_IPV4, address);
    ASSERT_EQ(80, port);

    ASSERT_TRUE(requestAddrInfo("https://127.0.0.1/", address, port));
    ASSERT_EQ(libtbag::net::LOOPBACK_IPV4, address);
    ASSERT_EQ(443, port);

    ASSERT_TRUE(requestAddrInfo("ws://localhost", address, port));
    ASSERT_EQ(libtbag::net::LOOPBACK_IPV4, address);
    ASSERT_EQ(80, port);

    ASSERT_TRUE(requestAddrInfo("wss://127.0.0.1/", address, port));
    ASSERT_EQ(libtbag::net::LOOPBACK_IPV4, address);
    ASSERT_EQ(443, port);

    ASSERT_TRUE(requestAddrInfo("wss://127.0.0.1:9999/", address, port));
    ASSERT_EQ(libtbag::net::LOOPBACK_IPV4, address);
    ASSERT_EQ(9999, port);
}

TEST(UriTest, DISABLE_HTTP_PARSER_URL_FIELDS)
{
    __check_error__http_parser_url_fields();
}

