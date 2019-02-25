/**
 * @file   ArchiveTest.cpp
 * @brief  Archive class tester.
 * @author zer0
 * @date   2019-02-25
 */

#include <gtest/gtest.h>
#include <libtbag/archive/Archive.hpp>

using namespace libtbag;
using namespace libtbag::archive;

TEST(ArchiveTest, Default)
{
    Archive object;
    ASSERT_TRUE(true);
}

