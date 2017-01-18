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

