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

TEST(LogicalOperatorTest, SplitLogical)
{
    Logical logical;
    logical = splitLogical("");
    ASSERT_EQ(logical_operator_error, logical.op);

    logical = splitLogical(" ");
    ASSERT_EQ(logical_operator_error, logical.op);

    logical = splitLogical(" ss,ss ");
    ASSERT_EQ(logical_operator_last, logical.op);
    ASSERT_STREQ("ss,ss", logical.left.c_str());

    logical = splitLogical(" aa && bb ");
    ASSERT_EQ(logical_operator_and, logical.op);
    ASSERT_STREQ("aa", logical.left.c_str());
    ASSERT_STREQ("bb", logical.right.c_str());

    logical = splitLogical(" aa || bb ");
    ASSERT_EQ(logical_operator_or, logical.op);
    ASSERT_STREQ("aa", logical.left.c_str());
    ASSERT_STREQ("bb", logical.right.c_str());

    logical = splitLogical(" & aa & bb & ");
    ASSERT_EQ(logical_operator_last, logical.op);
    ASSERT_STREQ("& aa & bb &", logical.left.c_str());

    logical = splitLogical("|aa|bb|");
    ASSERT_EQ(logical_operator_last, logical.op);
    ASSERT_STREQ("|aa|bb|", logical.left.c_str());

    logical = splitLogical(" && aa && bb && ");
    ASSERT_EQ(logical_operator_and, logical.op);
    ASSERT_TRUE(logical.left.empty());
    ASSERT_STREQ("aa && bb &&", logical.right.c_str());

    logical = splitLogical("||aa||bb||");
    ASSERT_EQ(logical_operator_or, logical.op);
    ASSERT_TRUE(logical.left.empty());
    ASSERT_STREQ("aa||bb||", logical.right.c_str());

    logical = splitLogical(" aa & bb && ");
    ASSERT_EQ(logical_operator_and, logical.op);
    ASSERT_STREQ("aa & bb", logical.left.c_str());
    ASSERT_TRUE(logical.right.empty());

    logical = splitLogical("aa|bb||");
    ASSERT_EQ(logical_operator_or, logical.op);
    ASSERT_STREQ("aa|bb", logical.left.c_str());
    ASSERT_TRUE(logical.right.empty());
}

