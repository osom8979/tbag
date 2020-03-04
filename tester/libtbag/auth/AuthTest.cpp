/**
 * @file   AuthTest.cpp
 * @brief  Auth class tester.
 * @author zer0
 * @date   2019-12-24
 * @date   2019-12-25 (Rename: HandshakeTest -> AuthTest)
 */

#include <gtest/gtest.h>
#include <libtbag/auth/Auth.hpp>

using namespace libtbag;
using namespace libtbag::auth;

TEST(AuthTest, Default)
{
    auto ca_keys = libtbag::crypto::generateKeys();
    auto const ca_public_key = std::move(ca_keys.first);
    auto const ca_private_key = std::move(ca_keys.second);

    std::string const CLIENT_DATA = "CLIENT";
    std::string const SERVER_DATA = "SERVER";

    AuthClient client;
    AuthServer server;

    ASSERT_TRUE(client.init(ca_public_key.key(), CLIENT_DATA));
    ASSERT_TRUE(server.init(ca_private_key.key(), SERVER_DATA));

    ASSERT_TRUE(client.isReady());
    ASSERT_TRUE(server.isReady());

    ASSERT_EQ(CLIENT_DATA, client.getClientData());
    ASSERT_EQ(SERVER_DATA, server.getServerData());

    ASSERT_TRUE(client.getServerData().empty());
    ASSERT_TRUE(server.getClientData().empty());

    auto const encoded_auth0 = client.encodeAuth0();
    ASSERT_FALSE(encoded_auth0.empty());
    ASSERT_TRUE(server.decodeAuth0(encoded_auth0));

    auto const encoded_auth1 = server.encodeAuth1();
    ASSERT_FALSE(encoded_auth1.empty());
    ASSERT_TRUE(client.decodeAuth1(encoded_auth1));

    auto const encoded_auth2 = client.encodeAuth2();
    ASSERT_FALSE(encoded_auth2.empty());
    ASSERT_TRUE(server.decodeAuth2(encoded_auth2));

    auto const encoded_auth3 = server.encodeAuth3();
    ASSERT_FALSE(encoded_auth3.empty());
    ASSERT_TRUE(client.decodeAuth3(encoded_auth3));

    ASSERT_FALSE(client.validation());
    ASSERT_FALSE(server.validation());

    ASSERT_EQ(CLIENT_DATA, client.getClientData());
    ASSERT_EQ(SERVER_DATA, client.getServerData());

    ASSERT_EQ(CLIENT_DATA, server.getClientData());
    ASSERT_EQ(SERVER_DATA, server.getServerData());

    std::string const TEST_DATA1 = "TEST_DATA1";
    std::string const TEST_DATA2 = "TEST_DATA2";
    ASSERT_TRUE(client.encode(TEST_DATA1).empty());
    ASSERT_TRUE(client.decode(TEST_DATA1).empty());

    client.ok();
    server.ok();

    ASSERT_TRUE(client.validation());
    ASSERT_TRUE(server.validation());

    auto const encode_data1 = client.encode(TEST_DATA1);
    ASSERT_FALSE(encode_data1.empty());
    ASSERT_EQ(TEST_DATA1, server.decode(encode_data1));

    auto const encode_data2 = server.encode(TEST_DATA2);
    ASSERT_FALSE(encode_data2.empty());
    ASSERT_EQ(TEST_DATA2, client.decode(encode_data2));
}

TEST(AuthTest, UseCaOnly)
{
    auto ca_keys = libtbag::crypto::generateKeys();
    auto const ca_public_key = std::move(ca_keys.first);
    auto const ca_private_key = std::move(ca_keys.second);

    AuthClient client;
    AuthServer server;

    ASSERT_TRUE(client.init(ca_public_key.key()));
    ASSERT_TRUE(server.init(ca_private_key.key()));

    ASSERT_TRUE(client.isReady());
    ASSERT_TRUE(server.isReady());

    std::string const CLIENT_TO_SERVER_DATA = "CLIENT_TO_SERVER_DATA";
    auto const c2s_encode = client.encodeByCaPublic(CLIENT_TO_SERVER_DATA);
    ASSERT_FALSE(c2s_encode.empty());
    ASSERT_EQ(CLIENT_TO_SERVER_DATA, server.decodeByCaPrivate(c2s_encode));

    std::string const SERVER_TO_CLIENT_DATA = "SERVER_TO_CLIENT_DATA";
    auto const s2c_encode = server.encodeByCaPrivate(SERVER_TO_CLIENT_DATA);
    ASSERT_FALSE(s2c_encode.empty());
    ASSERT_EQ(SERVER_TO_CLIENT_DATA, client.decodeByCaPublic(s2c_encode));
}

