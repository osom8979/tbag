/**
 * @file   MqEventQueueTest.cpp
 * @brief  MqEventQueue class tester.
 * @author zer0
 * @date   2018-11-12
 * @date   2018-11-13 (Rename: MqSendQueue -> MqEventQueue)
 */

#include <gtest/gtest.h>
#include <libtbag/mq/details/MqEventQueue.hpp>
#include <libtbag/log/Log.hpp>

#include <cstring>
#include <string>
#include <thread>
#include <chrono>

#define USE_DELAY_ENQUEUE_TEST

using namespace libtbag;
using namespace libtbag::mq;
using namespace libtbag::mq::details;

struct MqSendQueueTest : public MqEventQueue
{
    TBAG_CONSTEXPR static char const * TEST_VALUE = "TEST";

    int send_success = 0;
    int send_failure = 0;
    int close_success = 0;
    int close_failure = 0;

    MqSendQueueTest(Loop & loop, std::size_t size) : MqEventQueue(loop, size)
    { /* EMPTY. */ }
    virtual ~MqSendQueueTest()
    { /* EMPTY. */ }

    virtual AfterAction onMsg(AsyncMsg * msg) override
    {
        if (msg->event == ME_MSG) {
            if (std::string(msg->begin(), msg->end()) == TEST_VALUE) {
                ++send_success;
            } else {
                ++send_failure;
            }
        } else {
            if (msg->event == ME_CLOSE) {
                ++close_success;
            } else {
                ++close_failure;
            }
            closeAsyncMsgs();
        }
        return AfterAction::AA_OK;
    }
};

TEST(MqEventQueueTest, Default)
{
    log::SeverityGuard guard(log::TBAG_DEFAULT_LOGGER_NAME, log::INFO_SEVERITY);
    std::size_t const MAX_QUEUE = 4;

    libtbag::uvpp::Loop loop;
    MqSendQueueTest queue(loop, MAX_QUEUE);
    ASSERT_EQ(MAX_QUEUE, queue.getInaccurateSizeOfReady());

    std::size_t success_count = 0;
    std::thread thread([&](){
#if defined(USE_DELAY_ENQUEUE_TEST)
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
#endif
        auto const * VAL = MqSendQueueTest::TEST_VALUE;
        std::size_t const SIZE = strlen(VAL);
        // @formatter:off
        if (isSuccess(queue.enqueue(VAL, SIZE))) { ++success_count; }
        if (isSuccess(queue.enqueue(VAL, SIZE))) { ++success_count; }
        if (isSuccess(queue.enqueue(VAL, SIZE))) { ++success_count; }
        if (isSuccess(queue.enqueueClose())) { ++success_count; }
        // @formatter:on
    });

    Err const CODE = loop.run();
    ASSERT_EQ(Err::E_SUCCESS, CODE);

    ASSERT_TRUE(thread.joinable());
    thread.join();

    ASSERT_EQ(4, success_count);
    ASSERT_EQ(3, queue.send_success);
    ASSERT_EQ(0, queue.send_failure);
    ASSERT_EQ(1, queue.close_success);
    ASSERT_EQ(0, queue.close_failure);
}

