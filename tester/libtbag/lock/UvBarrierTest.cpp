/**
 * @file   UvBarrierTest.cpp
 * @brief  UvBarrier class tester.
 * @author zer0
 * @date   2017-05-18
 */

#include <gtest/gtest.h>
#include <libtbag/lock/UvBarrier.hpp>

#include <atomic>
#include <thread>
#include <vector>

using namespace libtbag;
using namespace libtbag::lock;

TEST(UvBarrierTest, Default)
{
    int const THREAD_COUNT = 100;

    UvBarrier barrier(THREAD_COUNT);
    std::atomic_int counter;
    counter.store(0);

    auto shared_routine = [&](){
        ++counter;
        barrier.wait();
    };

    std::vector<std::thread> threads(THREAD_COUNT);
    std::vector<int> result(THREAD_COUNT);

    for (int i = 0; i < THREAD_COUNT; ++i) {
        threads[i] = std::thread([&, i](){
            shared_routine();
            result[i] = counter;
        });
    }

    for (auto & thread : threads) {
        thread.join();
    }
    for (auto & cursor : result) {
        ASSERT_EQ(THREAD_COUNT, cursor);
    }
}

