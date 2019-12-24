/**
 * @file   HandshakeTest.cpp
 * @brief  Handshake class tester.
 * @author zer0
 * @date   2019-12-24
 */

#include <gtest/gtest.h>
#include <libtbag/auth/Handshake.hpp>

using namespace libtbag;
using namespace libtbag::auth;

TEST(HandshakeTest, Default)
{
    auto ca_keys = libtbag::crypto::generateKeys();
    auto const ca_public_key = std::move(ca_keys.first);
    auto const ca_private_key = std::move(ca_keys.second);

    auto const SERVER_DATA = "SERVER";
    auto const CLIENT_DATA = "CLIENT";

    HandshakeClient client;
    HandshakeServer server;

    ASSERT_TRUE(client.init(ca_public_key.key(), CLIENT_DATA));
    ASSERT_TRUE(server.init(ca_private_key.key(), SERVER_DATA));
}

