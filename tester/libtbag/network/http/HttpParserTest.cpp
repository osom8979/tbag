/**
 * @file   HttpParserTest.cpp
 * @brief  HttpParser class tester.
 * @author zer0
 * @date   2017-05-18
 */

#include <gtest/gtest.h>
#include <libtbag/network/http/HttpParser.hpp>

using namespace libtbag;
using namespace libtbag::network;
using namespace libtbag::network::http;

TEST(HttpParserTest, Default)
{
    HttpParser object;
    ASSERT_TRUE(true);
}

