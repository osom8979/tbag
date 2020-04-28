/**
 * @file   ComparisonOperatorTest.cpp
 * @brief  ComparisonOperator class tester.
 * @author zer0
 * @date   2020-04-28
 */

#include <gtest/gtest.h>
#include <libtbag/lexer/ComparisonOperator.hpp>

using namespace libtbag;
using namespace libtbag::lexer;

TEST(ComparisonOperatorTest, SplitComparison)
{
    Comparison comp;
    comp = splitComparison("");
    ASSERT_EQ(comparison_operator_error, comp.op);

    comp = splitComparison(" ");
    ASSERT_EQ(comparison_operator_error, comp.op);

    comp = splitComparison(" == ");
    ASSERT_EQ(comparison_operator_equal, comp.op);
    ASSERT_TRUE(comp.left.empty());
    ASSERT_TRUE(comp.right.empty());

    comp = splitComparison("1<2");
    ASSERT_EQ(comparison_operator_less_than, comp.op);
    ASSERT_STREQ("1", comp.left.c_str());
    ASSERT_STREQ("2", comp.right.c_str());

    comp = splitComparison("1>2");
    ASSERT_EQ(comparison_operator_greater_than, comp.op);
    ASSERT_STREQ("1", comp.left.c_str());
    ASSERT_STREQ("2", comp.right.c_str());

    comp = splitComparison(" 1 <= 2 ");
    ASSERT_EQ(comparison_operator_less_equal, comp.op);
    ASSERT_STREQ("1", comp.left.c_str());
    ASSERT_STREQ("2", comp.right.c_str());

    comp = splitComparison(" 1 >= 2 ");
    ASSERT_EQ(comparison_operator_greater_equal, comp.op);
    ASSERT_STREQ("1", comp.left.c_str());
    ASSERT_STREQ("2", comp.right.c_str());

    comp = splitComparison(" 1 == 2 ");
    ASSERT_EQ(comparison_operator_equal, comp.op);
    ASSERT_STREQ("1", comp.left.c_str());
    ASSERT_STREQ("2", comp.right.c_str());

    comp = splitComparison(" 1 != 2 ");
    ASSERT_EQ(comparison_operator_not_equal, comp.op);
    ASSERT_STREQ("1", comp.left.c_str());
    ASSERT_STREQ("2", comp.right.c_str());
}

TEST(ComparisonOperatorTest, SplitComparison_EmptyRight)
{
    Comparison comp;
    comp = splitComparison(" 1 >= ");
    ASSERT_EQ(comparison_operator_greater_equal, comp.op);
    ASSERT_STREQ("1", comp.left.c_str());
    ASSERT_TRUE(comp.right.empty());

    comp = splitComparison(" 1 > ");
    ASSERT_EQ(comparison_operator_greater_than, comp.op);
    ASSERT_STREQ("1", comp.left.c_str());
    ASSERT_TRUE(comp.right.empty());

    comp = splitComparison(" 1 <= ");
    ASSERT_EQ(comparison_operator_less_equal, comp.op);
    ASSERT_STREQ("1", comp.left.c_str());
    ASSERT_TRUE(comp.right.empty());

    comp = splitComparison(" 1 < ");
    ASSERT_EQ(comparison_operator_less_than, comp.op);
    ASSERT_STREQ("1", comp.left.c_str());
    ASSERT_TRUE(comp.right.empty());
}

TEST(ComparisonOperatorTest, SplitComparison_EmptyLeft)
{
    Comparison comp;
    comp = splitComparison(" >= 1");
    ASSERT_EQ(comparison_operator_greater_equal, comp.op);
    ASSERT_TRUE(comp.left.empty());
    ASSERT_STREQ("1", comp.right.c_str());

    comp = splitComparison(" > 1");
    ASSERT_EQ(comparison_operator_greater_than, comp.op);
    ASSERT_TRUE(comp.left.empty());
    ASSERT_STREQ("1", comp.right.c_str());

    comp = splitComparison(" <= 1");
    ASSERT_EQ(comparison_operator_less_equal, comp.op);
    ASSERT_TRUE(comp.left.empty());
    ASSERT_STREQ("1", comp.right.c_str());

    comp = splitComparison(" < 1");
    ASSERT_EQ(comparison_operator_less_than, comp.op);
    ASSERT_TRUE(comp.left.empty());
    ASSERT_STREQ("1", comp.right.c_str());
}

TEST(ComparisonOperatorTest, SplitComparison_Single)
{
    Comparison comp;
    comp = splitComparison(" =! 1");
    ASSERT_EQ(comparison_operator_last, comp.op);
    ASSERT_STREQ("=! 1", comp.left.c_str());

    comp = splitComparison(" !! 1");
    ASSERT_EQ(comparison_operator_last, comp.op);
    ASSERT_STREQ("!! 1", comp.left.c_str());

    comp = splitComparison(" 123 ");
    ASSERT_EQ(comparison_operator_last, comp.op);
    ASSERT_STREQ("123", comp.left.c_str());
}

