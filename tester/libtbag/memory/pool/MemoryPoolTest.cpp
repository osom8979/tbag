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

    {
        auto fragment = pool.save();
        std::size_t obtain_size = ALLOC_SIZE;
        void * ptr = pool.allocate(obtain_size, 0);
        ASSERT_NE(nullptr, ptr);
        ASSERT_EQ(ALLOC_SIZE, pool.max());
        ASSERT_EQ(obtain_size, pool.size());
        pool.deallocate(ptr, obtain_size);
    }
    ASSERT_EQ(ALLOC_SIZE, pool.left());

    {
        auto fragment = pool.save();
        std::size_t obtain_size = 1;
        void * ptr = pool.allocate(obtain_size, 0);
        ASSERT_NE(nullptr, ptr);
        ASSERT_EQ(reinterpret_cast<std::ptrdiff_t>(ptr), pool.begin());
        ASSERT_EQ(ALLOC_SIZE, pool.max());
        ASSERT_EQ(obtain_size, pool.size());
        ASSERT_EQ(ALLOC_SIZE - obtain_size, pool.left());
        pool.deallocate(ptr, obtain_size);
    }
    ASSERT_EQ(ALLOC_SIZE, pool.left());
}

