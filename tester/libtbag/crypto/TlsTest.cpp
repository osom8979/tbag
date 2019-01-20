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
    ASSERT_EQ(Err::E_SSLWREAD, client.handshake());
    ASSERT_EQ(Err::E_SUCCESS, client.readFromWriteBuffer(client_buffer));
    ASSERT_FALSE(client.isFinished());

    // Write: Client -> Server
    ASSERT_EQ(Err::E_SUCCESS, server.writeToReadBuffer(client_buffer.data(), client_buffer.size()));

    // [SERVER] ServerHelloDone
    ASSERT_EQ(Err::E_SSLWREAD, server.handshake());
    ASSERT_EQ(Err::E_SUCCESS, server.readFromWriteBuffer(server_buffer));
    ASSERT_FALSE(server.isFinished());

    // Write: Server -> Client
    ASSERT_EQ(Err::E_SUCCESS, client.writeToReadBuffer(server_buffer.data(), server_buffer.size()));

    // [CLIENT] HELLO DONE, ExchangeKey
    ASSERT_EQ(Err::E_SSLWREAD, client.handshake());
    ASSERT_EQ(Err::E_SUCCESS, client.readFromWriteBuffer(client_buffer));
    ASSERT_FALSE(client.isFinished());

    // Write: Client -> Server
    ASSERT_EQ(Err::E_SUCCESS, server.writeToReadBuffer(client_buffer.data(), client_buffer.size()));

    // [SERVER] ExchangeKey
    ASSERT_EQ(Err::E_SSL_NONE, server.handshake());
    ASSERT_TRUE(server.isFinished());
    ASSERT_EQ(Err::E_SUCCESS, server.readFromWriteBuffer(server_buffer));

    // Write: Server -> Client
    ASSERT_EQ(Err::E_SUCCESS, client.writeToReadBuffer(server_buffer.data(), server_buffer.size()));

    // [SERVER] ExchangeKey
    ASSERT_EQ(Err::E_SSL_NONE, client.handshake());
    ASSERT_TRUE(client.isFinished());
}

