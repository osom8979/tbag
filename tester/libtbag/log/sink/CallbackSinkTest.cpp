/**
 * @file   CallbackSinkTest.cpp
 * @brief  CallbackSink class tester.
 * @author zer0
 * @date   2019-07-03
 */

#include <gtest/gtest.h>
#include <libtbag/log/sink/CallbackSink.hpp>

using namespace libtbag;
using namespace libtbag::log;
using namespace libtbag::log::sink;

static bool __callback_sink_test_write_cb(char const * msg, int size, void * user)
{
    return true;
}

static void __callback_sink_test_flush_cb(void * user)
{
}

TEST(CallbackSinkTest, Default)
{
    int temp = 100;
    CallbackSink object(&__callback_sink_test_write_cb, &__callback_sink_test_flush_cb, &temp);
    ASSERT_TRUE(true);
}

