/**
 * @file   ParameterExpansionTest.cpp
 * @brief  ParameterExpansion class tester.
 * @author zer0
 * @date   2019-10-10
 */

#include <gtest/gtest.h>
#include <libtbag/string/ParameterExpansion.hpp>

using namespace libtbag;
using namespace libtbag::string;

struct ParameterExpansionFeatureTest : public testing::Test
{
    using Flags = libtbag::string::Flags;

    TBAG_CONSTEXPR static char const * const TEST_KEY = "string";
    TBAG_CONSTEXPR static char const * const TEST_VAL = "abc-efg-123-abc";

    Flags flags;

    virtual void SetUp() override
    {
        flags.push(TEST_KEY, TEST_VAL);
    }

    virtual void TearDown() override
    {
        // EMPTY.
    }

    std::string parameterExpansion(std::string const & variable) const
    {
        std::string result;
        auto const code = libtbag::string::parameterExpansion(stripParameterDecoration(variable), flags, result);
        if (isFailure(code)) {
            return {};
        }
        return result;
    }
};

TEST_F(ParameterExpansionFeatureTest, Default)
{
    EXPECT_EQ(std::string(TEST_VAL), parameterExpansion("${string}"));
    EXPECT_EQ(std::string(TEST_VAL), parameterExpansion("${string:-AAA}"));

    EXPECT_STREQ("", parameterExpansion("${test}").c_str());
    EXPECT_STREQ("AAA", parameterExpansion("${test:-AAA}").c_str());
}

TEST(ParameterExpansionTest, StripParameterDecoration)
{
    EXPECT_STREQ("tt", stripParameterDecoration(" ${tt} ").c_str());
    EXPECT_STREQ("test", stripParameterDecoration("${test}").c_str());
    EXPECT_STREQ("a", stripParameterDecoration("${a}").c_str());
    EXPECT_STREQ("", stripParameterDecoration("${}").c_str());
}

