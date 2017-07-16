/**
 * @file   TripleIoIndexTest.cpp
 * @brief  TripleIoIndex class tester.
 * @author zer0
 * @date   2017-07-14
 */

#include <gtest/gtest.h>
#include <libtbag/lockfree/TripleIoIndex.hpp>

#include <array>
#include <vector>
#include <thread>
#include <atomic>
#include <mutex>

#define DISABLE_MASSIVE_TEST

#if defined(DISABLE_MASSIVE_TEST)
TBAG_CONSTEXPR static std::size_t const LOOP_COUNT = 10000;
#else
TBAG_CONSTEXPR static std::size_t const LOOP_COUNT = 20000000;
#endif
TBAG_CONSTEXPR static std::size_t const BUFFER_SIZE = 512;

using namespace libtbag;
using namespace libtbag::lockfree;

using Buffer = std::vector<std::size_t>;
using BufferArray = std::array<Buffer, TripleIoIndex::SIZE>;

struct TripleIoIndexLock
{
    using Mutex = std::mutex;
    using Guard = std::lock_guard<Mutex>;

    Mutex mutable lock;

    TBAG_CONSTEXPR static std::size_t size() TBAG_NOEXCEPT { return 3; }

    int input  = 0;
    int output = 1;
    int ready  = 2;

    bool update = false;

    int nextInputValue()
    {
        Guard g(lock);
        algorithm::swapWithPod(input, ready);
        update = true;
        return input;
    }

    int nextOutputValue()
    {
        Guard g(lock);
        if (update == false) {
            return output;
        }
        algorithm::swapWithPod(output, ready);
        update = false;
        return output;
    }
};

TEST(TripleIoIndexTest, AtomicTest)
{
    std::atomic_int test(100);
    ASSERT_TRUE(test.is_lock_free());

    int expected1 = 200;
    int desired1  = 300;

    int expected2 = 100;
    int desired2  = 400;

    ASSERT_FALSE(test.compare_exchange_strong(expected1, desired1));
    ASSERT_EQ(100, expected1); // Exchange original value.
    ASSERT_EQ(300, desired1);
    ASSERT_EQ(100, test.load());

    ASSERT_TRUE(test.compare_exchange_strong(expected2, desired2));
    ASSERT_EQ(100, expected2);
    ASSERT_EQ(400, desired2);
    ASSERT_EQ(400, test.load());
}

TEST(TripleIoIndexTest, BitwiseOperations)
{
    using Info = TripleIoIndex::Info;

    ASSERT_EQ(0x00, TripleIoIndex::createByte(0, false));
    ASSERT_EQ(0x80, TripleIoIndex::createByte(0, true));
    ASSERT_EQ(0x01, TripleIoIndex::createByte(1, false));
    ASSERT_EQ(0x81, TripleIoIndex::createByte(1, true));
    ASSERT_EQ(0x02, TripleIoIndex::createByte(2, false));
    ASSERT_EQ(0x82, TripleIoIndex::createByte(2, true));

    Info info = TripleIoIndex::createInfo();
    ASSERT_FALSE(TripleIoIndex::isUpdate(info.array[TripleIoIndex::INPUT_INDEX]));
    ASSERT_EQ(0, TripleIoIndex::getValue(info.array[TripleIoIndex::INPUT_INDEX]));
    ASSERT_FALSE(TripleIoIndex::isUpdate(info.array[TripleIoIndex::OUTPUT_INDEX]));
    ASSERT_EQ(1, TripleIoIndex::getValue(info.array[TripleIoIndex::OUTPUT_INDEX]));
    ASSERT_FALSE(TripleIoIndex::isUpdate(info.array[TripleIoIndex::READY_INDEX]));
    ASSERT_EQ(2, TripleIoIndex::getValue(info.array[TripleIoIndex::READY_INDEX]));

    TripleIoIndex::updateInput(info, true);
    ASSERT_TRUE (TripleIoIndex::isUpdate(info.array[TripleIoIndex::INPUT_INDEX]));
    ASSERT_EQ(0, TripleIoIndex::getValue(info.array[TripleIoIndex::INPUT_INDEX]));

    TripleIoIndex::updateOutput(info, true);
    ASSERT_TRUE (TripleIoIndex::isUpdate(info.array[TripleIoIndex::OUTPUT_INDEX]));
    ASSERT_EQ(1, TripleIoIndex::getValue(info.array[TripleIoIndex::OUTPUT_INDEX]));

    TripleIoIndex::swapInputAndReady(info);
    ASSERT_TRUE (TripleIoIndex::isUpdate(info.array[TripleIoIndex::READY_INDEX]));
    ASSERT_EQ(0, TripleIoIndex::getValue(info.array[TripleIoIndex::READY_INDEX]));
    ASSERT_TRUE (TripleIoIndex::isUpdate(info.array[TripleIoIndex::OUTPUT_INDEX]));
    ASSERT_EQ(1, TripleIoIndex::getValue(info.array[TripleIoIndex::OUTPUT_INDEX]));
    ASSERT_FALSE(TripleIoIndex::isUpdate(info.array[TripleIoIndex::INPUT_INDEX]));
    ASSERT_EQ(2, TripleIoIndex::getValue(info.array[TripleIoIndex::INPUT_INDEX]));

    TripleIoIndex::swapOutputAndReady(info);
    ASSERT_TRUE (TripleIoIndex::isUpdate(info.array[TripleIoIndex::OUTPUT_INDEX]));
    ASSERT_EQ(0, TripleIoIndex::getValue(info.array[TripleIoIndex::OUTPUT_INDEX]));
    ASSERT_TRUE (TripleIoIndex::isUpdate(info.array[TripleIoIndex::READY_INDEX]));
    ASSERT_EQ(1, TripleIoIndex::getValue(info.array[TripleIoIndex::READY_INDEX]));
    ASSERT_FALSE(TripleIoIndex::isUpdate(info.array[TripleIoIndex::INPUT_INDEX]));
    ASSERT_EQ(2, TripleIoIndex::getValue(info.array[TripleIoIndex::INPUT_INDEX]));

    uint8_t const  INPUT_BYTE = TripleIoIndex:: getInputByte(info);
    uint8_t const OUTPUT_BYTE = TripleIoIndex::getOutputByte(info);
    uint8_t const  READY_BYTE = TripleIoIndex:: getReadyByte(info);
    ASSERT_EQ( INPUT_BYTE, info.array[TripleIoIndex:: INPUT_INDEX]);
    ASSERT_EQ(OUTPUT_BYTE, info.array[TripleIoIndex::OUTPUT_INDEX]);
    ASSERT_EQ( READY_BYTE, info.array[TripleIoIndex:: READY_INDEX]);
}

template <typename IndexType>
static void runBenchmark(IndexType & index)
{
    BufferArray buffers;
    ASSERT_EQ(index.size(), buffers.size());
    for (auto & b : buffers) {
        b.assign(BUFFER_SIZE, 0x00);
    }

    std::atomic_bool is_error(false);

    std::thread input_thread([&](){
        int input_index = index.nextInputValue();
        for (std::size_t i = 0; i < LOOP_COUNT; ++i) {
            for (auto & c : buffers[input_index]) {
                if (is_error) { break; }
                c = i;
            }
            if (is_error) { break; }
            input_index = index.nextInputValue();
        }
    });

    std::thread output_thread([&](){
        int output_index = index.nextOutputValue();
        for (std::size_t i = 0; i < LOOP_COUNT; ++i) {
            auto temp = buffers[output_index];
            ASSERT_LT(0, temp.size());

            std::size_t const CHECK_VALUE = temp[0];
            for (auto & c : temp) {
                if (CHECK_VALUE != c) {
                    is_error = true;
                    std::cerr << "Error loop: " << i << std::endl;
                }
                ASSERT_EQ(CHECK_VALUE, c);
            }

            output_index = index.nextOutputValue();
        }
    });

    ASSERT_TRUE(input_thread.joinable());
    ASSERT_TRUE(output_thread.joinable());

    input_thread.join();
    output_thread.join();
}

TEST(TripleIoIndexTest, BenchmarkOfAtomic)
{
    TripleIoIndex index;
    runBenchmark(index);
}

TEST(TripleIoIndexTest, BenchmarkOfMutex)
{
    TripleIoIndexLock index;
    runBenchmark(index);
}

