/**
 * @file   CoutSinkTest.cpp
 * @brief  CoutSink class tester.
 * @author zer0
 * @date   2016-07-17
 */

#include <gtest/gtest.h>
#include <libtbag/log/sink/CoutSink.hpp>

using namespace libtbag;
using namespace libtbag::log;
using namespace libtbag::log::sink;

TEST(CoutSinkTest, Default)
{
    char const * const TEST = "CoutSinkTest::Default\n";
    CoutSink<> sink;
    auto message = sink.makeMessage(std::string(TEST));
    sink.write(message);
}

