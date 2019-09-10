/**
 * @file   SocketAddressTest.cpp
 * @brief  SocketAddress class tester.
 * @author zer0
 * @date   2019-09-10
 */

#include <gtest/gtest.h>
#include <libtbag/net/SocketAddress.hpp>
#include <libtbag/util/TestUtils.hpp>

using namespace libtbag;
using namespace libtbag::net;

TEST(SocketAddressTest, Default)
{
    TEST_DEFAULT_CONSTRUCTOR(SocketAddress, obj1);
    TEST_DEFAULT_ASSIGNMENT(SocketAddress, obj2);
}

TEST(SocketAddressTest, FindHostNameOfConnectedInterfaceByIpAddress)
{
    auto const name = findHostNameOfConnectedInterfaceByIpAddress("127.0.0.1");
    std::cout << "Interface name: " << name << std::endl;
}

