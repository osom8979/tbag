/**
 * @file   MemoryPoolTest.cpp
 * @brief  MemoryPool class tester.
 * @author zer0
 * @date   2017-04-16
 */

#include <gtest/gtest.h>
#include <libtbag/memory/MemoryPool.hpp>

using namespace libtbag;
using namespace libtbag::memory;

TEST(MemoryPoolTest, Default)
{
    std::size_t const ALLOC_SIZE = 4;
    MemoryPool pool(ALLOC_SIZE);
    ASSERT_NE(nullptr, pool.memory());
    ASSERT_EQ(ALLOC_SIZE, pool.max());
    ASSERT_EQ(0, pool.cursor());

    void * ptr = pool.allocate(ALLOC_SIZE, 0);
    ASSERT_NE(nullptr, ptr);
    ASSERT_EQ(ALLOC_SIZE, pool.max());
    ASSERT_EQ(ALLOC_SIZE, pool.cursor());

    pool.deallocate(ptr, ALLOC_SIZE);
}

