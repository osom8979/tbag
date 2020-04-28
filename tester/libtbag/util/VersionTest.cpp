/**
 * @file   VersionTest.cpp
 * @brief  Version class tester.
 * @author zer0
 * @date   2017-05-25
 */

#include <gtest/gtest.h>
#include <libtbag/util/Version.hpp>

using namespace libtbag;
using namespace libtbag::util;

TEST(VersionTest, Compare)
{
    Version lh;
    Version rh;

    lh.set(1, 2, 3);
    rh.set(1, 2, 3);

    ASSERT_TRUE (lh == rh);
    ASSERT_FALSE(lh != rh);
    ASSERT_TRUE (lh >= rh);
    ASSERT_TRUE (lh <= rh);
    ASSERT_FALSE(lh >  rh);
    ASSERT_FALSE(lh <  rh);

    lh.set(1, 2, 4);
    rh.set(1, 2, 3);
    ASSERT_FALSE(lh == rh);
    ASSERT_TRUE (lh != rh);
    ASSERT_TRUE (lh >= rh);
    ASSERT_FALSE(lh <= rh);
    ASSERT_TRUE (lh >  rh);
    ASSERT_FALSE(lh <  rh);

    lh.set(1, 1, 4);
    rh.set(1, 2, 3);
    ASSERT_FALSE(lh == rh);
    ASSERT_TRUE (lh != rh);
    ASSERT_FALSE(lh >= rh);
    ASSERT_TRUE (lh <= rh);
    ASSERT_FALSE(lh >  rh);
    ASSERT_TRUE (lh <  rh);

    lh.set(4, 1, 4);
    rh.set(1, 2, 3);
    ASSERT_FALSE(lh == rh);
    ASSERT_TRUE (lh != rh);
    ASSERT_TRUE (lh >= rh);
    ASSERT_FALSE(lh <= rh);
    ASSERT_TRUE (lh >  rh);
    ASSERT_FALSE(lh <  rh);

    ASSERT_EQ(Version(4,3,1), Version(4));
    ASSERT_EQ(Version(4,3,1), Version(4,3));
    ASSERT_EQ(Version(4,3,1), Version(4,3,1));
}

TEST(VersionTest, String1)
{
    Version ver;
    ASSERT_EQ(E_SUCCESS, ver.fromString("1.2.3.4.9999.a"));
    ASSERT_EQ(Version(1, 2, 3), ver);
    ASSERT_STREQ("1.2.3", ver.toString().c_str());
    ASSERT_STREQ("4.9999.a", ver.getTweak().c_str());
}

TEST(VersionTest, String2)
{
    Version ver;
    ASSERT_EQ(E_SUCCESS, ver.fromString("1."));
    ASSERT_EQ(ver, Version(1));
    ASSERT_EQ(ver, Version(1, 0));
    ASSERT_STREQ("1", ver.toString().c_str());
}

TEST(VersionTest, String3)
{
    Version ver;
    ASSERT_EQ(E_SUCCESS, ver.fromString("0.77"));
    ASSERT_EQ(ver, Version(0));
    ASSERT_EQ(ver, Version(0, 77));
    ASSERT_NE(ver, Version(0, 77, 1));
    ASSERT_STREQ("0.77", ver.toString().c_str());
}

TEST(VersionTest, StringConstructor)
{
    Version ver("100.2.50");
    ASSERT_EQ(100, ver.getMajor());
    ASSERT_EQ(  2, ver.getMinor());
    ASSERT_EQ( 50, ver.getPatch());
}

TEST(VersionTest, Numeric)
{
    Version ver;
    ASSERT_EQ(E_SUCCESS, ver.fromNumeric(20005, 10000, 100, 1));
    ASSERT_EQ(ver.getMajor(), 2);
    ASSERT_EQ(ver.getMinor(), 0);
    ASSERT_EQ(ver.getPatch(), 5);
}

TEST(VersionTest, TbagVersion)
{
    auto ver = getTbagVersion();
    ASSERT_EQ(ver.getMajor(), LIBTBAG_VERSION_MAJOR);
    ASSERT_EQ(ver.getMinor(), LIBTBAG_VERSION_MINOR);
    ASSERT_EQ(ver.getPatch(), LIBTBAG_VERSION_PATCH);
}

TEST(VersionTest, GetFullVersionString)
{
    auto const text = getFullVersionString();
    ASSERT_FALSE(text.empty());
    std::cout << "Full version string: " << text << std::endl;
}

