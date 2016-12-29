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

using TestBuffer  = CircularBuffer<int>;
using TestIndexer = TestBuffer::Indexer;

TEST(CircularBufferTest, Indexer)
{
    TestBuffer buffer(5);
    TestIndexer index = buffer.getIndex();

    ASSERT_EQ(0, index.r);
    ASSERT_EQ(0, index.s);

    ASSERT_EQ(0, index.size());
    ASSERT_EQ(5, index.max());
    ASSERT_EQ(5, index.free());
    ASSERT_EQ(0, index.last());
    ASSERT_TRUE(index.empty());

    index.r = 1;
    index.s = 2;
    ASSERT_EQ(2, index.size());
    ASSERT_EQ(5, index.max());
    ASSERT_EQ(3, index.free());
    ASSERT_EQ(3, index.last());
    ASSERT_FALSE(index.empty());

    index.r = 3;
    index.s = 3;
    ASSERT_EQ(3, index.size());
    ASSERT_EQ(5, index.max());
    ASSERT_EQ(2, index.free());
    ASSERT_EQ(1, index.last());
    ASSERT_FALSE(index.empty());
}

TEST(CircularBufferTest, Iterator)
{
    TestBuffer buf(5);
    ASSERT_EQ(buf.begin(), buf.end());

    buf[0] = 10;
    buf.atIndex().r = 0;
    buf.atIndex().s = 1;

    auto itr = buf.begin();
    itr++;
    ASSERT_EQ(buf.end(), itr);
}

TEST(CircularBufferTest, PushAndPop1)
{
    std::string temp = "AB";
    CircularBuffer<char> buf(5);
    std::string reader(5, '\0');

    ASSERT_EQ(2, buf.push(&temp[0], temp.size()));
    ASSERT_EQ(2, buf.size());
    ASSERT_EQ('A', buf[0]);
    ASSERT_EQ('B', buf[1]);

    ASSERT_EQ(2, buf.pop(&reader[0], reader.size()));
    ASSERT_EQ(0, buf.size());
    ASSERT_EQ('A', reader[0]);
    ASSERT_EQ('B', reader[1]);

    temp = "CDEF";
    ASSERT_EQ(4, buf.push(&temp[0], temp.size()));
    ASSERT_EQ(4, buf.size());
    ASSERT_EQ('C', buf[2]);
    ASSERT_EQ('D', buf[3]);
    ASSERT_EQ('E', buf[4]);
    ASSERT_EQ('F', buf[0]);

    ASSERT_EQ(4, buf.pop(&reader[0], reader.size()));
    ASSERT_EQ(0, buf.size());
    ASSERT_EQ('C', reader[0]);
    ASSERT_EQ('D', reader[1]);
    ASSERT_EQ('E', reader[2]);
    ASSERT_EQ('F', reader[3]);
}

TEST(CircularBufferTest, PushAndPop2)
{
    std::string temp = "ABCD";
    CircularBuffer<char> buf(3);
    std::string reader(3, '\0');

    ASSERT_EQ(3, buf.push(&temp[0], temp.size()));
    ASSERT_EQ(3, buf.size());
    ASSERT_EQ(0, buf.push(&temp[0], temp.size()));
    ASSERT_EQ(3, buf.size());
    ASSERT_EQ('A', buf[0]);
    ASSERT_EQ('B', buf[1]);
    ASSERT_EQ('C', buf[2]);

    ASSERT_EQ(3, buf.pop(&reader[0], reader.size()));
    ASSERT_EQ(0, buf.size());
    ASSERT_EQ('A', reader[0]);
    ASSERT_EQ('B', reader[1]);
    ASSERT_EQ('C', reader[2]);

    std::string temp2 = "EF";
    ASSERT_EQ(2, buf.push(&temp2[0], temp2.size()));
    ASSERT_EQ(2, buf.size());
    ASSERT_EQ('E', buf[0]);
    ASSERT_EQ('F', buf[1]);

    ASSERT_EQ(2, buf.pop(&reader[0], reader.size()));
    ASSERT_EQ(0, buf.size());
    ASSERT_EQ('E', reader[0]);
    ASSERT_EQ('F', reader[1]);

    std::string temp3 = "GH";
    ASSERT_EQ(2, buf.push(&temp3[0], temp3.size()));
    ASSERT_EQ(2, buf.size());
    ASSERT_EQ('G', buf[2]);
    ASSERT_EQ('H', buf[0]);

    ASSERT_EQ(2, buf.pop(&reader[0], reader.size()));
    ASSERT_EQ(0, buf.size());
    ASSERT_EQ('G', reader[0]);
    ASSERT_EQ('H', reader[1]);
}

TEST(CircularBufferTest, PushAndPop3_UnSafe)
{
    std::string temp = "ABCD";
    CircularBuffer<char> buf(3);
    std::string reader(3, '\0');

    ASSERT_EQ(4, buf.push(&temp[0], temp.size(), false));
    ASSERT_EQ(3, buf.size());
    ASSERT_EQ(0, buf.push(&temp[0], temp.size()));
    ASSERT_EQ(3, buf.size());
    ASSERT_EQ('D', buf[0]);
    ASSERT_EQ('B', buf[1]);
    ASSERT_EQ('C', buf[2]);

    ASSERT_EQ(3, buf.pop(&reader[0], reader.size()));
    ASSERT_EQ(0, buf.size());
    ASSERT_EQ('B', reader[0]);
    ASSERT_EQ('C', reader[1]);
    ASSERT_EQ('D', reader[2]);

    std::string temp2 = "EF";
    ASSERT_EQ(2, buf.push(&temp2[0], temp2.size()));
    ASSERT_EQ(2, buf.size());
    ASSERT_EQ('D', buf[0]);
    ASSERT_EQ('E', buf[1]);
    ASSERT_EQ('F', buf[2]);

    ASSERT_EQ(2, buf.pop(&reader[0], reader.size()));
    ASSERT_EQ(0, buf.size());
    ASSERT_EQ('E', reader[0]);
    ASSERT_EQ('F', reader[1]);
}

