/**
 * @file   PoolAllocatorTest.cpp
 * @brief  PoolAllocator class tester.
 * @author zer0
 * @date   2017-06-10
 */

#include <gtest/gtest.h>
#include <libtbag/memory/PoolAllocator.hpp>
#include <libtbag/memory/pool/MemoryPool.hpp>

#include <vector>

using namespace libtbag;
using namespace libtbag::memory;
using namespace libtbag::memory::pool;

TEST(PoolAllocatorTest, Default)
{
    MemoryPool pool;
    using TestPool = PoolAllocator<char, MemoryPool>;
    TestPool alloc(pool);

    std::size_t const TEST_SIZE = 10;
    std::vector<char, TestPool> buffer(alloc);
    buffer.assign(TEST_SIZE, 'A');

    char const * MEMORY = static_cast<char const *>(pool.memory());
    std::string test1(MEMORY, MEMORY + TEST_SIZE);
    std::string test2(buffer.begin(), buffer.end());

    ASSERT_EQ(test1, test2);
}

TEST(PoolAllocatorTest, BadAlloc)
{
    MemoryPool pool(10);
    using TestPool = PoolAllocator<char, MemoryPool>;
    TestPool alloc(pool);

    std::size_t const TEST_SIZE = 10;
    std::vector<char, TestPool> buffer(alloc);
    buffer.assign(TEST_SIZE, 'A');

    ASSERT_THROW(buffer.push_back('A'), std::bad_alloc);
}

