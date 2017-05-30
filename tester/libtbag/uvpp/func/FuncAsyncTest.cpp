/**
 * @file   FuncAsyncTest.cpp
 * @brief  FuncAsync class tester.
 * @author zer0
 * @date   2017-05-30
 */

#include <gtest/gtest.h>
#include <libtbag/uvpp/func/FuncAsync.hpp>
#include <libtbag/uvpp/func/FuncIdle.hpp>
#include <libtbag/uvpp/Loop.hpp>

#include <cassert>
#include <atomic>
#include <thread>

using namespace libtbag;
using namespace libtbag::uvpp;
using namespace libtbag::uvpp::func;

TEST(FuncAsyncTest, Default)
{
    int async_counter = 0;
    int close_counter = 0;

    using DefaultFuncAsync = FuncAsync<Async>;
    using DefaultFuncIdle  = FuncIdle<Idle>;

    Loop loop;
    auto async = loop.newHandle<DefaultFuncAsync>(loop);
    auto  idle = loop.newHandle<DefaultFuncIdle >(loop);

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

