/**
 * @file   UriTest.cpp
 * @brief  Uri class tester.
 * @author zer0
 * @date   2017-05-19
 */

#include <gtest/gtest.h>
#include <libtbag/network/Uri.hpp>
#include <libtbag/network/details/NetCommon.hpp>

using namespace libtbag;
using namespace libtbag::network;

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
    ASSERT_EQ(Err::E_SUCCESS, uri1.requestAddrInfo(host, port));

    ASSERT_TRUE(details::isIpv4(host) || details::isIpv6(host));
    ASSERT_EQ(8080, port);

    Uri uri2("http://localhost/");
    ASSERT_EQ(Err::E_SUCCESS, uri2.requestAddrInfo(host, port));

    ASSERT_TRUE(details::isIpv4(host) || details::isIpv6(host));
    ASSERT_EQ(80, port);
}

