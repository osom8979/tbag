/**
 * @file   basic.cpp
 * @brief  Basic tester.
 * @author zer0
 * @date   2016-01-18
 */

#include <gtest/gtest.h>
#include <libtbag/Asset.hpp>

using namespace libtbag;

class AssetTest : public ::testing::Test
{
private:
    Asset _asset;

public:
    virtual void SetUp() {
    }

    virtual void TearDown() {
    }
};

TEST_F(AssetTest, PathOperators)
{
# if defined(WIN32) || defined(_WIN32)
    EXPECT_EQ(GetPathSeparator(), '\\');
    EXPECT_EQ(GetPathSplitter(),  ';');
# else
    EXPECT_EQ(GetPathSeparator(), '/');
    EXPECT_EQ(GetPathSplitter(),  ':');
# endif
}

