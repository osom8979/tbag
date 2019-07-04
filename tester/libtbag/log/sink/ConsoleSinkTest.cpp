/**
 * @file   ConsoleSinkTest.cpp
 * @brief  ConsoleSink class tester.
 * @author zer0
 * @date   2019-07-03
 */

#include <gtest/gtest.h>
#include <libtbag/log/sink/ConsoleSink.hpp>

using namespace libtbag;
using namespace libtbag::log;
using namespace libtbag::log::sink;

TEST(ConsoleSinkTest, Default)
{
    ConsoleSink sink;
#if defined(TBAG_PLATFORM_WINDOWS)
    ASSERT_TRUE(sink.write("ab\r\n", 4));
#else
    ASSERT_TRUE(sink.write("abc\n", 4));
#endif
    sink.flush();
}

