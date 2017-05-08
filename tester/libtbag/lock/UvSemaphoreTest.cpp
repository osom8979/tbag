/**
 * @file   UvSemaphoreTest.cpp
 * @brief  UvSemaphore class tester.
 * @author zer0
 * @date   2017-05-03
 */

#include <gtest/gtest.h>
#include <libtbag/lock/UvSemaphore.hpp>

#include <thread>
#include <mutex>
#include <atomic>

using namespace libtbag;
using namespace libtbag::lock;

TEST(UvSemaphoreTest, Default)
{
    int const MAX_COUNTER = 3;
    UvSemaphore sem(MAX_COUNTER);
    std::atomic_int counter;
    counter.store(0);

    auto callback = [&](){
        std::lock_guard<UvSemaphore> guard(sem);
        ++counter;
        while (counter < MAX_COUNTER) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    };

    std::thread t1(callback);
    std::thread t2(callback);
    callback();

    t1.join();
    t2.join();

    ASSERT_EQ(MAX_COUNTER, counter.load());
}

