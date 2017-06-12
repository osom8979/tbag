/**
 * @file   PoolAllocatorTest.cpp
 * @brief  PoolAllocator class tester.
 * @author zer0
 * @date   2017-06-10
 */

#include <gtest/gtest.h>
#include <libtbag/memory/PoolAllocator.hpp>
#include <libtbag/memory/pool/MemoryPool.hpp>
#include <libtbag/memory/pool/MultiMemoryPool.hpp>

#include <vector>

using namespace libtbag;
using namespace libtbag::memory;
using namespace libtbag::memory::pool;

TEST(PoolAllocatorTest, CopyConstructor)
{
    MemoryPool pool(10);

    using CharPool = PoolAllocator<char>;
    using ShortPool = PoolAllocator<short>;

    CharPool alloc1(&pool);
    ShortPool alloc2(alloc1);
    ASSERT_EQ(10, pool.left());

    std::vector<char, CharPool> buffer1(alloc1);
    buffer1.resize(1);
    ASSERT_EQ(9, pool.left());

    std::vector<short, ShortPool> buffer2(alloc2);
    buffer2.resize(1);
    ASSERT_EQ(7, pool.left());
}

TEST(PoolAllocatorTest, Default)
{
    MemoryPool pool;
    using TestPool = PoolAllocator<char>;
    TestPool alloc(&pool);

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
#if defined(TBAG_PLATFORM_WINDOWS)
    std::cout << "Skip this test in Windows Platform.\n";
#else
    MemoryPool pool(10);
    using TestPool = PoolAllocator<char>;
    TestPool alloc(&pool);

    std::size_t const TEST_SIZE = 10;
    std::vector<char, TestPool> buffer(alloc);
    buffer.assign(TEST_SIZE, 'A');

    ASSERT_THROW(buffer.push_back('A'), std::bad_alloc);
#endif
}

TEST(PoolAllocatorTest, Fragment)
{
    MemoryPool pool(10);
    using TestPool = PoolAllocator<char>;
    TestPool alloc(&pool);

    ASSERT_EQ(10, pool.left());
    {
        MpFragment f(pool);
        std::size_t const TEST_SIZE = 10;
        std::vector<char, TestPool> buffer(alloc);
        buffer.assign(TEST_SIZE, 'A');
    }
    ASSERT_EQ(10, pool.left());

    std::size_t const TEST_SIZE = 10;
    std::vector<char, TestPool> buffer(alloc);
    buffer.assign(TEST_SIZE, 'A');
}

TEST(PoolAllocatorTest, MultiMemoryPool)
{
    std::size_t const   SMALL_SIZE =  4;
    std::size_t const  NORMAL_SIZE =  8;
    std::size_t const   LARGE_SIZE = 16;
    std::size_t const OBJECT_COUNT =  1;

    MultiMemoryPool pool(SMALL_SIZE, NORMAL_SIZE, LARGE_SIZE, OBJECT_COUNT);
    using TestPool = PoolAllocator<char>;
    TestPool alloc(&pool);

    ASSERT_EQ( SMALL_SIZE * OBJECT_COUNT, pool.atSmall ().max());
    ASSERT_EQ(NORMAL_SIZE * OBJECT_COUNT, pool.atNormal().max());
    ASSERT_EQ( LARGE_SIZE * OBJECT_COUNT, pool.atLarge ().max());

    ASSERT_EQ( SMALL_SIZE * OBJECT_COUNT, pool.atSmall ().left());
    ASSERT_EQ(NORMAL_SIZE * OBJECT_COUNT, pool.atNormal().left());
    ASSERT_EQ( LARGE_SIZE * OBJECT_COUNT, pool.atLarge ().left());

    {
        MmpFragment fragment(pool);

        std::vector<char, TestPool> small_buffer(alloc);
        small_buffer.assign(SMALL_SIZE, 'A');
        std::vector<char, TestPool> normal_buffer(alloc);
        normal_buffer.assign(NORMAL_SIZE, 'A');
        std::vector<char, TestPool> large_buffer(alloc);
        large_buffer.assign(LARGE_SIZE, 'A');

        ASSERT_EQ( SMALL_SIZE, pool.atSmall ().size());
        ASSERT_EQ(NORMAL_SIZE, pool.atNormal().size());
        ASSERT_EQ( LARGE_SIZE, pool.atLarge ().size());
    }

    ASSERT_EQ(0, pool.atSmall ().size());
    ASSERT_EQ(0, pool.atNormal().size());
    ASSERT_EQ(0, pool.atLarge ().size());

    ASSERT_EQ( SMALL_SIZE * OBJECT_COUNT, pool.atSmall ().left());
    ASSERT_EQ(NORMAL_SIZE * OBJECT_COUNT, pool.atNormal().left());
    ASSERT_EQ( LARGE_SIZE * OBJECT_COUNT, pool.atLarge ().left());
}

