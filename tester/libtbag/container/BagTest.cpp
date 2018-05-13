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

TEST(BagContainerTest, Default)
{
    using TestBagAllocator = std::allocator<int>;
    using TestBagVector    = std::vector<int, TestBagAllocator>;
    using TestBagContainer = BagContainer<int, std::allocator, TestBagVector>;

    TestBagContainer container;
    ASSERT_EQ(0, container.size());
    ASSERT_TRUE(container.empty());

    int const TEST_SIZE = 10;

    container.resize(TEST_SIZE);
    for (int i = 0; i < TEST_SIZE; ++i) {
        container.at(i) = i;
    }
    ASSERT_EQ(TEST_SIZE, container.size());
    ASSERT_FALSE(container.empty());
    for (int i = 0; i < TEST_SIZE; ++i) {
        ASSERT_EQ(i, *(container.data() + i));
    }

    TestBagContainer container2;
    ASSERT_EQ(0, container2.size());
    ASSERT_TRUE(container2.empty());

    container2.swap(container);
    ASSERT_EQ(TEST_SIZE, container2.size());
    ASSERT_FALSE(container2.empty());
    ASSERT_EQ(0, container.size());
    ASSERT_TRUE(container.empty());
    for (int i = 0; i < TEST_SIZE; ++i) {
        ASSERT_EQ(i, container2.at(i));
    }

    container.copy(container2);
    ASSERT_EQ(TEST_SIZE, container.size());
    ASSERT_FALSE(container.empty());
    ASSERT_EQ(TEST_SIZE, container2.size());
    ASSERT_FALSE(container2.empty());

    container.clear();
    ASSERT_EQ(0, container.size());
    ASSERT_TRUE(container.empty());

    container2.clear();
    ASSERT_EQ(0, container2.size());
    ASSERT_TRUE(container2.empty());
}

TEST(BagDimensions, Default)
{
    using TestBagDimensions = BagDimensions<8>;
    ASSERT_EQ(8, TestBagDimensions::maxDimSize());

    TestBagDimensions dims;
    ASSERT_EQ(0, dims.size());
    ASSERT_EQ(0, dims.total());
    ASSERT_TRUE(dims.empty());

    dims.resize(2, 3, 4, 5);
    ASSERT_EQ(2, dims.at(0));
    ASSERT_EQ(3, dims.at(1));
    ASSERT_EQ(4, dims.at(2));
    ASSERT_EQ(5, dims.at(3));

    ASSERT_EQ(4, dims.size());
    ASSERT_EQ(2*3*4*5, dims.total());
    ASSERT_FALSE(dims.empty());

    ASSERT_EQ(0, dims.offset(0));
    ASSERT_EQ(1, dims.offset(1));
    ASSERT_EQ((2)+(1), dims.offset(1, 1));
    ASSERT_EQ((2*3)+(2)+(1), dims.offset(1, 1, 1));

    dims.clear();
    ASSERT_EQ(0, dims.size());
    ASSERT_EQ(0, dims.total());
    ASSERT_TRUE(dims.empty());

    TestBagDimensions dims2;
    dims2.resize(2, 3);
    ASSERT_EQ(2, dims2.size());

    dims.swap(dims2);
    ASSERT_EQ(2, dims.size());
    ASSERT_EQ(0, dims2.size());

    dims2.copy(dims);
    ASSERT_EQ(2, dims.size());
    ASSERT_EQ(2, dims2.size());
}

template <typename T>
struct BagTest : public testing::Test
{
    Bag<T> bag;

    virtual void SetUp() override
    {
        bag.resize(2, 3, 4);
        for (int i = 0; i < bag.size(); ++i) {
            bag.at(i) = i;
        }
    }

    virtual void TearDown() override
    {
        bag.clear();
    }
};

using BagTypes = testing::Types<char, int, unsigned, double>;
TYPED_TEST_CASE(BagTest, BagTypes);

TYPED_TEST(BagTest, Default)
{
    auto & b = this->bag;
    ASSERT_EQ(2*3*4, b.size());
    ASSERT_EQ(2, b.size(0));
    ASSERT_EQ(3, b.size(1));
    ASSERT_EQ(4, b.size(2));
    ASSERT_FALSE(b.empty());
}

TYPED_TEST(BagTest, Elements)
{
    auto & b = this->bag;

    ASSERT_EQ( 0, b.atOffset(0, 0, 0));
    ASSERT_EQ( 1, b.atOffset(1, 0, 0));
    ASSERT_EQ( 2, b.atOffset(0, 1, 0));
    ASSERT_EQ( 3, b.atOffset(1, 1, 0));
    ASSERT_EQ( 4, b.atOffset(0, 2, 0));
    ASSERT_EQ( 5, b.atOffset(1, 2, 0));

    ASSERT_EQ( 6, b.atOffset(0, 0, 1));
    ASSERT_EQ( 7, b.atOffset(1, 0, 1));
    ASSERT_EQ( 8, b.atOffset(0, 1, 1));
    ASSERT_EQ( 9, b.atOffset(1, 1, 1));
    ASSERT_EQ(10, b.atOffset(0, 2, 1));
    ASSERT_EQ(11, b.atOffset(1, 2, 1));

    ASSERT_EQ(12, b.atOffset(0, 0, 2));
    ASSERT_EQ(13, b.atOffset(1, 0, 2));
    ASSERT_EQ(14, b.atOffset(0, 1, 2));
    ASSERT_EQ(15, b.atOffset(1, 1, 2));
    ASSERT_EQ(16, b.atOffset(0, 2, 2));
    ASSERT_EQ(17, b.atOffset(1, 2, 2));

    ASSERT_EQ(18, b.atOffset(0, 0, 3));
    ASSERT_EQ(19, b.atOffset(1, 0, 3));
    ASSERT_EQ(20, b.atOffset(0, 1, 3));
    ASSERT_EQ(21, b.atOffset(1, 1, 3));
    ASSERT_EQ(22, b.atOffset(0, 2, 3));
    ASSERT_EQ(23, b.atOffset(1, 2, 3));
}

TYPED_TEST(BagTest, ForEachIterator)
{
    auto & b = this->bag;
    int i = 0;
    for (auto & cursor : b) {
        ++i;
    }
    ASSERT_EQ(2*3*4, i);
}

TYPED_TEST(BagTest, Iterator)
{
    auto & b = this->bag;
    auto itr = b.begin();
    // @formatter:off
    ASSERT_EQ( 0, *itr++); ASSERT_NE(b.end(), itr);
    ASSERT_EQ( 1, *itr++); ASSERT_NE(b.end(), itr);
    ASSERT_EQ( 2, *itr++); ASSERT_NE(b.end(), itr);
    ASSERT_EQ( 3, *itr++); ASSERT_NE(b.end(), itr);
    ASSERT_EQ( 4, *itr++); ASSERT_NE(b.end(), itr);
    ASSERT_EQ( 5, *itr++); ASSERT_NE(b.end(), itr);
    ASSERT_EQ( 6, *itr++); ASSERT_NE(b.end(), itr);
    ASSERT_EQ( 7, *itr++); ASSERT_NE(b.end(), itr);
    ASSERT_EQ( 8, *itr++); ASSERT_NE(b.end(), itr);
    ASSERT_EQ( 9, *itr++); ASSERT_NE(b.end(), itr);
    ASSERT_EQ(10, *itr++); ASSERT_NE(b.end(), itr);
    ASSERT_EQ(11, *itr++); ASSERT_NE(b.end(), itr);
    ASSERT_EQ(12, *itr++); ASSERT_NE(b.end(), itr);
    ASSERT_EQ(13, *itr++); ASSERT_NE(b.end(), itr);
    ASSERT_EQ(14, *itr++); ASSERT_NE(b.end(), itr);
    ASSERT_EQ(15, *itr++); ASSERT_NE(b.end(), itr);
    ASSERT_EQ(16, *itr++); ASSERT_NE(b.end(), itr);
    ASSERT_EQ(17, *itr++); ASSERT_NE(b.end(), itr);
    ASSERT_EQ(18, *itr++); ASSERT_NE(b.end(), itr);
    ASSERT_EQ(19, *itr++); ASSERT_NE(b.end(), itr);
    ASSERT_EQ(20, *itr++); ASSERT_NE(b.end(), itr);
    ASSERT_EQ(21, *itr++); ASSERT_NE(b.end(), itr);
    ASSERT_EQ(22, *itr++); ASSERT_NE(b.end(), itr);
    ASSERT_EQ(23, *itr++); ASSERT_EQ(b.end(), itr);
    // @formatter:on
}

TEST(BagTest_NoTyped, Default)
{
    using TestBag = Bag<int>;

    TestBag bag1;
    ASSERT_EQ(0, bag1.size());
    ASSERT_TRUE(bag1.empty());

    TestBag bag2 = {2, 3};
    ASSERT_EQ(2*3, bag2.size());
    ASSERT_FALSE(bag2.empty());

    TestBag bag3 = bag2;
    ASSERT_EQ(2*3, bag3.size());
    ASSERT_FALSE(bag3.empty());

    TestBag bag4 = std::move(bag3);
    ASSERT_EQ(0, bag3.size());
    ASSERT_TRUE(bag3.empty());
    ASSERT_EQ(2*3, bag4.size());
    ASSERT_FALSE(bag4.empty());

    TestBag bag5;
    bag5 = bag4;
    ASSERT_EQ(2*3, bag5.size());
    ASSERT_FALSE(bag5.empty());

    TestBag bag6;
    bag6 = std::move(bag5);
    ASSERT_EQ(2*3, bag6.size());
    ASSERT_FALSE(bag6.empty());
    ASSERT_EQ(0, bag5.size());
    ASSERT_TRUE(bag5.empty());
}

