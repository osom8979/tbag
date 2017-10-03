/**
 * @file   HttpReaderTest.cpp
 * @brief  HttpReader class tester.
 * @author zer0
 * @date   2017-10-03
 */

#include <gtest/gtest.h>
#include <libtbag/network/http/HttpReader.hpp>

using namespace libtbag;
using namespace libtbag::network;
using namespace libtbag::network::http;

TEST(HttpReaderTest, Default)
{
    HttpReader object;
    ASSERT_TRUE(true);
}

