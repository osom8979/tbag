/**
 * @file   AsyncSingleRequestQueueTest.cpp
 * @brief  AsyncSingleRequestQueue class tester.
 * @author zer0
 * @date   2018-11-13
 */

#include <gtest/gtest.h>
#include <libtbag/uvpp/ex/AsyncSingleRequestQueue.hpp>

using namespace libtbag;
using namespace libtbag::uvpp;
using namespace libtbag::uvpp::ex;

struct AsyncSingleRequestQueueTest : public AsyncSingleRequestQueue<int, WriteRequest>
{
    using Base = AsyncSingleRequestQueue<int, WriteRequest>;

    int do_request = 0;
    int on_dequeue = 0;

    AsyncSingleRequestQueueTest(Loop & loop) : Base(loop)
    { /* EMPTY. */ }

    ~AsyncSingleRequestQueueTest()
    { /* EMPTY. */ }

    virtual void doRequest(Request & request, Value const & value) override
    {
        if (size() == 1) {
            do_request = value;
        } else {
            do_request = 0;
        }
        doneRequest();
    }

    virtual void onDequeue(Value const & value) override
    {
        if (empty()) {
            on_dequeue = value;
        } else {
            on_dequeue = 0;
        }
        close();
    }
};

TEST(AsyncSingleRequestQueueTest, Default)
{
    int const TEST_VALUE = 50;

    Loop loop;
    auto async = loop.newHandle<AsyncSingleRequestQueueTest>(loop);
    ASSERT_TRUE(async->empty());
    ASSERT_EQ(0, async->size());
    ASSERT_TRUE(async->isWaiting());

    async->enqueue(TEST_VALUE);
    ASSERT_FALSE(async->empty());
    ASSERT_EQ(1, async->size());
    ASSERT_FALSE(async->isWaiting());

    ASSERT_EQ(Err::E_SUCCESS, loop.run());
    ASSERT_EQ(TEST_VALUE, async->do_request);
    ASSERT_EQ(TEST_VALUE, async->on_dequeue);
}

