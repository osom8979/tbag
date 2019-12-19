/**
 * @file   TaskManagerTest.cpp
 * @brief  TaskManager class tester.
 * @author zer0
 * @date   2019-12-19
 */

#include <gtest/gtest.h>
#include <libtbag/task/TaskManager.hpp>

using namespace libtbag;
using namespace libtbag::task;

TEST(TaskManagerTest, Default)
{
    TaskManager object;
    ASSERT_TRUE(true);
}

