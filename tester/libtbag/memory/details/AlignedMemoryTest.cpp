/**
 * @file   AlignedMemoryTest.cpp
 * @brief  AlignedMemory class tester.
 * @author zer0
 * @date   2018-01-08
 * @date   2019-03-03 (Change namespace: libtbag::memory -> libtbag::memory::details)
 */

#include <gtest/gtest.h>
#include <libtbag/memory/details/AlignedMemory.hpp>

using namespace libtbag;
using namespace libtbag::memory;
using namespace libtbag::memory::details;

TEST(AlignedMemoryTest, Size)
{
    ASSERT_EQ(0, getDefaultAlignedSize() %  2);
    ASSERT_EQ(0, getDefaultAlignedSize() % 16);
    ASSERT_EQ(0, getDefaultAlignedSize() % sizeof(void*));
}

TEST(AlignedMemoryTest, Memory)
{
    void * data = alignedMemoryAlloc(1, getDefaultAlignedSize());
    ASSERT_NE(nullptr, data);
    alignedMemoryFree(data);
}

