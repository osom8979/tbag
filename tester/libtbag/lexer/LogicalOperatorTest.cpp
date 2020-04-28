/**
 * @file   LogicalOperatorTest.cpp
 * @brief  LogicalOperator class tester.
 * @author zer0
 * @date   2020-04-28
 */

#include <gtest/gtest.h>
#include <libtbag/lexer/LogicalOperator.hpp>

using namespace libtbag;
using namespace libtbag::lexer;

TEST(LogicalOperatorTest, SplitStatement)
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

