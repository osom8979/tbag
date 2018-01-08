/**
 * @file   AlignedMemoryTest.cpp
 * @brief  AlignedMemory class tester.
 * @author zer0
 * @date   2018-01-08
 */

#include <gtest/gtest.h>
#include <libtbag/memory/AlignedMemory.hpp>

using namespace libtbag;
using namespace libtbag::memory;

TEST(AlignedMemoryTest, Size)
{
    ASSERT_EQ(0, alignedMemorySize() %  2);
    ASSERT_EQ(0, alignedMemorySize() % 16);
    ASSERT_EQ(0, alignedMemorySize() % sizeof(void*));
}

TEST(AlignedMemoryTest, Memory)
{
    void * data = alignedMemoryAlloc(1);
    ASSERT_NE(nullptr, data);
    alignedMemoryFree(data);
}

