/**
 * @file   MqQueueTest.cpp
 * @brief  MqQueue class tester.
 * @author zer0
 * @date   2018-11-11
 */

#include <gtest/gtest.h>
#include <libtbag/mq/details/MqQueue.hpp>

#include <vector>

using namespace libtbag;
using namespace libtbag::mq;
using namespace libtbag::mq::details;

TEST(MqQueueTest, Default)
{
    int const QUEUE_SIZE = 4;
    MqQueue queue(QUEUE_SIZE);
    ASSERT_EQ(0, queue.getInaccurateSizeOfActive());
    ASSERT_EQ(QUEUE_SIZE, queue.getInaccurateSizeOfReady());

    std::string const TEST1 = "TEST1";
    std::string const TEST2 = "TEST2";
    std::string const TEST3 = "TEST3";
    std::string const TEST4 = "TEST4";
    std::string const TEST5 = "TEST5";

    ASSERT_EQ(Err::E_SUCCESS, queue.enqueue(TEST1.data(), TEST1.size()));
    ASSERT_EQ(1, queue.getInaccurateSizeOfActive());
    ASSERT_EQ(QUEUE_SIZE-1, queue.getInaccurateSizeOfReady());

    ASSERT_EQ(Err::E_SUCCESS, queue.enqueue(TEST2.data(), TEST2.size()));
    ASSERT_EQ(2, queue.getInaccurateSizeOfActive());
    ASSERT_EQ(QUEUE_SIZE-2, queue.getInaccurateSizeOfReady());

    ASSERT_EQ(Err::E_SUCCESS, queue.enqueue(TEST3.data(), TEST3.size()));
    ASSERT_EQ(3, queue.getInaccurateSizeOfActive());
    ASSERT_EQ(QUEUE_SIZE-3, queue.getInaccurateSizeOfReady());

    ASSERT_EQ(Err::E_SUCCESS, queue.enqueue(TEST4.data(), TEST4.size()));
    ASSERT_EQ(4, queue.getInaccurateSizeOfActive());
    ASSERT_EQ(QUEUE_SIZE-4, queue.getInaccurateSizeOfReady());

    ASSERT_EQ(Err::E_NREADY, queue.enqueue(TEST5.data(), TEST5.size()));
    ASSERT_EQ(4, queue.getInaccurateSizeOfActive());
    ASSERT_EQ(QUEUE_SIZE-4, queue.getInaccurateSizeOfReady());

    MqMsg msg1;
    MqMsg msg2;
    MqMsg msg3;
    MqMsg msg4;
    MqMsg msg5;

    ASSERT_EQ(Err::E_SUCCESS, queue.dequeue(msg1));
    ASSERT_EQ(MqEvent::ME_MSG, msg1.event);
    ASSERT_EQ(TEST1.size(), msg1.size());
    ASSERT_TRUE(std::equal(TEST1.begin(), TEST1.end(), msg1.begin(), msg1.end()));
    ASSERT_EQ(3, queue.getInaccurateSizeOfActive());
    ASSERT_EQ(QUEUE_SIZE-3, queue.getInaccurateSizeOfReady());

    ASSERT_EQ(Err::E_SUCCESS, queue.dequeue(msg2));
    ASSERT_EQ(MqEvent::ME_MSG, msg2.event);
    ASSERT_EQ(TEST2.size(), msg2.size());
    ASSERT_TRUE(std::equal(TEST2.begin(), TEST2.end(), msg2.begin(), msg2.end()));
    ASSERT_EQ(2, queue.getInaccurateSizeOfActive());
    ASSERT_EQ(QUEUE_SIZE-2, queue.getInaccurateSizeOfReady());

    ASSERT_EQ(Err::E_SUCCESS, queue.dequeue(msg3));
    ASSERT_EQ(MqEvent::ME_MSG, msg3.event);
    ASSERT_EQ(TEST3.size(), msg3.size());
    ASSERT_TRUE(std::equal(TEST3.begin(), TEST3.end(), msg3.begin(), msg3.end()));
    ASSERT_EQ(1, queue.getInaccurateSizeOfActive());
    ASSERT_EQ(QUEUE_SIZE-1, queue.getInaccurateSizeOfReady());

    ASSERT_EQ(Err::E_SUCCESS, queue.dequeue(msg4));
    ASSERT_EQ(MqEvent::ME_MSG, msg4.event);
    ASSERT_EQ(TEST4.size(), msg4.size());
    ASSERT_TRUE(std::equal(TEST4.begin(), TEST4.end(), msg4.begin(), msg4.end()));
    ASSERT_EQ(0, queue.getInaccurateSizeOfActive());
    ASSERT_EQ(QUEUE_SIZE, queue.getInaccurateSizeOfReady());

    ASSERT_EQ(Err::E_NREADY, queue.dequeue(msg5));
    ASSERT_EQ(0, queue.getInaccurateSizeOfActive());
    ASSERT_EQ(QUEUE_SIZE, queue.getInaccurateSizeOfReady());
}

