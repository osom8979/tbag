/**
 * @file   CallbackSinkTest.cpp
 * @brief  CallbackSink class tester.
 * @author zer0
 * @date   2019-07-03
 */

#include <gtest/gtest.h>
#include <libtbag/log/sink/CallbackSink.hpp>
#include <string>

using namespace libtbag;
using namespace libtbag::log;
using namespace libtbag::log::sink;

static bool __callback_sink_test_write_cb(char const * msg, int size, void * user)
{
    auto * buffer = (std::string*)user;
    buffer->assign(msg, msg + size);
    return true;
}

static void __callback_sink_test_flush_cb(void * user)
{
    auto * buffer = (std::string*)user;
    buffer->assign("flush");
}

TEST(CallbackSinkTest, Default)
{
    std::string buffer;
    CallbackSink sink(&__callback_sink_test_write_cb, &__callback_sink_test_flush_cb, &buffer);

    ASSERT_TRUE(sink.write("abcdefg", 4));
    ASSERT_STREQ("abcd", buffer.c_str());

    sink.flush();
    ASSERT_STREQ("flush", buffer.c_str());
}

