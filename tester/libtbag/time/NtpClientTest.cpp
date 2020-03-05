/**
 * @file   NtpClientTest.cpp
 * @brief  NtpClient class tester.
 * @author zer0
 * @date   2020-03-03
 */

#include <gtest/gtest.h>
#include <libtbag/time/NtpClient.hpp>
#include <libtbag/time/TimePoint.hpp>

using namespace libtbag;
using namespace libtbag::time;

TEST(NtpClientTest, Default)
{
    NtpClient client;
    auto const result = client.request();
    if (result) {
        std::cout << "NTP request success: " << result.value.toLocalLongString() << std::endl;
    } else {
        std::cout << "NTP request error: " << result.code << std::endl;
    }
}
