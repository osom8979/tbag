/**
 * @file   ArgumentsTest.cpp
 * @brief  Arguments class tester.
 * @author zer0
 * @date   2016-09-23
 */

#include <gtest/gtest.h>
#include <libtbag/string/Arguments.hpp>

using namespace libtbag;
using namespace libtbag::string;

TEST(ArgumentsTest, OnlyCoverage)
{
    Arguments args("name", "args");
    args.getName();
    args.getDelimiter();
    args.getPointDelimiter();

    args.setName("");
    args.setDelimiter("");
    args.setPointDelimiter("");

    args.clear();
    args.empty();
    args.size();

    args.parse("");
    args.optInteger(0, nullptr);
    args.optDouble(0, nullptr);
    args.optString(0, nullptr);
    args.optIntegerPoint(0, nullptr);
    args.optDoublePoint(0, nullptr);
    args.optIntegerRect(0, nullptr);
    args.optDoubleRect(0, nullptr);
}

struct ArgumentsFixture : public testing::Test
{
public:
    static constexpr char const * const ARGUMENT_STRING = "test,1,2.1,9x10,100.10x200.2x300x400,x";

public:
    Arguments args;

protected:
    virtual void SetUp() override
    {
        args.parse(ARGUMENT_STRING);
    }

    virtual void TearDown() override
    {
        args.clear();
    }

public:
    struct TestResult
    {
        std::string s1;

        int         v1;
        double      v2;

        Arguments::IntegerPoint p1;
        Arguments::DoublePoint  p2;

        Arguments::IntegerRect  r1;
        Arguments::DoubleRect   r2;
    };
};

TEST_F(ArgumentsFixture, Default)
{
    ASSERT_FALSE(args.empty());
    ASSERT_EQ(6U, args.size());
}

TEST_F(ArgumentsFixture, toString)
{
    ASSERT_STREQ(ARGUMENT_STRING, args.toString().c_str());
}

TEST_F(ArgumentsFixture, getVector)
{
    ASSERT_EQ(6U, args.getStrings().size());
    ASSERT_EQ(2U, args.getIntegers().size());
    ASSERT_EQ(2U, args.getDoubles().size());
    ASSERT_EQ(1U, args.getIntegerPoints().size());
    ASSERT_EQ(1U, args.getDoublePoints().size());
    ASSERT_EQ(1U, args.getIntegerRects().size());
    ASSERT_EQ(1U, args.getDoubleRects().size());
}

TEST_F(ArgumentsFixture, optString)
{
    int const TEST_INDEX = 0;
    TestResult result;

    ASSERT_TRUE( args.optString      (TEST_INDEX, &result.s1));
    ASSERT_FALSE(args.optInteger     (TEST_INDEX, &result.v1));
    ASSERT_FALSE(args.optDouble      (TEST_INDEX, &result.v2));
    ASSERT_FALSE(args.optIntegerPoint(TEST_INDEX, &result.p1));
    ASSERT_FALSE(args.optDoublePoint (TEST_INDEX, &result.p2));
    ASSERT_FALSE(args.optIntegerRect (TEST_INDEX, &result.r1));
    ASSERT_FALSE(args.optDoubleRect  (TEST_INDEX, &result.r2));
    ASSERT_STREQ("test", result.s1.c_str());
}

TEST_F(ArgumentsFixture, optInteger)
{
    int const TEST_INDEX = 1;
    TestResult result;

    ASSERT_TRUE( args.optString      (TEST_INDEX, &result.s1));
    ASSERT_TRUE( args.optInteger     (TEST_INDEX, &result.v1));
    ASSERT_TRUE( args.optDouble      (TEST_INDEX, &result.v2));
    ASSERT_FALSE(args.optIntegerPoint(TEST_INDEX, &result.p1));
    ASSERT_FALSE(args.optDoublePoint (TEST_INDEX, &result.p2));
    ASSERT_FALSE(args.optIntegerRect (TEST_INDEX, &result.r1));
    ASSERT_FALSE(args.optDoubleRect  (TEST_INDEX, &result.r2));

    ASSERT_EQ(1, result.v1);
    ASSERT_EQ(1, result.v2);
}

TEST_F(ArgumentsFixture, optDouble)
{
    int const TEST_INDEX = 2;
    TestResult result;

    ASSERT_TRUE( args.optString      (TEST_INDEX, &result.s1));
    ASSERT_TRUE( args.optInteger     (TEST_INDEX, &result.v1));
    ASSERT_TRUE( args.optDouble      (TEST_INDEX, &result.v2));
    ASSERT_FALSE(args.optIntegerPoint(TEST_INDEX, &result.p1));
    ASSERT_FALSE(args.optDoublePoint (TEST_INDEX, &result.p2));
    ASSERT_FALSE(args.optIntegerRect (TEST_INDEX, &result.r1));
    ASSERT_FALSE(args.optDoubleRect  (TEST_INDEX, &result.r2));

    ASSERT_EQ(2  , result.v1);
    ASSERT_EQ(2.1, result.v2);
}

TEST_F(ArgumentsFixture, optPoint)
{
    int const TEST_INDEX = 3;
    TestResult result;

    ASSERT_TRUE( args.optString      (TEST_INDEX, &result.s1));
    ASSERT_FALSE(args.optInteger     (TEST_INDEX, &result.v1));
    ASSERT_FALSE(args.optDouble      (TEST_INDEX, &result.v2));
    ASSERT_TRUE( args.optIntegerPoint(TEST_INDEX, &result.p1));
    ASSERT_TRUE( args.optDoublePoint (TEST_INDEX, &result.p2));
    ASSERT_FALSE(args.optIntegerRect (TEST_INDEX, &result.r1));
    ASSERT_FALSE(args.optDoubleRect  (TEST_INDEX, &result.r2));

    ASSERT_EQ(libtbag::geometry::makePoint<int   >(9  , 10  ), result.p1);
    ASSERT_EQ(libtbag::geometry::makePoint<double>(9.0, 10.0), result.p2);
}

TEST_F(ArgumentsFixture, optRect)
{
    int const TEST_INDEX = 4;
    TestResult result;

    ASSERT_TRUE( args.optString      (TEST_INDEX, &result.s1));
    ASSERT_FALSE(args.optInteger     (TEST_INDEX, &result.v1));
    ASSERT_FALSE(args.optDouble      (TEST_INDEX, &result.v2));
    ASSERT_FALSE(args.optIntegerPoint(TEST_INDEX, &result.p1));
    ASSERT_FALSE(args.optDoublePoint (TEST_INDEX, &result.p2));
    ASSERT_TRUE( args.optIntegerRect (TEST_INDEX, &result.r1));
    ASSERT_TRUE( args.optDoubleRect  (TEST_INDEX, &result.r2));

    ASSERT_EQ(libtbag::geometry::makeRect<int   >(100   , 200  , 300  , 400  ), result.r1);
    ASSERT_EQ(libtbag::geometry::makeRect<double>(100.10, 200.2, 300.0, 400.0), result.r2);
}

TEST_F(ArgumentsFixture, checkPointDelimiter)
{
    int const TEST_INDEX = 5;
    TestResult result;

    ASSERT_TRUE( args.optString      (TEST_INDEX, &result.s1));
    ASSERT_FALSE(args.optInteger     (TEST_INDEX, &result.v1));
    ASSERT_FALSE(args.optDouble      (TEST_INDEX, &result.v2));
    ASSERT_FALSE(args.optIntegerPoint(TEST_INDEX, &result.p1));
    ASSERT_FALSE(args.optDoublePoint (TEST_INDEX, &result.p2));
    ASSERT_FALSE(args.optIntegerRect(TEST_INDEX, &result.r1));
    ASSERT_FALSE(args.optDoubleRect (TEST_INDEX, &result.r2));
}

