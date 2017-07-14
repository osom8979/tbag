/**
 * @file   TripleIoIndexTest.cpp
 * @brief  TripleIoIndex class tester.
 * @author zer0
 * @date   2017-07-14
 */

#include <gtest/gtest.h>
#include <libtbag/lockfree/TripleIoIndex.hpp>

using namespace libtbag;
using namespace libtbag::lockfree;

TEST(TripleIoIndexTest, Default)
{
    TripleIoIndex object;
    ASSERT_TRUE(true);
}

