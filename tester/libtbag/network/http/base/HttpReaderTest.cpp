/**
 * @file   HttpReaderTest.cpp
 * @brief  HttpReader class tester.
 * @author zer0
 * @date   2017-10-03
 */

#include <gtest/gtest.h>
#include <libtbag/network/http/base/HttpReader.hpp>

using namespace libtbag;
using namespace libtbag::network;
using namespace libtbag::network::http;
using namespace libtbag::network::http::base;

TEST(HttpReaderTest, Default)
{
    HttpReader object;
    ASSERT_TRUE(true);
}

