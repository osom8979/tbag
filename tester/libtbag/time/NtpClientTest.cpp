/**
 * @file   NtpClientTest.cpp
 * @brief  NtpClient class tester.
 * @author zer0
 * @date   2020-03-03
 */

#include <gtest/gtest.h>
#include <libtbag/time/NtpClient.hpp>

using namespace libtbag;
using namespace libtbag::time;

TEST(NtpClientTest, Default)
{
    NtpClient object;
    ASSERT_TRUE(true);
}

