/**
 * @file   CircularBufferTest.cpp
 * @brief  CircularBuffer class tester.
 * @author zer0
 * @date   2016-12-29
 */

#include <gtest/gtest.h>
#include <libtbag/container/CircularBuffer.hpp>

#include <string>

using namespace libtbag;
using namespace libtbag::container;

using TestBuffer = CircularBuffer<char>;

TEST(CircularBufferTest, NestedBufferTest)
{
    std::size_t const TEST_SIZE = 5;
    char const TEST_VALUE = static_cast<char>(0x0F);
    TestBuffer::Buffer buffer(TEST_SIZE, TEST_VALUE);

    std::size_t capacity = buffer.capacity();
    std::size_t size     = buffer.size();

    ASSERT_EQ(TEST_SIZE, size);
    ASSERT_GE(size, capacity);

    std::size_t i;
    for (i = 0; i < size; ++i) {
        ASSERT_EQ(TEST_VALUE, buffer[i]);
    }

    buffer.resize(capacity + TEST_SIZE);
    ASSERT_GE(capacity + TEST_SIZE, buffer.size());

    for (i = 0; i < size; ++i) {
        ASSERT_EQ(TEST_VALUE, buffer[i]);
    }
}

TEST(CircularBufferTest, PushAndPop1)
{
    int const BUFFER_SIZE = 5;
    TestBuffer buf(BUFFER_SIZE);
    char read[BUFFER_SIZE + 1] = {0,};

    ASSERT_EQ(2, buf.push("AB", 2));
    ASSERT_FALSE(buf.empty());
    ASSERT_EQ(2, buf.size());
    ASSERT_EQ(0, buf.start());
    ASSERT_EQ(5, buf.max());
    ASSERT_EQ(3, buf.free());
    ASSERT_EQ(2, buf.last());
    ASSERT_EQ('A', buf[0]);
    ASSERT_EQ('B', buf[1]);

    ASSERT_EQ(2, buf.pop(read, BUFFER_SIZE));
    ASSERT_TRUE(buf.empty());
    ASSERT_EQ(0, buf.size());
    ASSERT_EQ(2, buf.start());
    ASSERT_EQ(5, buf.max());
    ASSERT_EQ(5, buf.free());
    ASSERT_EQ(2, buf.last());
    ASSERT_EQ('A', read[0]);
    ASSERT_EQ('B', read[1]);

    ASSERT_EQ(4, buf.push("CDEF", 4));
    ASSERT_FALSE(buf.empty());
    ASSERT_EQ(4, buf.size());
    ASSERT_EQ(2, buf.start());
    ASSERT_EQ(5, buf.max());
    ASSERT_EQ(1, buf.free());
    ASSERT_EQ(1, buf.last());
    ASSERT_EQ('C', buf[2]);
    ASSERT_EQ('D', buf[3]);
    ASSERT_EQ('E', buf[4]);
    ASSERT_EQ('F', buf[0]);

    ASSERT_EQ(4, buf.pop(read, BUFFER_SIZE));
    ASSERT_TRUE(buf.empty());
    ASSERT_EQ(0, buf.size());
    ASSERT_EQ(1, buf.start());
    ASSERT_EQ(5, buf.max());
    ASSERT_EQ(5, buf.free());
    ASSERT_EQ(1, buf.last());
    ASSERT_EQ('C', read[0]);
    ASSERT_EQ('D', read[1]);
    ASSERT_EQ('E', read[2]);
    ASSERT_EQ('F', read[3]);
}

TEST(CircularBufferTest, PushAndPop2)
{
    int const BUFFER_SIZE = 3;
    TestBuffer buf(BUFFER_SIZE);
    char read[BUFFER_SIZE + 1] = {0,};

    ASSERT_EQ(3, buf.push("ABCD", 4));
    ASSERT_EQ(3, buf.size());
    ASSERT_EQ(0, buf.push("ABCD", 4));
    ASSERT_FALSE(buf.empty());
    ASSERT_EQ(3, buf.size());
    ASSERT_EQ(0, buf.start());
    ASSERT_EQ(3, buf.max());
    ASSERT_EQ(0, buf.free());
    ASSERT_EQ(0, buf.last());
    ASSERT_EQ('A', buf[0]);
    ASSERT_EQ('B', buf[1]);
    ASSERT_EQ('C', buf[2]);

    ASSERT_EQ(3, buf.pop(read, BUFFER_SIZE));
    ASSERT_TRUE(buf.empty());
    ASSERT_EQ(0, buf.size());
    ASSERT_EQ(0, buf.start());
    ASSERT_EQ(3, buf.max());
    ASSERT_EQ(3, buf.free());
    ASSERT_EQ(0, buf.last());
    ASSERT_EQ('A', read[0]);
    ASSERT_EQ('B', read[1]);
    ASSERT_EQ('C', read[2]);

    ASSERT_EQ(2, buf.push("EF", 2));
    ASSERT_FALSE(buf.empty());
    ASSERT_EQ(2, buf.size());
    ASSERT_EQ(0, buf.start());
    ASSERT_EQ(3, buf.max());
    ASSERT_EQ(1, buf.free());
    ASSERT_EQ(2, buf.last());
    ASSERT_EQ('E', buf[0]);
    ASSERT_EQ('F', buf[1]);

    ASSERT_EQ(2, buf.pop(read, BUFFER_SIZE));
    ASSERT_TRUE(buf.empty());
    ASSERT_EQ(0, buf.size());
    ASSERT_EQ(2, buf.start());
    ASSERT_EQ(3, buf.max());
    ASSERT_EQ(3, buf.free());
    ASSERT_EQ(2, buf.last());
    ASSERT_EQ('E', read[0]);
    ASSERT_EQ('F', read[1]);

    ASSERT_EQ(2, buf.push("GH", 2));
    ASSERT_FALSE(buf.empty());
    ASSERT_EQ(2, buf.size());
    ASSERT_EQ(2, buf.start());
    ASSERT_EQ(3, buf.max());
    ASSERT_EQ(1, buf.free());
    ASSERT_EQ(1, buf.last());
    ASSERT_EQ('G', buf[2]);
    ASSERT_EQ('H', buf[0]);

    ASSERT_EQ(2, buf.pop(read, BUFFER_SIZE));
    ASSERT_TRUE(buf.empty());
    ASSERT_EQ(0, buf.size());
    ASSERT_EQ(1, buf.start());
    ASSERT_EQ(3, buf.max());
    ASSERT_EQ(3, buf.free());
    ASSERT_EQ(1, buf.last());
    ASSERT_EQ('G', read[0]);
    ASSERT_EQ('H', read[1]);
}

TEST(CircularBufferTest, PushAndPop3)
{
    int const BUFFER_SIZE = 3;
    TestBuffer buf(BUFFER_SIZE);
    char read[BUFFER_SIZE + 1] = {0,};

    ASSERT_EQ(1, buf.push("A", 1));
    ASSERT_EQ(1, buf.push("B", 1));
    ASSERT_EQ(1, buf.push("C", 1));
    ASSERT_EQ(0, buf.push("D", 1));

    ASSERT_FALSE(buf.empty());
    ASSERT_EQ(3, buf.size());
    ASSERT_EQ(0, buf.start());
    ASSERT_EQ(3, buf.max());
    ASSERT_EQ(0, buf.free());
    ASSERT_EQ(0, buf.last());

    ASSERT_EQ('A', buf[0]);
    ASSERT_EQ('B', buf[1]);
    ASSERT_EQ('C', buf[2]);

    ASSERT_EQ(1, buf.pop(read + 0, 1));
    ASSERT_EQ(1, buf.pop(read + 1, 1));
    ASSERT_EQ(1, buf.pop(read + 2, 1));
    ASSERT_EQ(0, buf.pop(read + 3, 1));

    ASSERT_EQ('A', read[0]);
    ASSERT_EQ('B', read[1]);
    ASSERT_EQ('C', read[2]);
}

TEST(CircularBufferTest, extendCapacity)
{
    TestBuffer buf(4);
    ASSERT_EQ(4, buf.push("ABCD", 4));
    ASSERT_EQ(4, buf.size());
    ASSERT_EQ(4, buf.max());
    ASSERT_EQ(0, buf.free());
    ASSERT_EQ('A', buf[0]);
    ASSERT_EQ('B', buf[1]);
    ASSERT_EQ('C', buf[2]);
    ASSERT_EQ('D', buf[3]);

    buf.extendCapacity(4);
    ASSERT_EQ(4, buf.size());
    ASSERT_EQ(8, buf.max());
    ASSERT_EQ(4, buf.free());
    ASSERT_EQ('A', buf[0]);
    ASSERT_EQ('B', buf[1]);
    ASSERT_EQ('C', buf[2]);
    ASSERT_EQ('D', buf[3]);

    ASSERT_EQ(4, buf.push("EFGH", 4));
    ASSERT_EQ(8, buf.size());
    ASSERT_EQ(8, buf.max());
    ASSERT_EQ(0, buf.free());
    ASSERT_EQ('A', buf[0]);
    ASSERT_EQ('B', buf[1]);
    ASSERT_EQ('C', buf[2]);
    ASSERT_EQ('D', buf[3]);
    ASSERT_EQ('E', buf[4]);
    ASSERT_EQ('F', buf[5]);
    ASSERT_EQ('G', buf[6]);
    ASSERT_EQ('H', buf[7]);
}

