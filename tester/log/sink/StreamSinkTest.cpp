/**
 * @file   StreamSinkTest.cpp
 * @brief  StreamSink class tester.
 * @author zer0
 * @date   2016-07-10
 */

#include <gtest/gtest.h>
#include <libtbag/log/sink/StreamSink.hpp>

#include <sstream>

using namespace libtbag;
using namespace libtbag::log;
using namespace libtbag::log::sink;

TEST(StreamSinkTest, Default)
{
    char const * const TEST = "TEST";
    std::stringstream ss;
    StreamSink<std::stringstream> sink(ss);
    auto message = sink.makeMessage(std::string(TEST));
    sink.write(message);
    sink.flush();

    ASSERT_STREQ(ss.str().c_str(), TEST);
}

