/**
 * @file   BoundedMpMcQueueTest.cpp
 * @brief  BoundedMpMcQueue class tester.
 * @author zer0
 * @date   2018-11-09
 */

#include <gtest/gtest.h>
#include <libtbag/lockfree/BoundedMpMcQueue.hpp>

using namespace libtbag;
using namespace libtbag::lockfree;

struct BoundedMpMcQueueTestData
{
    char name[64] = {0,};
};

TEST(BoundedMpMcQueueTest, Default)
{
    char const * TEST1_DATA = "TEST1";
    char const * TEST2_DATA = "TEST2";
    char const * TEST3_DATA = "TEST3";
    char const * TEST4_DATA = "TEST4";
    char const * TEST_ERROR = "ERROR";

    BoundedMpMcQueueTestData data1;
    BoundedMpMcQueueTestData data2;
    BoundedMpMcQueueTestData data3;
    BoundedMpMcQueueTestData data4;
    BoundedMpMcQueueTestData data_error;

    ASSERT_NE(nullptr, strcpy(data1.name, TEST1_DATA));
    ASSERT_NE(nullptr, strcpy(data2.name, TEST2_DATA));
    ASSERT_NE(nullptr, strcpy(data3.name, TEST3_DATA));
    ASSERT_NE(nullptr, strcpy(data4.name, TEST4_DATA));
    ASSERT_NE(nullptr, strcpy(data_error.name, TEST_ERROR));

    BoundedMpMcQueue queue(4);
    ASSERT_TRUE(queue.enqueueVal(&data1));
    ASSERT_TRUE(queue.enqueueVal(&data2));
    ASSERT_TRUE(queue.enqueueVal(&data3));
    ASSERT_TRUE(queue.enqueueVal(&data4));
    ASSERT_FALSE(queue.enqueueVal(&data_error));

    BoundedMpMcQueueTestData * temp_data1;
    BoundedMpMcQueueTestData * temp_data2;
    BoundedMpMcQueueTestData * temp_data3;
    BoundedMpMcQueueTestData * temp_data4;
    BoundedMpMcQueueTestData * temp_data_error;

    ASSERT_TRUE(queue.dequeueVal((void**)&temp_data1));
    ASSERT_TRUE(queue.dequeueVal((void**)&temp_data2));
    ASSERT_TRUE(queue.dequeueVal((void**)&temp_data3));
    ASSERT_TRUE(queue.dequeueVal((void**)&temp_data4));
    ASSERT_FALSE(queue.dequeueVal((void**)&temp_data_error));

    ASSERT_STREQ(TEST1_DATA, temp_data1->name);
    ASSERT_STREQ(TEST2_DATA, temp_data2->name);
    ASSERT_STREQ(TEST3_DATA, temp_data3->name);
    ASSERT_STREQ(TEST4_DATA, temp_data4->name);
}

