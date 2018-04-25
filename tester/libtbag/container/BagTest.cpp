/**
 * @file   BagTest.cpp
 * @brief  Bag class tester.
 * @author zer0
 * @date   2018-04-25
 */

#include <gtest/gtest.h>
#include <libtbag/container/Bag.hpp>

using namespace libtbag;
using namespace libtbag::container;

template <typename T>
struct BagTest : public testing::Test
{
    Bag<T> bag;

    virtual void SetUp() override
    {
        // EMPTY.
    }

    virtual void TearDown() override
    {
        // EMPTY.
    }
};

using BagTypes = testing::Types<char, int, unsigned int>;
TYPED_TEST_CASE(BagTest, BagTypes);

TYPED_TEST(BagTest, Default)
{
    //ASSERT_EQ(0, this->bag.size());
}

