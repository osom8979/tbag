/**
 * @file   RwLockTest.cpp
 * @brief  RwLock class tester.
 * @author zer0
 * @date   2016-05-12
 */

#include <gtest/gtest.h>
#include <libtbag/predef.hpp>
#include <libtbag/lock/RwLock.hpp>

#include <thread>
#include <chrono>
#include <atomic>
#include <vector>

using namespace libtbag;
using namespace libtbag::lock;

TEST(RwLockTest, Default)
{
    int const TEST_VALUE = 10000;
    int test_total = 0;
    int total_data = 0;

    std::vector<int> vector;

    for (int input = 0; input < TEST_VALUE; ++input) {
        vector.push_back(input);
        test_total += input;
    }

    int size_before[] = { 0, 0, 0, 0 };
    int size_after [] = { 0, 0, 0, 0 };

    int const INDEX0 = 0;
    int const INDEX1 = 1;
    int const INDEX2 = 2;
    int const INDEX3 = 3;

    RwLock lock;

    std::thread read_thread0 = std::thread([&](){
        ReadLockGuard guard(lock);
        size_before[INDEX0] = vector.size();
        int temp = 0;
        for (auto cursor : vector) {
            temp += cursor;
        }
        REMOVE_UNUSED_VARIABLE(temp);
        size_after[INDEX0] = vector.size();
    });

    std::thread read_thread1 = std::thread([&](){
        ReadLockGuard guard(lock);
        size_before[INDEX1] = vector.size();
        int temp = 0;
        for (auto cursor : vector) {
            temp += cursor;
        }
        REMOVE_UNUSED_VARIABLE(temp);
        size_after[INDEX1] = vector.size();
    });

    std::thread write_thread = std::thread([&](){
        WriteLockGuard guard(lock);
        vector.push_back(TEST_VALUE);
        size_before[INDEX2] = vector.size();
        for (auto cursor : vector) {
            total_data += cursor;
        }
        size_after[INDEX2] = vector.size();
    });

    std::thread read_thread3 = std::thread([&](){
        ReadLockGuard guard(lock);
        size_before[INDEX3] = vector.size();
        int temp = 0;
        for (auto cursor : vector) {
            temp += cursor;
        }
        REMOVE_UNUSED_VARIABLE(temp);
        size_after[INDEX3] = vector.size();
    });

    read_thread0.join();
    read_thread1.join();
    write_thread.join();
    read_thread3.join();

    ASSERT_EQ(size_before[0], size_after[0]);
    ASSERT_EQ(size_before[1], size_after[1]);
    ASSERT_EQ(size_before[2], size_after[2]);
    ASSERT_EQ(size_before[3], size_after[3]);

    ASSERT_EQ(total_data, test_total + TEST_VALUE);
}

