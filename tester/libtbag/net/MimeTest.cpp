/**
 * @file   MimeTest.cpp
 * @brief  Mime class tester.
 * @author zer0
 * @date   2020-05-11
 */

#include <gtest/gtest.h>
#include <libtbag/net/Mime.hpp>

using namespace libtbag;
using namespace libtbag::net;

TEST(MimeTest, ToString)
{
    auto const obj = Mime("image", "png");
    ASSERT_STREQ("image/png", obj.toString().c_str());
}

TEST(MimeTest, FromString)
{
    auto const obj = Mime("text/plain;charset=UTF-8");
    ASSERT_STREQ("text", obj.type.c_str());
    ASSERT_STREQ("plain", obj.subtype.c_str());
    ASSERT_STREQ("charset=UTF-8", obj.parameter.c_str());
}

TEST(MimeTest, IsCompatible)
{
    ASSERT_EQ(Mime("text/plain;charset=UTF-8"), Mime("text/*"));
    ASSERT_EQ(Mime("*"), Mime("text/plain"));
    ASSERT_EQ(Mime("text/*"), Mime("text/plain"));
    ASSERT_NE(Mime("text/html"), Mime("text/plain"));
}

