/**
 * @file   SafetyAsyncTest.cpp
 * @brief  SafetyAsync class tester.
 * @author zer0
 * @date   2017-05-01
 */

#include <gtest/gtest.h>
#include <libtbag/uvpp/ex/SafetyAsync.hpp>
#include <libtbag/uvpp/Loop.hpp>

#include <atomic>

using namespace libtbag;
using namespace libtbag::uvpp;
using namespace libtbag::uvpp::ex;

TEST(SafetyAsyncTest, Default)
{
    Loop loop;
    auto async = loop.newHandle<SafetyAsync>(loop);

    ASSERT_EQ(2/* Async & Idle */, loop.size());
    ASSERT_TRUE(static_cast<bool>(async));

    std::atomic_bool is_end;
    is_end.store(false);

    std::atomic_int async_counter;
    async_counter.store(0);

    Err loop_result = Err::E_UNKNOWN;
    std::thread thread = std::thread([&](){
        loop_result = loop.run();
        is_end = true;
    });

    int const TEST_COUNT = 5;
    for (int i = 0; i < TEST_COUNT; ++i) {
        auto shared = async->newSendFunc([&](){
            async_counter++;
        });
        ASSERT_TRUE(static_cast<bool>(shared));
    }

    while (async_counter.load() != TEST_COUNT) {
        /* BUSY WAIT. */
    }
    ASSERT_EQ(0, async->size());

    async->sendClose();
    thread.join();

    ASSERT_TRUE(is_end.load());
    ASSERT_EQ(0, loop.size());
    ASSERT_EQ(TEST_COUNT, async_counter.load());
    ASSERT_EQ(Err::E_SUCCESS, loop_result);

    ASSERT_TRUE(static_cast<bool>(async));
    ASSERT_TRUE(async->isClosing());
}

