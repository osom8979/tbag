/**
 * @file   TlsTest.cpp
 * @brief  Tls class tester.
 * @author zer0
 * @date   2019-01-20
 */

#include <gtest/gtest.h>
#include <libtbag/crypto/Tls.hpp>
#include <libtbag/crypto/Rsa.hpp>
#include <libtbag/crypto/X509.hpp>

using namespace libtbag;
using namespace libtbag::crypto;

TEST(TlsTest, Default)
{
    auto const PRIVATE_KEY = Rsa::generatePemPrivateKey();
    ASSERT_FALSE(PRIVATE_KEY.empty());

    auto const CSR = generateCsrVersion1(PRIVATE_KEY);
    ASSERT_FALSE(CSR.empty());

    auto const CERTIFICATE = generateSelfSignedCertificate(PRIVATE_KEY, CSR);
    ASSERT_FALSE(CERTIFICATE.empty());

    Tls client;
    Tls server(CERTIFICATE, PRIVATE_KEY);

    client.connect();
    server.accept();

    libtbag::util::Buffer client_buffer;
    libtbag::util::Buffer server_buffer;

    // [CLIENT] ServerHello
    ASSERT_EQ(E_SSLWREAD, client.handshake());
    ASSERT_EQ(E_SUCCESS, client.readFromWriteBuffer(client_buffer));
    ASSERT_FALSE(client.isFinished());

    // Write: Client -> Server
    ASSERT_EQ(E_SUCCESS, server.writeToReadBuffer(client_buffer.data(), client_buffer.size()));

    // [SERVER] ServerHelloDone
    ASSERT_EQ(E_SSLWREAD, server.handshake());
    ASSERT_EQ(E_SUCCESS, server.readFromWriteBuffer(server_buffer));
    ASSERT_FALSE(server.isFinished());

    // Write: Server -> Client
    ASSERT_EQ(E_SUCCESS, client.writeToReadBuffer(server_buffer.data(), server_buffer.size()));

    // [CLIENT] HELLO DONE, ExchangeKey
    ASSERT_EQ(E_SSLWREAD, client.handshake());
    ASSERT_EQ(E_SUCCESS, client.readFromWriteBuffer(client_buffer));
    ASSERT_FALSE(client.isFinished());

    // Write: Client -> Server
    ASSERT_EQ(E_SUCCESS, server.writeToReadBuffer(client_buffer.data(), client_buffer.size()));

    // [SERVER] ExchangeKey
    ASSERT_EQ(E_SSL_NONE, server.handshake());
    ASSERT_TRUE(server.isFinished());
    ASSERT_EQ(E_SUCCESS, server.readFromWriteBuffer(server_buffer));

    // Write: Server -> Client
    ASSERT_EQ(E_SUCCESS, client.writeToReadBuffer(server_buffer.data(), server_buffer.size()));

    // [SERVER] ExchangeKey
    ASSERT_EQ(E_SSL_NONE, client.handshake());
    ASSERT_TRUE(client.isFinished());

    std::string const TEST_MESSAGE = "TEST_MESSAGE";

    COMMENT("Client -> Server") {
        Err encode_code = E_UNKNOWN;
        auto encode_result = client.encode(TEST_MESSAGE.data(), TEST_MESSAGE.size(), &encode_code);
        ASSERT_EQ(E_SUCCESS, encode_code);

        Err decode_code = E_UNKNOWN;
        auto cursor = server.decode(encode_result.data(), encode_result.size(), &decode_code);
        ASSERT_EQ(E_SUCCESS, decode_code);
        ASSERT_EQ(TEST_MESSAGE, std::string(cursor.begin(), cursor.end()));
    }

    COMMENT("Server -> Client") {
        Err encode_code = E_UNKNOWN;
        auto encode_result = server.encode(TEST_MESSAGE.data(), TEST_MESSAGE.size(), &encode_code);
        ASSERT_EQ(E_SUCCESS, encode_code);

        Err decode_code = E_UNKNOWN;
        auto cursor = client.decode(encode_result.data(), encode_result.size(), &decode_code);
        ASSERT_EQ(E_SUCCESS, decode_code);
        ASSERT_EQ(TEST_MESSAGE, std::string(cursor.begin(), cursor.end()));
    }
}

