/**
 * @file   ProcessIdTest.cpp
 * @brief  ProcessId class tester.
 * @author zer0
 * @date   2019-05-23
 */

#include <gtest/gtest.h>
#include <libtbag/system/ProcessId.hpp>

using namespace libtbag;
using namespace libtbag::system;

TEST(ProcessIdTest, Default)
{
    ASSERT_LT(0, getProcessId());
}

