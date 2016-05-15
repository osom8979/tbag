/**
 * @file   TaskTestTest.cpp
 * @brief  TaskTest class tester.
 * @author zer0
 * @date   2016-05-15
 */

#include <gtest/gtest.h>
#include <libtbag/thread/task/Task.hpp>

using namespace libtbag;
using namespace libtbag::thread;
using namespace libtbag::thread::task;

TEST(TaskTest, Default)
{
    Task task;
}

