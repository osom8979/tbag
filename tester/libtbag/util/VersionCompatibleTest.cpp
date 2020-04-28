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

TEST(VersionCompatibleTest, SplitStatement)
{
    Statement stat;
    stat = splitStatement("");
    ASSERT_EQ(logical_operator_done, stat.op);
    ASSERT_TRUE(stat.left.empty());

    stat = splitStatement(">5,<5.7");
    ASSERT_EQ(logical_operator_and, stat.op);
    ASSERT_STREQ(">5", stat.left.c_str());
    ASSERT_STREQ("<5.7", stat.right.c_str());

    stat = splitStatement(">5&&<5.7");
    ASSERT_EQ(logical_operator_and, stat.op);
    ASSERT_STREQ(">5", stat.left.c_str());
    ASSERT_STREQ("<5.7", stat.right.c_str());

    stat = splitStatement(">5&<5.7");
    ASSERT_EQ(logical_operator_and, stat.op);
    ASSERT_STREQ(">5", stat.left.c_str());
    ASSERT_STREQ("<5.7", stat.right.c_str());

    stat = splitStatement(">0 & <5.7.1");
    ASSERT_EQ(logical_operator_and, stat.op);
    ASSERT_STREQ(">0 ", stat.left.c_str());
    ASSERT_STREQ(" <5.7.1", stat.right.c_str());

    stat = splitStatement(">0 &| <5.7.1");
    ASSERT_EQ(logical_operator_error, stat.op);

    stat = splitStatement(">0 |& <5.7.1");
    ASSERT_EQ(logical_operator_error, stat.op);

    stat = splitStatement("foo==6 | >5");
    ASSERT_EQ(logical_operator_or, stat.op);
    ASSERT_STREQ("foo==6 ", stat.left.c_str());
    ASSERT_STREQ(" >5", stat.right.c_str());

    stat = splitStatement("foo==6 || >5");
    ASSERT_EQ(logical_operator_or, stat.op);
    ASSERT_STREQ("foo==6 ", stat.left.c_str());
    ASSERT_STREQ(" >5", stat.right.c_str());

    stat = splitStatement("foo==6 ||");
    ASSERT_EQ(logical_operator_done, stat.op);
    ASSERT_STREQ("foo==6 ", stat.left.c_str());

    stat = splitStatement("foo==6 &");
    ASSERT_EQ(logical_operator_done, stat.op);
    ASSERT_STREQ("foo==6 ", stat.left.c_str());

    stat = splitStatement("foo==6 ,");
    ASSERT_EQ(logical_operator_done, stat.op);
    ASSERT_STREQ("foo==6 ", stat.left.c_str());

    stat = splitStatement("|| foo==6 &&");
    ASSERT_EQ(logical_operator_or, stat.op);
    ASSERT_TRUE(stat.left.empty());
    ASSERT_STREQ(" foo==6 &&", stat.right.c_str());

    stat = splitStatement("&& =6 &&");
    ASSERT_EQ(logical_operator_and, stat.op);
    ASSERT_TRUE(stat.left.empty());
    ASSERT_STREQ(" =6 &&", stat.right.c_str());

    stat = splitStatement("& 77 &&");
    ASSERT_EQ(logical_operator_and, stat.op);
    ASSERT_TRUE(stat.left.empty());
    ASSERT_STREQ(" 77 &&", stat.right.c_str());

    stat = splitStatement(", 9 &");
    ASSERT_EQ(logical_operator_and, stat.op);
    ASSERT_TRUE(stat.left.empty());
    ASSERT_STREQ(" 9 &", stat.right.c_str());
}

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

    ASSERT_EQ(E_VERSION, testCompatible(TEST_ORIGIN, "=6"));
    ASSERT_EQ(E_VERSION, testCompatible(TEST_ORIGIN, "=5.7"));
    ASSERT_EQ(E_VERSION, testCompatible(TEST_ORIGIN, "=5.6.2"));
    ASSERT_EQ(E_VERSION, testCompatible(TEST_ORIGIN, "=4.6.1"));
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

    ASSERT_EQ(E_VERSION, testCompatible(TEST_ORIGIN, "foo<5"));
    ASSERT_EQ(E_VERSION, testCompatible(TEST_ORIGIN, "foo<5.6"));
    ASSERT_EQ(E_VERSION, testCompatible(TEST_ORIGIN, "foo<5.6.1"));
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
    ASSERT_EQ(E_VERSION, testCompatible(TEST_ORIGIN, "foo>6 | ==5.6.0"));
}

