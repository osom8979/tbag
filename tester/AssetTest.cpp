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

TEST_F(AssetTest, Directory)
{
    ASSERT_TRUE(true);
}

