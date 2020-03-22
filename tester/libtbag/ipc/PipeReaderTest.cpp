/**
 * @file   PipeReaderTest.cpp
 * @brief  PipeReader class tester.
 * @author zer0
 * @date   2020-03-22
 */

#include <gtest/gtest.h>
#include <libtbag/ipc/PipeReader.hpp>

using namespace libtbag;
using namespace libtbag::ipc;

TEST(PipeReaderTest, Default)
{
    PipeReader object;
    ASSERT_TRUE(true);
}

