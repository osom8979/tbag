/**
 * @file   NullSinkTest.cpp
 * @brief  NullSink class tester.
 * @author zer0
 * @date   2019-07-03
 */

#include <gtest/gtest.h>
#include <libtbag/log/sink/NullSink.hpp>

using namespace libtbag;
using namespace libtbag::log;
using namespace libtbag::log::sink;

TEST(NullSinkTest, Default)
{
    NullSink sink;
    ASSERT_TRUE(sink.write("abcd", 4));
    sink.flush();
}

