/**
 * @file   Array2dTest.cpp
 * @brief  Array2d class tester.
 * @author zer0
 * @date   2017-06-10
 */

#include <gtest/gtest.h>
#include <libtbag/container/Array2d.hpp>

using namespace libtbag;
using namespace libtbag::container;

TEST(Array2dTest, Default)
{
    int const TEST1 = 1;
    int const TEST2 = 2;
    int const TEST3 = 3;
    int const TEST4 = 4;

    Array2d<int> a1;

    ASSERT_EQ(0, a1.width());
    ASSERT_EQ(0, a1.height());
    ASSERT_EQ(nullptr, a1.data());

    Array2d<int> a2(TEST2, TEST2);
    Array2d<int> a3 = Array2d<int>(TEST3, TEST3);
    a1 = Array2d<int>(TEST1, TEST1);

    ASSERT_EQ(static_cast<std::size_t>(TEST1 * TEST1), a1.size());
    ASSERT_EQ(static_cast<std::size_t>(TEST2 * TEST2), a2.size());
    ASSERT_EQ(static_cast<std::size_t>(TEST3 * TEST3), a3.size());

    a2 = a3;
    ASSERT_EQ(static_cast<std::size_t>(TEST3 * TEST3), a2.size());
    ASSERT_EQ(static_cast<std::size_t>(TEST3 * TEST3), a3.size());

    a1 = Array2d<int>(TEST4, TEST4);
    ASSERT_EQ(static_cast<std::size_t>(TEST4 * TEST4), a1.size());
}

TEST(Array2dTest, Index)
{
    int const TEST1  = 2;
    int const VALUE1 = 10;

    Array2d<int> a1(TEST1, TEST1);
    a1.at(0, 0) = VALUE1;

    ASSERT_EQ(VALUE1, *a1.data()   );
    ASSERT_EQ(VALUE1,  a1[0]       );
    ASSERT_EQ(VALUE1,  a1.at(0)    );
    ASSERT_EQ(VALUE1,  a1.get(0)   );
    ASSERT_EQ(VALUE1,  a1.get(0, 0));

    ASSERT_THROW(a1.at(TEST1 * TEST1) , std::out_of_range);
    ASSERT_THROW(a1.get(TEST1 * TEST1), std::out_of_range);

    ASSERT_THROW(a1.at(-1,  0)  , std::out_of_range);
    ASSERT_THROW(a1.at( 0, -1)  , std::out_of_range);
    ASSERT_THROW(a1.at(TEST1, 0), std::out_of_range);
    ASSERT_THROW(a1.at(0, TEST1), std::out_of_range);

    ASSERT_THROW(a1.get(-1,  0)  , std::out_of_range);
    ASSERT_THROW(a1.get( 0, -1)  , std::out_of_range);
    ASSERT_THROW(a1.get(TEST1, 0), std::out_of_range);
    ASSERT_THROW(a1.get(0, TEST1), std::out_of_range);
}

TEST(Array2dTest, ConstIterator)
{
    // Just HIT.
    Array2d<int> const a1;
    Array2d<int>::const_iterator itr1 = a1.cbegin();
    Array2d<int>::const_iterator end1 = a1.cend();
    Array2d<int>::const_iterator itr2 = a1.cbeginWithHeight();
    Array2d<int>::const_iterator end2 = a1.cendWithHeight();

    ASSERT_TRUE(itr1 == end1);
    ASSERT_TRUE(itr2 == end2);
}

TEST(Array2dTest, Iterator)
{
    Array2d<int> a1(2, 2);
    Array2d<int>::iterator itr = a1.begin();
    Array2d<int>::iterator end = a1.end();

    *itr = 10;
    ASSERT_EQ(10, a1.at(0, 0));
    itr++;
    ASSERT_TRUE(itr != end);

    *itr = 20;
    ASSERT_EQ(20, a1.at(1, 0));
    itr++;
    ASSERT_TRUE(itr != end);

    *itr = 30;
    ASSERT_EQ(30, a1.at(0, 1));
    itr++;
    ASSERT_TRUE(itr != end);

    *itr = 40;
    ASSERT_EQ(40, a1.at(1, 1));
    itr++;
    ASSERT_TRUE(itr == end);
}

TEST(Array2dTest, HeightIterator)
{
    Array2d<int> a1(2, 2);
    Array2d<int>::iterator itr = a1.beginWithHeight();
    Array2d<int>::iterator end = a1.endWithHeight();

    *itr = 10;
    ASSERT_EQ(10, a1.at(0, 0));
    itr++;
    ASSERT_TRUE(itr != end);

    *itr = 20;
    ASSERT_EQ(20, a1.at(0, 1));
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
    ASSERT_EQ(vec[0], a1[0]);
    ASSERT_EQ(vec[1], a1[1]);
    ASSERT_EQ(vec[2], a1[2]);
    ASSERT_EQ(vec[3], a1[3]);
}

