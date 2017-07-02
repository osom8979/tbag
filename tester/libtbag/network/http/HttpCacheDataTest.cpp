/**
 * @file   HttpCacheDataTest.cpp
 * @brief  HttpCacheData class tester.
 * @author zer0
 * @date   2017-07-02
 */

#include <gtest/gtest.h>
#include <libtbag/network/http/HttpCacheData.hpp>

using namespace libtbag;
using namespace libtbag::network;
using namespace libtbag::network::http;

TEST(HttpCacheDataTest, Default)
{
    HttpCacheData object;
    ASSERT_TRUE(true);
}

