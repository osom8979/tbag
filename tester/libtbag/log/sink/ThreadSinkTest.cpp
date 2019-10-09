/**
 * @file   ThreadSinkTest.cpp
 * @brief  ThreadSink class tester.
 * @author zer0
 * @date   2019-10-09
 */

#include <gtest/gtest.h>
#include <libtbag/log/sink/ThreadSink.hpp>
#include <libtbag/log/sink/StringQueueSink.hpp>

using namespace libtbag;
using namespace libtbag::log;
using namespace libtbag::log::sink;

TEST(ThreadSinkTest, Default)
{
    auto queue_sink = std::make_shared<StringQueueSink>();
    {
        ThreadSink sink(queue_sink);
        ASSERT_TRUE(sink.write(0, "ab", 2));
        ASSERT_TRUE(sink.write(0, "cd", 2));
    }

    ASSERT_EQ(2, queue_sink->deque().size());
    ASSERT_STREQ("ab", queue_sink->deque()[0].c_str());
    ASSERT_STREQ("cd", queue_sink->deque()[1].c_str());
}

