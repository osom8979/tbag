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

    std::vector<char> val1(10);
    std::vector<char> val2(10);
    std::vector<char> val3(10);
    std::vector<char> val4(10);
    std::vector<char> val5(10);

    MqType type1;
    MqType type2;
    MqType type3;
    MqType type4;
    MqType type5;

    std::size_t size1;
    std::size_t size2;
    std::size_t size3;
    std::size_t size4;
    std::size_t size5;

    ASSERT_EQ(Err::E_SUCCESS, queue.dequeue(val1.data(), val1.size(), &type1, &size1));
    ASSERT_EQ(MqType::MT_BOX_ADDRESS, type1);
    ASSERT_EQ(TEST1.size(), size1);
    ASSERT_TRUE(std::equal(TEST1.begin(), TEST1.end(), val1.begin(), val1.begin() + size1));
    ASSERT_EQ(3, queue.getInaccurateSizeOfActive());
    ASSERT_EQ(QUEUE_SIZE-3, queue.getInaccurateSizeOfReady());

    ASSERT_EQ(Err::E_SUCCESS, queue.dequeue(val2.data(), val2.size(), &type2, &size2));
    ASSERT_EQ(MqType::MT_BOX_ADDRESS, type2);
    ASSERT_EQ(TEST2.size(), size2);
    ASSERT_TRUE(std::equal(TEST2.begin(), TEST2.end(), val2.begin(), val2.begin() + size2));
    ASSERT_EQ(2, queue.getInaccurateSizeOfActive());
    ASSERT_EQ(QUEUE_SIZE-2, queue.getInaccurateSizeOfReady());

    ASSERT_EQ(Err::E_SUCCESS, queue.dequeue(val3.data(), val3.size(), &type3, &size3));
    ASSERT_EQ(MqType::MT_BOX_ADDRESS, type3);
    ASSERT_EQ(TEST3.size(), size3);
    ASSERT_TRUE(std::equal(TEST3.begin(), TEST3.end(), val3.begin(), val3.begin() + size3));
    ASSERT_EQ(1, queue.getInaccurateSizeOfActive());
    ASSERT_EQ(QUEUE_SIZE-1, queue.getInaccurateSizeOfReady());

    ASSERT_EQ(Err::E_SUCCESS, queue.dequeue(val4.data(), val4.size(), &type4, &size4));
    ASSERT_EQ(MqType::MT_BOX_ADDRESS, type4);
    ASSERT_EQ(TEST4.size(), size4);
    ASSERT_TRUE(std::equal(TEST4.begin(), TEST4.end(), val4.begin(), val4.begin() + size4));
    ASSERT_EQ(0, queue.getInaccurateSizeOfActive());
    ASSERT_EQ(QUEUE_SIZE, queue.getInaccurateSizeOfReady());

    ASSERT_EQ(Err::E_NREADY, queue.dequeue(val5.data(), val5.size(), &type5, &size5));
    ASSERT_EQ(0, queue.getInaccurateSizeOfActive());
    ASSERT_EQ(QUEUE_SIZE, queue.getInaccurateSizeOfReady());
}

