/**
 * @file   Array2dTest.cpp
 * @brief  Array2d class tester.
 * @author zer0
 * @date   2016-04-24
 */

#include <gtest/gtest.h>
#include <libtbag/container/Array2d.hpp>
#include <vector>
#include <algorithm>

using namespace libtbag;
using namespace libtbag::container;
using namespace libtbag::exception;

TEST(Array2dTest, Default)
{
    int const TEST1 = 1;
    int const TEST2 = 2;
    int const TEST3 = 3;
    int const TEST4 = 4;

    Array2d<int> a1;

    ASSERT_EQ(a1.width(), 0);
    ASSERT_EQ(a1.height(), 0);
    ASSERT_TRUE(a1.data() == nullptr);

    Array2d<int> a2(TEST2, TEST2);
    Array2d<int> a3 = Array2d<int>(TEST3, TEST3);
    a1 = Array2d<int>(TEST1, TEST1);

    ASSERT_EQ(a1.size(), static_cast<std::size_t>(TEST1 * TEST1));
    ASSERT_EQ(a2.size(), static_cast<std::size_t>(TEST2 * TEST2));
    ASSERT_EQ(a3.size(), static_cast<std::size_t>(TEST3 * TEST3));

    a2 = a3;
    ASSERT_EQ(a2.size(), static_cast<std::size_t>(TEST3 * TEST3));
    ASSERT_EQ(a3.size(), static_cast<std::size_t>(TEST3 * TEST3));

    a1 = Array2d<int>(TEST4, TEST4);
    ASSERT_EQ(a1.size(), static_cast<std::size_t>(TEST4 * TEST4));
}

TEST(Array2dTest, Index)
{
    int const TEST1  = 2;
    int const VALUE1 = 10;

    Array2d<int> a1(TEST1, TEST1);
    a1.at(0, 0) = VALUE1;

    ASSERT_EQ(*a1.data()   , VALUE1);
    ASSERT_EQ( a1[0]       , VALUE1);
    ASSERT_EQ( a1.at(0)    , VALUE1);
    ASSERT_EQ( a1.get(0)   , VALUE1);
    ASSERT_EQ( a1.get(0, 0), VALUE1);

    ASSERT_THROW(a1.at(TEST1 * TEST1) , OutOfRangeException);
    ASSERT_THROW(a1.get(TEST1 * TEST1), OutOfRangeException);

    ASSERT_THROW(a1.at(-1,  0)  , OutOfRangeException);
    ASSERT_THROW(a1.at( 0, -1)  , OutOfRangeException);
    ASSERT_THROW(a1.at(TEST1, 0), OutOfRangeException);
    ASSERT_THROW(a1.at(0, TEST1), OutOfRangeException);

    ASSERT_THROW(a1.get(-1,  0)  , OutOfRangeException);
    ASSERT_THROW(a1.get( 0, -1)  , OutOfRangeException);
    ASSERT_THROW(a1.get(TEST1, 0), OutOfRangeException);
    ASSERT_THROW(a1.get(0, TEST1), OutOfRangeException);
}

TEST(Array2dTest, ResizeException)
{
    Array2d<int> a1;
    ASSERT_THROW(a1.resize(0, 0), IllegalArgumentException);
    ASSERT_THROW(a1.resize(0, 1), IllegalArgumentException);
    ASSERT_THROW(a1.resize(1, 0), IllegalArgumentException);
}

TEST(Array2dTest, ConstIterator)
{
    // Just HIT.
    Array2d<int> const a1;
    Array2d<int>::ConstIterator itr1 = a1.cbegin();
    Array2d<int>::ConstIterator end1 = a1.cend();
    Array2d<int>::ConstIterator itr2 = a1.cbeginWithHeight();
    Array2d<int>::ConstIterator end2 = a1.cendWithHeight();

    ASSERT_TRUE(itr1 == end1);
    ASSERT_TRUE(itr2 == end2);

    ASSERT_EQ(Array2dIterator<int>::getDataSize(), sizeof(int));
}

TEST(Array2dTest, Iterator)
{
    Array2d<int> a1(2, 2);
    Array2d<int>::Iterator itr = a1.begin();
    Array2d<int>::Iterator end = a1.end();

    *itr = 10;
    ASSERT_EQ(a1.at(0, 0), 10);
    itr++;
    ASSERT_TRUE(itr != end);

    *itr = 20;
    ASSERT_EQ(a1.at(1, 0), 20);
    itr++;
    ASSERT_TRUE(itr != end);

    *itr = 30;
    ASSERT_EQ(a1.at(0, 1), 30);
    itr++;
    ASSERT_TRUE(itr != end);

    *itr = 40;
    ASSERT_EQ(a1.at(1, 1), 40);
    itr++;
    ASSERT_TRUE(itr == end);
}

TEST(Array2dTest, HeightIterator)
{
    Array2d<int> a1(2, 2);
    Array2d<int>::Iterator itr = a1.beginWithHeight();
    Array2d<int>::Iterator end = a1.endWithHeight();

    *itr = 10;
    ASSERT_EQ(a1.at(0, 0), 10);
    itr++;
    ASSERT_TRUE(itr != end);

    *itr = 20;
    ASSERT_EQ(a1.at(0, 1), 20);
    itr++;
    ASSERT_TRUE(itr == end);
}

TEST(Array2dTest, Iterator_StdCopy)
{
    Array2d<int> a1(2, 2);
    a1[0] = 0;
    a1[1] = 1;
    a1[2] = 2;
    a1[3] = 3;

    std::vector<int> vec(4);
    std::copy(a1.begin(), a1.end(), vec.begin());
    ASSERT_EQ(a1[0], vec[0]);
    ASSERT_EQ(a1[1], vec[1]);
    ASSERT_EQ(a1[2], vec[2]);
    ASSERT_EQ(a1[3], vec[3]);
}

