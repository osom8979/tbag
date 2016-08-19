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
    /* TODO: Valgrind memory leak:
     * ==38325== 16 bytes in 1 blocks are definitely lost in loss record 7 of 107
     * ==38325==    at 0x1002785B9: calloc (in /usr/local/Cellar/valgrind/3.11.0/lib/valgrind/vgpreload_memcheck-amd64-darwin.so)
     * ==38325==    by 0x100E437CE: __cxa_get_globals (in /usr/lib/libc++abi.dylib)
     * ==38325==    by 0x100E43BB1: __cxa_throw (in /usr/lib/libc++abi.dylib)
     * ==38325==    by 0x1000687CF: libtbag::container::Array2d<int, std::__1::allocator<int> >::at(unsigned long) (in ./tester)
     * ==38325==    by 0x1000636BD: Array2dTest_Index_Test::TestBody() (in ./tester)
     * ==38325==    by 0x1001786C9: void testing::internal::HandleExceptionsInMethodIfSupported<testing::Test, void>(testing::Test*, void (testing::Test::*)(), char const*) (gtest.cc:2078)
     * ==38325==    by 0x100165149: testing::Test::Run() (gtest.cc:2150)
     * ==38325==    by 0x100165E01: testing::TestInfo::Run() (gtest.cc:2326)
     * ==38325==    by 0x100166642: testing::TestCase::Run() (gtest.cc:2444)
     * ==38325==    by 0x10016D77A: testing::internal::UnitTestImpl::RunAllTests() (gtest.cc:4315)
     * ==38325==    by 0x100178EE2: bool testing::internal::HandleExceptionsInMethodIfSupported<testing::internal::UnitTestImpl, bool>(testing::internal::UnitTestImpl*, bool (testing::internal::UnitTestImpl::*)(), char const*) (gtest.cc:2078)
     * ==38325==    by 0x10016D3ED: testing::UnitTest::Run() (gtest.cc:3926)
     */

    int const TEST1  = 2;
    int const VALUE1 = 10;

    Array2d<int> a1(TEST1, TEST1);
    a1.at(0, 0) = VALUE1;

    ASSERT_EQ(VALUE1, *a1.data()   );
    ASSERT_EQ(VALUE1,  a1[0]       );
    ASSERT_EQ(VALUE1,  a1.at(0)    );
    ASSERT_EQ(VALUE1,  a1.get(0)   );
    ASSERT_EQ(VALUE1,  a1.get(0, 0));

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

    ASSERT_EQ(sizeof(int), Array2dIterator<int>::getDataSize());
}

TEST(Array2dTest, Iterator)
{
    Array2d<int> a1(2, 2);
    Array2d<int>::Iterator itr = a1.begin();
    Array2d<int>::Iterator end = a1.end();

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
    Array2d<int>::Iterator itr = a1.beginWithHeight();
    Array2d<int>::Iterator end = a1.endWithHeight();

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

