/**
 * @file   SyncedMemoryTest.cpp
 * @brief  SyncedMemory class tester.
 * @author zer0
 * @date   2017-12-28
 */

#include <gtest/gtest.h>
#include <libtbag/gpu/SyncedMemory.hpp>

using namespace libtbag;
using namespace libtbag::gpu;

TEST(SyncedMemoryTest, Default)
{
    SyncedMemory object;
    ASSERT_TRUE(true);
}

