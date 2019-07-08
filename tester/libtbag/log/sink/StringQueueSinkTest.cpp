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
    StringQueueSink sink(2);

    ASSERT_TRUE(sink.write(0, "ab", 2));
    ASSERT_TRUE(sink.write(0, "cd", 2));
    sink.flush();
    ASSERT_EQ(2, sink.deque().size());
    ASSERT_STREQ("ab", sink.deque()[0].c_str());
    ASSERT_STREQ("cd", sink.deque()[1].c_str());

    ASSERT_TRUE(sink.write(0, "ef", 2));
    sink.flush();
    ASSERT_EQ(2, sink.deque().size());
    ASSERT_STREQ("cd", sink.deque()[0].c_str());
    ASSERT_STREQ("ef", sink.deque()[1].c_str());
}

