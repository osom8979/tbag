/**
 * @file   UvCommonTest.cpp
 * @brief  UvCommon class tester.
 * @author zer0
 * @date   2017-05-27
 */

#include <gtest/gtest.h>
#include <libtbag/uvpp/UvCommon.hpp>

#include <string>

using namespace libtbag;
using namespace libtbag::uvpp;

TEST(UvCommonTest, Inet4)
{
    char const * TEST_ADDRESS = "255.255.255.255";

    std::size_t const SIZE = sizeof(struct in_addr);
    unsigned char buffer[SIZE] = {0,};

    ASSERT_EQ(E_SUCCESS, convertInetPton(AF_INET, TEST_ADDRESS, &buffer));

    std::string result;
    ASSERT_EQ(E_SUCCESS, convertInetNtop(AF_INET, &buffer, result));
    ASSERT_STREQ(TEST_ADDRESS, result.c_str());
}

TEST(UvCommonTest, Inet6)
{
    char const * TEST_ADDRESS = "2001:db8:85a3:8d3:1319:8a2e:370:7334";

    std::size_t const SIZE = sizeof(struct in6_addr);
    unsigned char buffer[SIZE] = {0,};

    ASSERT_EQ(E_SUCCESS, convertInetPton(AF_INET6, TEST_ADDRESS, &buffer));

    std::string result;
    ASSERT_EQ(E_SUCCESS, convertInetNtop(AF_INET6, &buffer, result));
    ASSERT_STREQ(TEST_ADDRESS, result.c_str());
}

