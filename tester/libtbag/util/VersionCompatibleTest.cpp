/**
 * @file   VersionCompatibleTest.cpp
 * @brief  VersionCompatible class tester.
 * @author zer0
 * @date   2020-04-28
 */

#include <gtest/gtest.h>
#include <libtbag/util/VersionCompatible.hpp>

using namespace libtbag;
using namespace libtbag::util;

TEST(VersionCompatibleTest, Equals)
{
    auto const TEST_ORIGIN = Version("5.6.1");

    ASSERT_EQ(E_SUCCESS, testCompatible(TEST_ORIGIN, "5"));
    ASSERT_EQ(E_SUCCESS, testCompatible(TEST_ORIGIN, "5.6"));
    ASSERT_EQ(E_SUCCESS, testCompatible(TEST_ORIGIN, "5.6.1"));

    ASSERT_EQ(E_SUCCESS, testCompatible(TEST_ORIGIN, "==5"));
    ASSERT_EQ(E_SUCCESS, testCompatible(TEST_ORIGIN, "==5.6"));
    ASSERT_EQ(E_SUCCESS, testCompatible(TEST_ORIGIN, "==5.6.1"));

    ASSERT_EQ(E_VERSION, testCompatible(TEST_ORIGIN, "6"));
    ASSERT_EQ(E_VERSION, testCompatible(TEST_ORIGIN, "5.7"));
    ASSERT_EQ(E_VERSION, testCompatible(TEST_ORIGIN, "5.6.2"));
    ASSERT_EQ(E_VERSION, testCompatible(TEST_ORIGIN, "4.6.1"));

    ASSERT_EQ(E_PARSING, testCompatible(TEST_ORIGIN, "=6"));
    ASSERT_EQ(E_PARSING, testCompatible(TEST_ORIGIN, "=5.7"));
    ASSERT_EQ(E_PARSING, testCompatible(TEST_ORIGIN, "=5.6.2"));
    ASSERT_EQ(E_PARSING, testCompatible(TEST_ORIGIN, "=4.6.1"));
}

TEST(VersionCompatibleTest, Single)
{
    auto const TEST_ORIGIN = Version("5.6.1");

    ASSERT_EQ(E_SUCCESS, testCompatible(TEST_ORIGIN, "foo>=5"));
    ASSERT_EQ(E_SUCCESS, testCompatible(TEST_ORIGIN, "foo>=5.6"));
    ASSERT_EQ(E_SUCCESS, testCompatible(TEST_ORIGIN, "foo>=5.6.1"));

    ASSERT_EQ(E_SUCCESS, testCompatible(TEST_ORIGIN, "foo>5"));
    ASSERT_EQ(E_SUCCESS, testCompatible(TEST_ORIGIN, "foo>5.6"));
    ASSERT_EQ(E_VERSION, testCompatible(TEST_ORIGIN, "foo>5.6.1"));

    ASSERT_EQ(E_SUCCESS, testCompatible(TEST_ORIGIN, "foo<=5"));
    ASSERT_EQ(E_SUCCESS, testCompatible(TEST_ORIGIN, "foo<=5.6"));
    ASSERT_EQ(E_SUCCESS, testCompatible(TEST_ORIGIN, "foo<=5.6.1"));

//    ASSERT_EQ(E_SUCCESS, testCompatible(TEST_ORIGIN, "foo<=5+cu101"));
//    ASSERT_EQ(E_SUCCESS, testCompatible(TEST_ORIGIN, "foo<=5.6+cu101"));
//    ASSERT_EQ(E_SUCCESS, testCompatible(TEST_ORIGIN, "foo<=5.6.1+cu101"));

    ASSERT_EQ(E_VERSION, testCompatible(TEST_ORIGIN, "foo<5"));
    ASSERT_EQ(E_VERSION, testCompatible(TEST_ORIGIN, "foo<5.6"));
    ASSERT_EQ(E_VERSION, testCompatible(TEST_ORIGIN, "foo<5.6.1"));

    ASSERT_EQ(E_SUCCESS, testCompatible(TEST_ORIGIN, "foo==5"));
    ASSERT_EQ(E_SUCCESS, testCompatible(TEST_ORIGIN, "foo==5.6"));
    ASSERT_EQ(E_SUCCESS, testCompatible(TEST_ORIGIN, "foo==5.6.1"));

    ASSERT_EQ(E_VERSION, testCompatible(TEST_ORIGIN, "foo!=5"));
    ASSERT_EQ(E_VERSION, testCompatible(TEST_ORIGIN, "foo!=5.6"));
    ASSERT_EQ(E_VERSION, testCompatible(TEST_ORIGIN, "foo!=5.6.1"));
}

TEST(VersionCompatibleTest, Multiple)
{
    auto const TEST_ORIGIN = Version("5.6.1");

    ASSERT_EQ(E_SUCCESS, testCompatible(TEST_ORIGIN, "foo>5, <5.7"));
    ASSERT_EQ(E_SUCCESS, testCompatible(TEST_ORIGIN, "foo>0, <5.7"));

    ASSERT_EQ(E_SUCCESS, testCompatible(TEST_ORIGIN, ">5 && <5.7"));
    ASSERT_EQ(E_SUCCESS, testCompatible(TEST_ORIGIN, ">0 & <5.7.1"));

    ASSERT_EQ(E_VERSION, testCompatible(TEST_ORIGIN, "foo==6 && >5"));
    ASSERT_EQ(E_SUCCESS, testCompatible(TEST_ORIGIN, "foo==6 || >5"));

    ASSERT_EQ(E_SUCCESS, testCompatible(TEST_ORIGIN, "foo>6 | ==5.6.1"));
    ASSERT_EQ(E_SUCCESS, testCompatible(TEST_ORIGIN, "foo>6 | ==5.6.0"));
}

