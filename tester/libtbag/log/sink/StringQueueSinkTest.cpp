/**
 * @file   StringQueueSinkTest.cpp
 * @brief  StringQueueSink class tester.
 * @author zer0
 * @date   2019-07-03
 */

#include <gtest/gtest.h>
#include <libtbag/log/sink/StringQueueSink.hpp>

using namespace libtbag;
using namespace libtbag::log;
using namespace libtbag::log::sink;

TEST(StringQueueSinkTest, Default)
{
    StringQueueSink object;
    ASSERT_TRUE(true);
}

