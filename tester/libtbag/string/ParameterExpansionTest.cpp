/**
 * @file   ParameterExpansionTest.cpp
 * @brief  ParameterExpansion class tester.
 * @author zer0
 * @date   2019-10-10
 */

#include <gtest/gtest.h>
#include <libtbag/util/TestUtils.hpp>
#include <libtbag/string/ParameterExpansion.hpp>
#include <libtbag/filesystem/File.hpp>

using namespace libtbag;
using namespace libtbag::string;

TEST(ParameterExpansionTest, StripParameterDecoration)
{
    EXPECT_STREQ("tt", stripParameterDecoration(" ${tt} ").c_str());
    EXPECT_STREQ("test", stripParameterDecoration("${test}").c_str());
    EXPECT_STREQ("a", stripParameterDecoration("${a}").c_str());
    EXPECT_STREQ("", stripParameterDecoration("${}").c_str());
}

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

TEST_F(ParameterExpansionFeatureTest, DefaultVariable)
{
    EXPECT_EQ(std::string(TEST_VAL), parameterExpansion("${string}"));
    EXPECT_EQ(std::string(TEST_VAL), parameterExpansion("${string:-AAA}"));

    EXPECT_STREQ("", parameterExpansion("${test}").c_str());
    EXPECT_STREQ("AAA", parameterExpansion("${test:-AAA}").c_str());
}

TEST_F(ParameterExpansionFeatureTest, Length)
{
    EXPECT_EQ(libtbag::string::toString(std::string(TEST_VAL).size()), parameterExpansion("${#string}"));
}

TEST_F(ParameterExpansionFeatureTest, ReadFile)
{
    tttDir_Automatic();
    auto const PATH = tttDir_Get();

    auto const * const FILENAME = "temp.txt";
    auto const * const PATH_KEY = "path";
    auto const * const CONTENT = "0123456789";
    flags.push(PATH_KEY, PATH);

    ASSERT_EQ(E_SUCCESS, libtbag::filesystem::writeFile(PATH / FILENAME, CONTENT));
    ASSERT_EQ(PATH, parameterExpansion("${path}").c_str());
    ASSERT_STREQ(CONTENT, parameterExpansion("${path@/temp.txt}").c_str());
}

