/**
 * @file   HttpPropertyTest.cpp
 * @brief  HttpProperty class tester.
 * @author zer0
 * @date   2017-11-24
 */

#include <gtest/gtest.h>
#include <libtbag/network/http/base/HttpProperty.hpp>

using namespace libtbag;
using namespace libtbag::network;
using namespace libtbag::network::http;
using namespace libtbag::network::http::base;

TEST(HttpPropertyTest, HttpVersion)
{
    HttpVersion ver0;
    ASSERT_EQ(DEFAULT_HTTP_VERSION_MAJOR, ver0.getMajor());
    ASSERT_EQ(DEFAULT_HTTP_VERSION_MINOR, ver0.getMinor());

    HttpVersion ver1(5, 6);
    ASSERT_NE(ver0, ver1);
    ASSERT_EQ(5, ver1.getMajor());
    ASSERT_EQ(6, ver1.getMinor());

    HttpVersion ver2 = ver1;
    ASSERT_EQ(ver1, ver2);

    HttpVersion ver3(0, 0);
    ASSERT_EQ(0, ver3.getMajor());
    ASSERT_EQ(0, ver3.getMinor());

    HttpVersion ver4 = std::move(ver3);
    ASSERT_EQ(DEFAULT_HTTP_VERSION_MAJOR, ver3.getMajor());
    ASSERT_EQ(DEFAULT_HTTP_VERSION_MINOR, ver3.getMinor());
    ASSERT_EQ(0, ver4.getMajor());
    ASSERT_EQ(0, ver4.getMinor());

    HttpVersion ver5;
    ver5.setMajor(1);
    ver5.setMinor(1);
    ASSERT_STREQ("HTTP/1.1", ver5.toVersionString().c_str());

    HttpVersion ver6;
    ver6 = ver5;
    ASSERT_EQ(ver5, ver6);

    HttpVersion ver7(3, 4);
    HttpVersion ver8(0, 0);
    ver8 = std::move(ver7);
    ASSERT_EQ(0, ver7.getMajor());
    ASSERT_EQ(0, ver7.getMinor());
    ASSERT_EQ(3, ver8.getMajor());
    ASSERT_EQ(4, ver8.getMinor());
}

TEST(HttpPropertyTest, HttpCommonProperty)
{
    HttpCommonProperty common1;
    common1.setBody("COM1");
    common1.setVersion(1, 2);
    common1.insert("A", "B");
    common1.insert("C", "D");

    HttpCommonProperty common2;
    common2.setBody("COM2");
    common2.setVersion(3, 4);
    common2.insert("E", "F");

    HttpCommonProperty common3 = common2;
    ASSERT_EQ(1, common3.sizeOfHeaders());
    ASSERT_STREQ("COM2", common3.getBodyString().c_str());
    ASSERT_EQ(HttpVersion(3, 4), common3);

    HttpCommonProperty common4;
    common4 = common1;
    ASSERT_EQ(2, common1.sizeOfHeaders());
    ASSERT_EQ(2, common4.sizeOfHeaders());
    ASSERT_EQ(HttpVersion(common1), HttpVersion(common4));

    HttpCommonProperty common5;
    common5.setBody("COM5");
    common5.setVersion(5, 6);
    common5.insert("G", "H");

    common5 = std::move(common4);
    ASSERT_EQ(1, common4.sizeOfHeaders());
    ASSERT_EQ(2, common5.sizeOfHeaders());
    ASSERT_STREQ("COM5", common4.getBodyString().c_str());
    ASSERT_STREQ("COM1", common5.getBodyString().c_str());
    ASSERT_STREQ("H", common4["G"].c_str());
    ASSERT_STREQ("B", common5["A"].c_str());
    ASSERT_STREQ("D", common5["C"].c_str());
}

TEST(HttpPropertyTest, HttpRequest)
{
    HttpRequest req1;
    req1.setBody("REQ1");
    req1.setVersion(1, 2);
    req1.insert("A", "B");
    req1.insert("C", "D");
    req1.method = "GET";
    req1.path = "/";

    HttpRequest req2;
    req2.setBody("REQ2");
    req2.setVersion(3, 4);
    req2.insert("E", "F");
    req2.method = "POST";
    req2.path = "/temp/";

    req2 = std::move(req1);
    ASSERT_NE(HttpVersion(req2), HttpVersion(req1));
    ASSERT_EQ(HttpVersion(3, 4), HttpVersion(req1));
    ASSERT_EQ(HttpVersion(1, 2), HttpVersion(req2));
    ASSERT_EQ(1, req1.sizeOfHeaders());
    ASSERT_EQ(2, req2.sizeOfHeaders());
    ASSERT_STREQ("REQ2", req1.getBodyString().c_str());
    ASSERT_STREQ("REQ1", req2.getBodyString().c_str());
    ASSERT_STREQ("F", req1["E"].c_str());
    ASSERT_STREQ("B", req2["A"].c_str());
    ASSERT_STREQ("D", req2["C"].c_str());
    ASSERT_STREQ("POST", req1.method.c_str());
    ASSERT_STREQ("GET", req2.method.c_str());
    ASSERT_STREQ("/temp/", req1.path.c_str());
    ASSERT_STREQ("/", req2.path.c_str());

    HttpRequest req3 = req2;
    ASSERT_EQ(HttpVersion(req3), req2);
    ASSERT_EQ(HttpVersion(1, 2), HttpVersion(req3));
    ASSERT_EQ(2, req3.sizeOfHeaders());
    ASSERT_STREQ("REQ1", req3.getBodyString().c_str());
    ASSERT_STREQ("B", req3["A"].c_str());
    ASSERT_STREQ("D", req3["C"].c_str());
    ASSERT_STREQ("GET", req3.method.c_str());
    ASSERT_STREQ("/", req3.path.c_str());
}

TEST(HttpPropertyTest, HttpResponse)
{
    HttpResponse res1;
    res1.setBody("RES1");
    res1.setVersion(1, 2);
    res1.insert("A", "B");
    res1.insert("C", "D");
    res1.code = 100;
    res1.reason = "CONTINUE";

    HttpResponse res2;
    res2.setBody("RES2");
    res2.setVersion(3, 4);
    res2.insert("E", "F");
    res2.code = 200;
    res2.reason = "OK";

    res2 = std::move(res1);
    ASSERT_NE(HttpVersion(res2), HttpVersion(res1));
    ASSERT_EQ(HttpVersion(3, 4), HttpVersion(res1));
    ASSERT_EQ(HttpVersion(1, 2), HttpVersion(res2));
    ASSERT_EQ(1, res1.sizeOfHeaders());
    ASSERT_EQ(2, res2.sizeOfHeaders());
    ASSERT_STREQ("RES2", res1.getBodyString().c_str());
    ASSERT_STREQ("RES1", res2.getBodyString().c_str());
    ASSERT_STREQ("F", res1["E"].c_str());
    ASSERT_STREQ("B", res2["A"].c_str());
    ASSERT_STREQ("D", res2["C"].c_str());
    ASSERT_EQ(200, res1.code);
    ASSERT_EQ(100, res2.code);
    ASSERT_STREQ("OK", res1.reason.c_str());
    ASSERT_STREQ("CONTINUE", res2.reason.c_str());

    HttpResponse res3 = res2;
    ASSERT_EQ(HttpVersion(res3), res2);
    ASSERT_EQ(HttpVersion(1, 2), HttpVersion(res3));
    ASSERT_EQ(2, res3.sizeOfHeaders());
    ASSERT_STREQ("RES1", res3.getBodyString().c_str());
    ASSERT_STREQ("B", res3["A"].c_str());
    ASSERT_STREQ("D", res3["C"].c_str());
    ASSERT_EQ(100, res3.code);
    ASSERT_STREQ("CONTINUE", res3.reason.c_str());
}

TEST(HttpPropertyTest, HttpProperty)
{
    HttpProperty pro1;
    pro1.setBody("RES1");
    pro1.setVersion(1, 2);
    pro1.insert("A", "B");
    pro1.insert("C", "D");
    pro1.code = 100;
    pro1.reason = "CONTINUE";
    pro1.method = "GET";
    pro1.path = "/";

    HttpProperty pro2;
    pro2.setBody("RES2");
    pro2.setVersion(3, 4);
    pro2.insert("E", "F");
    pro2.code = 200;
    pro2.reason = "OK";
    pro2.method = "POST";
    pro2.path = "/temp/";

    pro2 = std::move(pro1);
    ASSERT_NE(HttpVersion(pro2), HttpVersion(pro1));
    ASSERT_EQ(HttpVersion(3, 4), HttpVersion(pro1));
    ASSERT_EQ(HttpVersion(1, 2), HttpVersion(pro2));
    ASSERT_EQ(1, pro1.sizeOfHeaders());
    ASSERT_EQ(2, pro2.sizeOfHeaders());
    ASSERT_STREQ("RES2", pro1.getBodyString().c_str());
    ASSERT_STREQ("RES1", pro2.getBodyString().c_str());
    ASSERT_STREQ("F", pro1["E"].c_str());
    ASSERT_STREQ("B", pro2["A"].c_str());
    ASSERT_STREQ("D", pro2["C"].c_str());
    ASSERT_EQ(200, pro1.code);
    ASSERT_EQ(100, pro2.code);
    ASSERT_STREQ("OK", pro1.reason.c_str());
    ASSERT_STREQ("CONTINUE", pro2.reason.c_str());
    ASSERT_STREQ("POST", pro1.method.c_str());
    ASSERT_STREQ("GET", pro2.method.c_str());
    ASSERT_STREQ("/temp/", pro1.path.c_str());
    ASSERT_STREQ("/", pro2.path.c_str());

    HttpProperty pro3 = pro2;
    ASSERT_EQ(HttpVersion(pro3), pro2);
    ASSERT_EQ(HttpVersion(1, 2), HttpVersion(pro3));
    ASSERT_EQ(2, pro3.sizeOfHeaders());
    ASSERT_STREQ("RES1", pro3.getBodyString().c_str());
    ASSERT_STREQ("B", pro3["A"].c_str());
    ASSERT_STREQ("D", pro3["C"].c_str());
    ASSERT_EQ(100, pro3.code);
    ASSERT_STREQ("CONTINUE", pro3.reason.c_str());
    ASSERT_STREQ("GET", pro3.method.c_str());
    ASSERT_STREQ("/", pro3.path.c_str());

    HttpProperty pro4;
    pro4.setBody("RES4");
    pro4.setVersion(1, 2);
    pro4.insert("G", "H");
    pro4.insert("I", "J");
    pro4.code = 400;
    pro4.reason = "BAD";
    pro4.method = "PUT";
    pro4.path = "/down";

    HttpVersion        ver4 = pro4;
    HttpHeader         hdr4 = pro4;
    HttpCommonProperty com4 = pro4;
    HttpRequest        req4 = pro4;
    HttpResponse       res4 = pro4;

    ASSERT_EQ(HttpVersion(1, 2), ver4);
    ASSERT_EQ(2, hdr4.sizeOfHeaders());
    ASSERT_STREQ("H", hdr4["G"].c_str());
    ASSERT_STREQ("J", hdr4["I"].c_str());
    ASSERT_STREQ("RES4", com4.getBodyString().c_str());
    ASSERT_STREQ("PUT", req4.method.c_str());
    ASSERT_STREQ("/down", req4.path.c_str());
    ASSERT_EQ(400, res4.code);
    ASSERT_STREQ("BAD", res4.reason.c_str());

    pro4.clear();
    ASSERT_EQ(HttpVersion(0, 0), pro4);
    ASSERT_TRUE(pro4.emptyOfHeaders());
    ASSERT_TRUE(pro4.getBodyString().empty());
    ASSERT_TRUE(pro4.method.empty());
    ASSERT_TRUE(pro4.path.empty());
    ASSERT_EQ(0, pro4.code);
    ASSERT_TRUE(pro4.reason.empty());
}

