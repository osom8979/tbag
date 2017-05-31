/**
 * @file   AsyncTest.cpp
 * @brief  Async libuv class tester.
 * @author zer0
 * @date   2017-01-06
 */

#include <gtest/gtest.h>
#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/uvpp/func/FunctionalAsync.hpp>
#include <libtbag/uvpp/func/FunctionalIdle.hpp>

#include <cassert>
#include <atomic>
#include <thread>

using namespace libtbag;
using namespace libtbag::uvpp;
using namespace libtbag::uvpp::func;

struct AsyncTest : public Async
{
    int max_async_count;
    int async_count;
    int close_count;

    AsyncTest(Loop & loop, int count) : Async(loop), max_async_count(count), async_count(0), close_count(0)
    { /* EMPTY. */ }

    virtual void onAsync() override
    {
        ++async_count;

        if (async_count >= max_async_count) {
            close();
        } else {
            send();
        }
    }

    virtual void onClose() override
    {
        ++close_count;
    }
};

TEST(AsyncTest, Default)
{
    int const TEST_COUNT = 5;

    std::shared_ptr<AsyncTest> async;
    std::shared_ptr<Loop> loop;

    loop.reset(new Loop());
    async = loop->newHandle<AsyncTest>(*loop, TEST_COUNT);

    ASSERT_EQ(1, loop->size());
    ASSERT_TRUE(static_cast<bool>(async));

    async->send();
    loop->run();

    ASSERT_EQ(0, loop->size());

    ASSERT_TRUE(static_cast<bool>(async));
    ASSERT_TRUE(async->isClosing());
    ASSERT_EQ(TEST_COUNT, async->async_count);
    ASSERT_EQ(1, async->close_count);
}

TEST(AsyncTest, Thread)
{
    int async_counter = 0;
    int close_counter = 0;

    Loop loop;
    auto async = loop.newHandle<FuncAsync>(loop);
    auto  idle = loop.newHandle<FuncIdle >(loop);

    std::atomic_bool is_idle;
    std::atomic_bool is_end;
    is_idle.store(false);
    is_end.store(false);

    async->setOnAsync([&](){
        ++async_counter;
        async->close();
    });
    async->setOnClose([&](){
        ++close_counter;
    });
    idle->setOnIdle([&](){
        is_idle.store(true);
        async->send();
        idle->close();
    });
    ASSERT_EQ(Err::E_SUCCESS, idle->start());

    Err loop_result = Err::E_UNKNOWN;
    std::thread thread([&](){
        loop_result = loop.run();
        is_end.store(true);
    });

    // BUSY WAITING...
    while (is_idle == false) {
        if (is_end) {
            assert(false && "loop error.");
            break;
        }
    }

    ASSERT_TRUE(thread.joinable());
    thread.join();

    ASSERT_EQ(Err::E_SUCCESS, loop_result);
    ASSERT_TRUE(is_idle.load());
    ASSERT_TRUE(is_end.load());

    ASSERT_EQ(1, async_counter);
    ASSERT_EQ(1, close_counter);
}


