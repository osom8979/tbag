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
            assert(false && "loop error."); // <------ TRAVIS ERROR !!
//[ RUN      ]AsyncTest.Thread
//2018-09-12T01:28:53.322409851 @139789366672256 [DEBUG] Loop::insertChildHandle(@0x32fa000[UV_ASYNC]) Success (Handles: 1)
//2018-09-12T01:28:53.322443244 @139789366672256 [DEBUG] Loop::insertChildHandle(@0x32f07b0[UV_IDLE]) Success (Handles: 2)
//2018-09-12T01:28:53.353540433 @139788426045184 [DEBUG] Loop::eraseChildHandle(@0x32fa000[UV_ASYNC]) Success (Handles: 1)
//2018-09-12T01:28:53.353595076 @139788426045184 [DEBUG] Loop::eraseChildHandle(@0x32f07b0[UV_IDLE]) Success (Handles: 0)
//tester: /home/travis/build/osom8979/tbag/tester/libtbag/uvpp/AsyncTest.cpp:108: virtual void AsyncTest_Thread_Test::TestBody(): Assertion `false && "loop error."' failed.
//2018-09-12T01:28:53.361887378 @139789366672256 [ALERT] Abort signal. (SIGABRT):
// #00 0x00007F233D80B8ED libtbag::debug::getStackTraceString()+136 [libtbag.so.0]
// #01 0x00007F233DA36F2A libtbag::signal::__impl::DefaultAbortHandler::run()+86 [libtbag.so.0]
// #02 0x00007F233DA35E39 libtbag::signal::__impl::SignalObservable::notifyImplement()+1117 [libtbag.so.0]
// #03 0x00007F233DA360D2 libtbag::signal::__impl::SignalObservable::notify()+50 [libtbag.so.0]
// #04 0x00007F233DA33A7A +15551098 [libtbag.so.0]
// #05 0x00007F233BF98CB0 +224432 [/lib/x86_64-linux-gnu/libc.so.6]
// #06 0x00007F233BF98C37 gsignal+55 [/lib/x86_64-linux-gnu/libc.so.6]
// #07 0x00007F233BF9C028 abort+328 [/lib/x86_64-linux-gnu/libc.so.6]
// #08 0x00007F233BF91BF6 +195574 [/lib/x86_64-linux-gnu/libc.so.6]
// #09 0x00007F233BF91CA2 +195746 [/lib/x86_64-linux-gnu/libc.so.6]
// #10 0x0000000000E07B99 AsyncTest_Thread_Test::TestBody()+1691 [./tester]
// #11 0x0000000000E76DC4 testing::internal::HandleSehExceptionsInMethodIfSupported<>()+101 [./tester]
// #12 0x0000000000E73766 testing::internal::HandleExceptionsInMethodIfSupported<>()+75 [./tester]
// #13 0x0000000000E64FBA testing::Test::Run()+212 [./tester]
// #14 0x0000000000E6575A testing::TestInfo::Run()+260 [./tester]
// #15 0x0000000000E65DBD testing::TestCase::Run()+263 [./tester]
// #16 0x0000000000E6C443 testing::internal::UnitTestImpl::RunAllTests()+707 [./tester]
// #17 0x0000000000E77AEF testing::internal::HandleSehExceptionsInMethodIfSupported<>()+101 [./tester]
// #18 0x0000000000E74362 testing::internal::HandleExceptionsInMethodIfSupported<>()+75 [./tester]
// #19 0x0000000000E6B183 testing::UnitTest::Run()+183 [./tester]
// #20 0x0000000000E5F5B6 RUN_ALL_TESTS()+56 [./tester]
// #21 0x0000000000E5E8AA main+718 [./tester]
// #22 0x00007F233BF83F45 __libc_start_main+245 [/lib/x86_64-linux-gnu/libc.so.6]
// #23 0x000000000078D879 +0 [./tester]
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


