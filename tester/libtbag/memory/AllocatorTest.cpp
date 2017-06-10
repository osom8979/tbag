/**
 * @file   AllocatorTest.cpp
 * @brief  Allocator class tester.
 * @author zer0
 * @date   2017-02-14
 */

#include <gtest/gtest.h>
#include <libtbag/memory/Allocator.hpp>
#include <iostream>

using namespace libtbag;
using namespace libtbag::memory;

struct AllocTestObject
{
    AllocTestObject()
    {
        std::cout << "AllocTestObject()\n";
    }

    ~AllocTestObject()
    {
        std::cout << "~AllocTestObject()\n";
    }
};

TEST(AllocatorTest, Memroy1)
{
    Allocator<int> allocator;
    int const TEST_VALUE = 0xFF;

    int * data = allocator.allocate(1);
    {
        *data = TEST_VALUE;
        ASSERT_EQ(TEST_VALUE, *data);
    }
    allocator.deallocate(data, 1);
}

TEST(AllocatorTest, Memroy2)
{
    Allocator<AllocTestObject> allocator;
    AllocTestObject * data = allocator.allocate(1);
    {
        ASSERT_EQ(data, allocator.address(*data));
        ASSERT_LT(0, allocator.max_size());

        allocator.construct(data);
        allocator.destroy(data);
    }
    allocator.deallocate(data, 1);
}

