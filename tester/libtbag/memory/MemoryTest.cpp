/**
 * @file   MemoryTest.cpp
 * @brief  Memory class tester.
 * @author zer0
 * @date   2019-03-03
 */

#include <gtest/gtest.h>
#include <libtbag/memory/Memory.hpp>

//using namespace libtbag;
//using namespace libtbag::memory;

TEST(MemoryTest, Default)
{
    void * mem = tbMalloc(sizeof(int));
    ASSERT_NE(nullptr, mem);
    tbFree(mem);
}

