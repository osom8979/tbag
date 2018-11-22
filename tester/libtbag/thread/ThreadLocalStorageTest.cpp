/**
 * @file   ThreadLocalStorageTest.cpp
 * @brief  ThreadLocalStorage class tester.
 * @author zer0
 * @date   2018-11-22
 */

#include <gtest/gtest.h>
#include <libtbag/thread/ThreadLocalStorage.hpp>

#include <thread>

using namespace libtbag;
using namespace libtbag::thread;

TEST(ThreadLocalStorageTest, Default)
{
    int const TEST_VAL = 100;
    ThreadLocalStorage tls;
    tls.set(new int(TEST_VAL));
    auto * result = tls.cast<int>();
    ASSERT_EQ(TEST_VAL, *result);
    delete result;
}

TEST(ThreadLocalStorageTest, Thread)
{
    int const TEST1_VAL = 100;
    int const TEST2_VAL = 200;

    int * result1 = nullptr;
    int * result2 = nullptr;

    ThreadLocalStorage tls;

    auto thread1 = std::thread([&](){
        tls.set(new int(TEST1_VAL));
        result1 = tls.cast<int>();
    });
    auto thread2 = std::thread([&](){
        tls.set(new int(TEST2_VAL));
        result2 = tls.cast<int>();
    });

    thread1.join();
    thread2.join();

    ASSERT_EQ(TEST1_VAL, *result1);
    ASSERT_EQ(TEST2_VAL, *result2);

    delete result1;
    delete result2;
}

