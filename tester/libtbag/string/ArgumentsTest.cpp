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
    inline static TBAG_CONSTEXPR char const * const getArgumentString()
    { return "test,1,2.1,9x10,100.10x200.2x300x400,x"; }

public:
    Arguments args;

protected:
    virtual void SetUp() override
    {
        args.parse(getArgumentString());
    }

    virtual void TearDown() override
    {
        args.clear();
    }

public:
    struct TestResult
    {
        std::string s1;

        int    v1;
        double v2;

        Arguments::Pointi p1;
        Arguments::Pointd p2;
        Arguments::Recti  r1;
        Arguments::Rectd  r2;
    };
};

TEST_F(ArgumentsFixture, Default)
{
    ASSERT_FALSE(args.empty());
    ASSERT_EQ(6U, args.size());
}

TEST_F(ArgumentsFixture, ToString)
{
    ASSERT_STREQ(getArgumentString(), args.toString().c_str());
}

TEST_F(ArgumentsFixture, ModifyMethods)
{
    ASSERT_EQ("test", args.get(0));
    args.pop();
    ASSERT_EQ("1", args.get(0));
    args.insert(0, "test");
    ASSERT_EQ("test", args.get(0));
}

TEST_F(ArgumentsFixture, GetVector)
{
    ASSERT_EQ(6U, args.getStrings().size());
    ASSERT_EQ(2U, args.getIntegers().size());
    ASSERT_EQ(2U, args.getDoubles().size());
    ASSERT_EQ(1U, args.getIntegerPoints().size());
    ASSERT_EQ(1U, args.getDoublePoints().size());
    ASSERT_EQ(1U, args.getIntegerRects().size());
    ASSERT_EQ(1U, args.getDoubleRects().size());
}

TEST_F(ArgumentsFixture, OptString)
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

TEST_F(ArgumentsFixture, OptInteger)
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

TEST_F(ArgumentsFixture, OptDouble)
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

TEST_F(ArgumentsFixture, OptPoint)
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

    ASSERT_EQ(libtbag::geometry::BasePoint2<int   >(9  , 10  ), result.p1);
    ASSERT_EQ(libtbag::geometry::BasePoint2<double>(9.0, 10.0), result.p2);
}

TEST_F(ArgumentsFixture, OptRect)
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

    ASSERT_EQ(libtbag::geometry::BaseRect2<int   >(100   , 200  , 300  , 400  ), result.r1);
    ASSERT_EQ(libtbag::geometry::BaseRect2<double>(100.10, 200.2, 300.0, 400.0), result.r2);
}

TEST_F(ArgumentsFixture, CheckPointDelimiter)
{
    int const TEST_INDEX = 5;
    TestResult result;

    ASSERT_TRUE( args.optString      (TEST_INDEX, &result.s1));
    ASSERT_FALSE(args.optInteger     (TEST_INDEX, &result.v1));
    ASSERT_FALSE(args.optDouble      (TEST_INDEX, &result.v2));
    ASSERT_FALSE(args.optIntegerPoint(TEST_INDEX, &result.p1));
    ASSERT_FALSE(args.optDoublePoint (TEST_INDEX, &result.p2));
    ASSERT_FALSE(args.optIntegerRect (TEST_INDEX, &result.r1));
    ASSERT_FALSE(args.optDoubleRect  (TEST_INDEX, &result.r2));
}

