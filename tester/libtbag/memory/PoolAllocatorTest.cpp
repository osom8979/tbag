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
    using TestPool = PoolAllocator<char>;
    TestPool alloc(&pool);

    std::size_t const TEST_SIZE = 10;
    std::vector<char, TestPool> buffer(alloc);

    // [WARNING] MSVC allocates std::_Container_proxy more.
    std::size_t const MORE_ALLOC_SIZE = pool.size();

    buffer.assign(TEST_SIZE, 'A');

    char const * MEMORY = static_cast<char const *>(pool.memory());
    std::string test1(MEMORY + MORE_ALLOC_SIZE, MEMORY + MORE_ALLOC_SIZE + TEST_SIZE);
    std::string test2(buffer.begin(), buffer.end());

    ASSERT_EQ(test1, test2);
}

TEST(PoolAllocatorTest, Fragment)
{
    std::size_t const TEST_SIZE = 64;

    MemoryPool pool(TEST_SIZE);
    using TestPool = PoolAllocator<char>;
    TestPool alloc(&pool);

    ASSERT_EQ(TEST_SIZE, pool.left());
    {
        auto fragment = pool.save();
        std::vector<char, TestPool> buffer1(alloc);
        buffer1.push_back('A');
        buffer1.push_back('A');

        std::vector<char, TestPool> buffer2(alloc);
        buffer2.push_back('A');
        buffer2.push_back('A');
        ASSERT_NE(TEST_SIZE, pool.left());
    }
    ASSERT_EQ(TEST_SIZE, pool.left());
}

TEST(PoolAllocatorTest, CopyConstructor)
{
    std::size_t TEST_MEM_SIZE = 100;
    MemoryPool pool(TEST_MEM_SIZE);

    using CharPool = PoolAllocator<char>;
    using ShortPool = PoolAllocator<short>;

    CharPool alloc1(&pool);
    ShortPool alloc2(alloc1);
    ASSERT_EQ(TEST_MEM_SIZE, pool.left());

    {
        auto fragment = pool.save();
        std::vector<char, CharPool> buffer1(alloc1);
        // [WARNING] MSVC allocates std::_Container_proxy more.
        std::size_t const MORE_ALLOC_SIZE = pool.size();
        buffer1.resize(1);
        ASSERT_EQ(TEST_MEM_SIZE - MORE_ALLOC_SIZE - sizeof(char), pool.left());
    }
    ASSERT_EQ(TEST_MEM_SIZE, pool.left());

    {
        auto fragment = pool.save();
        std::vector<short, ShortPool> buffer2(alloc2);
        // [WARNING] MSVC allocates std::_Container_proxy more.
        std::size_t const MORE_ALLOC_SIZE = pool.size();
        buffer2.resize(1);
        ASSERT_EQ(TEST_MEM_SIZE - MORE_ALLOC_SIZE - sizeof(short), pool.left());
    }
    ASSERT_EQ(TEST_MEM_SIZE, pool.left());
}

TEST(PoolAllocatorTest, BadAlloc)
{
    std::size_t const TEST_SIZE = 32;
    MemoryPool pool(TEST_SIZE);
    using TestPool = PoolAllocator<char>;
    TestPool alloc(&pool);

    std::vector<char, TestPool> buffer(alloc);

    // [WARNING] MSVC allocates std::_Container_proxy more.
    std::size_t const MORE_ALLOC_SIZE = pool.size();

    buffer.assign(TEST_SIZE - MORE_ALLOC_SIZE, 'A');

    ASSERT_THROW(buffer.push_back('A'), std::bad_alloc);
}

