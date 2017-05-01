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
//    std::shared_ptr<SafetyAsync> async;
//    std::shared_ptr<Loop> loop;
//
//    loop.reset(new Loop());
//    async = loop->newHandle<SafetyAsync>(*loop);
//
//    ASSERT_EQ(2/* Async & Idle */, loop->size());
//    ASSERT_TRUE(static_cast<bool>(async));
//
//    std::thread thread = std::thread([&loop](){
//        loop->run();
//    });
//
//    std::atomic_int counter;
//    counter.store(0);
//
//    int const TEST_COUNT = 1000;
//    for (int i = 0; i < TEST_COUNT; ++i) {
//        auto shared = async->newPushFunc([&counter](SafetyAsync * async){ counter++; });
//        ASSERT_TRUE(static_cast<bool>(shared));
//    }
//
//    while (counter.load() != TEST_COUNT) {
//        /* BUSY WAIT. */
//    }
//    ASSERT_EQ(0, async->size());
//
//    async->pushCloseJob();
//    thread.join();
//
//    ASSERT_EQ(0, loop->size());
//    ASSERT_TRUE(static_cast<bool>(async));
//    ASSERT_TRUE(async->isClosing());
}

