/**
 * @file   MemoryPoolTest.cpp
 * @brief  MemoryPool class tester.
 * @author zer0
 * @date   2017-06-10
 */

#include <gtest/gtest.h>
#include <libtbag/memory/pool/MemoryPool.hpp>

using namespace libtbag;
using namespace libtbag::memory;
using namespace libtbag::memory::pool;

TEST(MemoryPoolTest, Default)
{
    std::size_t const ALLOC_SIZE = 4;
    MemoryPool pool(ALLOC_SIZE);
    ASSERT_NE(nullptr, pool.memory());
    ASSERT_EQ(ALLOC_SIZE, pool.max());
    ASSERT_EQ(0, pool.size());

    void * ptr = pool.allocate(ALLOC_SIZE, 0);
    ASSERT_NE(nullptr, ptr);
    ASSERT_EQ(ALLOC_SIZE, pool.max());
    ASSERT_EQ(ALLOC_SIZE, pool.size());

    pool.deallocate(ptr, ALLOC_SIZE);
}

