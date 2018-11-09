/**
 * @file   BoundMpMcQueueTest.cpp
 * @brief  BoundMpMcQueue class tester.
 * @author zer0
 * @date   2018-11-09
 */

#include <gtest/gtest.h>
#include <libtbag/lockfree/BoundMpMcQueue.hpp>

using namespace libtbag;
using namespace libtbag::lockfree;

struct BoundMpMcQueueTestData
{
    char name[64] = {0,};
};

TEST(BoundMpMcQueueTest, Default)
{
    char const * TEST1_DATA = "TEST1";
    char const * TEST2_DATA = "TEST2";
    char const * TEST3_DATA = "TEST3";
    char const * TEST4_DATA = "TEST4";
    char const * TEST_ERROR = "ERROR";

    BoundMpMcQueueTestData data1;
    BoundMpMcQueueTestData data2;
    BoundMpMcQueueTestData data3;
    BoundMpMcQueueTestData data4;
    BoundMpMcQueueTestData data_error;

    ASSERT_NE(nullptr, strcpy(data1.name, TEST1_DATA));
    ASSERT_NE(nullptr, strcpy(data2.name, TEST2_DATA));
    ASSERT_NE(nullptr, strcpy(data3.name, TEST3_DATA));
    ASSERT_NE(nullptr, strcpy(data4.name, TEST4_DATA));
    ASSERT_NE(nullptr, strcpy(data_error.name, TEST_ERROR));

    BoundMpMcQueue queue(4);
    ASSERT_TRUE(queue.enqueue(&data1));
    ASSERT_TRUE(queue.enqueue(&data2));
    ASSERT_TRUE(queue.enqueue(&data3));
    ASSERT_TRUE(queue.enqueue(&data4));
    ASSERT_FALSE(queue.enqueue(&data_error));

    BoundMpMcQueueTestData * temp_data1;
    BoundMpMcQueueTestData * temp_data2;
    BoundMpMcQueueTestData * temp_data3;
    BoundMpMcQueueTestData * temp_data4;
    BoundMpMcQueueTestData * temp_data_error;

    ASSERT_TRUE(queue.dequeue((void**)&temp_data1));
    ASSERT_TRUE(queue.dequeue((void**)&temp_data2));
    ASSERT_TRUE(queue.dequeue((void**)&temp_data3));
    ASSERT_TRUE(queue.dequeue((void**)&temp_data4));
    ASSERT_FALSE(queue.dequeue((void**)&temp_data_error));

    ASSERT_STREQ(TEST1_DATA, temp_data1->name);
    ASSERT_STREQ(TEST2_DATA, temp_data2->name);
    ASSERT_STREQ(TEST3_DATA, temp_data3->name);
    ASSERT_STREQ(TEST4_DATA, temp_data4->name);
}

