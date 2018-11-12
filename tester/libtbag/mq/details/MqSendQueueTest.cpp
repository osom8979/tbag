/**
 * @file   MqSendQueueTest.cpp
 * @brief  MqSendQueue class tester.
 * @author zer0
 * @date   2018-11-12
 */

#include <gtest/gtest.h>
#include <libtbag/mq/details/MqSendQueue.hpp>

#include <cstring>

using namespace libtbag;
using namespace libtbag::mq;
using namespace libtbag::mq::details;

struct MqSendQueueTest : public MqSendQueue
{
    TBAG_CONSTEXPR static char const * TEST_VALUE = "TEST";

    int async_success = 0;
    int async_failure = 0;
    int close_count   = 0;

    MqSendQueueTest(Loop & loop, std::size_t size) : MqSendQueue(loop, size)
    { /* EMPTY. */ }

    virtual ~MqSendQueueTest()
    { /* EMPTY. */ }

    virtual void onAsync(AsyncMsg * msg) override
    {
        if (MqType::MT_BOX_ADDRESS == msg->type &&
                msg->box.size() == strlen(TEST_VALUE) &&
                msg->box.toString() == TEST_VALUE) {
            ++async_success;
        } else {
            ++async_failure;
        }
        msg->close();
    }

    virtual void onClose(AsyncMsg * msg) override
    {
        ++close_count;
    }
};

TEST(MqSendQueueTest, Default)
{
    std::size_t const MAX_QUEUE = 4;

    libtbag::uvpp::Loop loop;
    MqSendQueueTest queue(loop, MAX_QUEUE);
    ASSERT_EQ(MAX_QUEUE, queue.getInaccurateSizeOfReady());
    ASSERT_EQ(Err::E_SUCCESS, queue.enqueue(MqSendQueueTest::TEST_VALUE, strlen(MqSendQueueTest::TEST_VALUE)));
    ASSERT_EQ(Err::E_SUCCESS, queue.enqueue(MqSendQueueTest::TEST_VALUE, strlen(MqSendQueueTest::TEST_VALUE)));
    ASSERT_EQ(Err::E_SUCCESS, queue.enqueue(MqSendQueueTest::TEST_VALUE, strlen(MqSendQueueTest::TEST_VALUE)));
    ASSERT_EQ(Err::E_SUCCESS, queue.enqueue(MqSendQueueTest::TEST_VALUE, strlen(MqSendQueueTest::TEST_VALUE)));
    ASSERT_EQ(0, queue.getInaccurateSizeOfReady());

    Err loop_result = Err::E_UNKNOWN;
    std::thread thread([&](){
        loop_result = loop.run();
    });

    ASSERT_TRUE(thread.joinable());
    thread.join();

    ASSERT_EQ(Err::E_SUCCESS, loop_result);

    ASSERT_EQ(MAX_QUEUE, queue.async_success);
    ASSERT_EQ(0, queue.async_failure);
    ASSERT_EQ(MAX_QUEUE, queue.close_count);
}

