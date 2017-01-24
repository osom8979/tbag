/**
 * @file   AsyncTest.cpp
 * @brief  Async libuv class tester.
 * @author zer0
 * @date   2017-01-06
 */

#include <gtest/gtest.h>
#include <libtbag/uv/Loop.hpp>
#include <libtbag/uv/Async.hpp>

#include <atomic>
#include <thread>

using namespace libtbag;
using namespace libtbag::uv;

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

struct AsyncThreadTest : public Async
{
    std::atomic_int async_count;
    std::atomic_int close_count;

    std::atomic_bool close_flag;

    AsyncThreadTest(Loop & loop) : Async(loop), async_count(0), close_count(0), close_flag(false)
    { /* EMPTY. */ }

    virtual void onAsync() override
    {
        if (close_flag) {
            close();
        } else {
            ++async_count;
        }
    }

    virtual void onClose() override
    {
        ++close_count;
    }
};

TEST(AsyncTest, Thread)
{
    std::shared_ptr<AsyncThreadTest> async;
    std::shared_ptr<Loop> loop;

    loop.reset(new Loop());
    async = loop->newHandle<AsyncThreadTest>(*loop);

    ASSERT_EQ(1, loop->size());
    ASSERT_TRUE(static_cast<bool>(async));

    std::thread thread = std::thread([&loop](){
        loop->run();
    });

    ASSERT_TRUE(async->send());
    while (async->async_count.load() == 0) { /* BUSY WAIT. */ }
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    ASSERT_EQ(1, async->async_count);
    ASSERT_EQ(0, async->close_count);

    ASSERT_TRUE(async->send());
    while (async->async_count.load() == 1) { /* BUSY WAIT. */ }
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    ASSERT_EQ(2, async->async_count);
    ASSERT_EQ(0, async->close_count);

    async->close_flag = true;
    ASSERT_TRUE(async->send());
    while (async->close_count.load() == 0) { /* BUSY WAIT. */ }
    ASSERT_EQ(2, async->async_count);
    ASSERT_EQ(1, async->close_count);

    thread.join();

    ASSERT_EQ(0, loop->size());
    ASSERT_TRUE(static_cast<bool>(async));
    ASSERT_TRUE(async->isClosing());
}

struct AsyncJobTest : public Async::Job
{
    std::atomic_int async_count;

    AsyncJobTest() : async_count(0)
    {
        // EMPTY.
    }

    virtual void run(Async * handle)
    {
        ++async_count;
    }
};

TEST(AsyncTest, newJob)
{
    std::shared_ptr<Async> async;
    std::shared_ptr<Loop> loop;

    loop.reset(new Loop());
    async = loop->newHandle<Async>(*loop);

    ASSERT_EQ(1, loop->size());
    ASSERT_TRUE(static_cast<bool>(async));

    std::thread thread = std::thread([&loop](){
        loop->run();
    });

    auto shared = async->safeNewJob<AsyncJobTest>();
    ASSERT_EQ(1, async->size());
    ASSERT_TRUE(async->send());
    while (shared->async_count.load() == 0) { /* BUSY WAIT. */ }
    ASSERT_EQ(1, shared->async_count);
    ASSERT_EQ(0, async->size());

    shared.reset();
    shared = async->safeNewJob<AsyncJobTest>();
    ASSERT_EQ(1, async->size());
    ASSERT_TRUE(async->send());
    while (shared->async_count.load() == 0) { /* BUSY WAIT. */ }
    ASSERT_EQ(1, shared->async_count);
    ASSERT_EQ(0, async->size());

    ASSERT_TRUE(async->safeClose());
    thread.join();

    ASSERT_EQ(0, loop->size());
    ASSERT_TRUE(static_cast<bool>(async));
    ASSERT_TRUE(async->isClosing());
}

