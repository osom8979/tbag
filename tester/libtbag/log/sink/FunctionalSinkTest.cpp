/**
 * @file   FunctionalSinkTest.cpp
 * @brief  FunctionalSink class tester.
 * @author zer0
 * @date   2019-07-03
 */

#include <gtest/gtest.h>
#include <libtbag/log/sink/FunctionalSink.hpp>

using namespace libtbag;
using namespace libtbag::log;
using namespace libtbag::log::sink;

TEST(FunctionalSinkTest, Default)
{
    auto write_cb = [](char const * msg, int size, void * user) -> bool {
        auto * buffer = (std::string*)user;
        buffer->assign(msg, msg + size);
        return true;
    };

    auto flush_cb = [](void * user){
        auto * buffer = (std::string*)user;
        buffer->assign("flush");
    };

    std::string buffer;
    FunctionalSink sink(write_cb, flush_cb, &buffer);

    ASSERT_TRUE(sink.write("abcdefg", 4));
    ASSERT_STREQ("abcd", buffer.c_str());

    sink.flush();
    ASSERT_STREQ("flush", buffer.c_str());
}

