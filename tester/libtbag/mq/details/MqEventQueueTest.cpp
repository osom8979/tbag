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
    int delay_close_error = 0;

    MqSendQueueTest(Loop & loop, std::size_t size) : MqEventQueue(loop, size)
    { /* EMPTY. */ }
    virtual ~MqSendQueueTest()
    { /* EMPTY. */ }

    virtual void onEvent(MqEvent event, char const * data, std::size_t size) override
    {
        if (event == MqEvent::ME_CLOSE && isWatingSender()) {
            this->closeAll();
        } else {
            if (isFailure(enqueueClose())) {
                ++delay_close_error;
            }
        }
    }

    virtual void onSend(char const * data, std::size_t size) override
    {
        std::string const RESULT(data, data + size);
        if (RESULT == TEST_VALUE) {
            ++send_success;
        } else {
            ++send_failure;
        }
        doneWrite(Err::E_SUCCESS);
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
        if (isSuccess(queue.enqueueClose())) { ++success_count; }
        // @formatter:on
    });

    Err const CODE = loop.run();
    ASSERT_EQ(Err::E_SUCCESS, CODE);

    ASSERT_TRUE(thread.joinable());
    thread.join();

    ASSERT_EQ(3, success_count);
    ASSERT_EQ(2, queue.send_success);
    ASSERT_EQ(0, queue.send_failure);
    ASSERT_EQ(0, queue.delay_close_error);
}

