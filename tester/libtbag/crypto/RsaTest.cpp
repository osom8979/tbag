/**
 * @file   RsaTest.cpp
 * @brief  Rsa class tester.
 * @author zer0
 * @date   2017-12-07
 */

#include <gtest/gtest.h>
#include <libtbag/crypto/Rsa.hpp>

using namespace libtbag;
using namespace libtbag::crypto;

TEST(RsaTest, Default)
{
    std::string public_key;
    std::string private_key;

    ASSERT_TRUE(Rsa::generatePem(public_key, private_key));
    ASSERT_FALSE(public_key.empty());
    ASSERT_FALSE(private_key.empty());

    std::cout << public_key << std::endl;
    std::cout << private_key << std::endl;
}

TEST(RsaTest, EncryptPublic)
{
    std::string public_key;
    std::string private_key;

    ASSERT_TRUE(Rsa::generatePem(public_key, private_key));
    ASSERT_FALSE(public_key.empty());
    ASSERT_FALSE(private_key.empty());

    Rsa encrypt, decrypt;
    ASSERT_TRUE(encrypt.readPemPublicKey(public_key));
    ASSERT_TRUE(decrypt.readPemPrivateKey(private_key));

    std::string const TEST_DATA = "TEST_DATA";
    auto const ENCRYPT_DATA = encrypt.encryptPublic(TEST_DATA);
    ASSERT_FALSE(ENCRYPT_DATA.empty());
    ASSERT_NE(TEST_DATA, ENCRYPT_DATA);

    auto const DECRYPT_DATA = decrypt.decryptPrivate(ENCRYPT_DATA);
    ASSERT_FALSE(DECRYPT_DATA.empty());
    ASSERT_EQ(TEST_DATA, DECRYPT_DATA);
}

TEST(RsaTest, EncryptPrivate)
{
    std::string public_key;
    std::string private_key;

    ASSERT_TRUE(Rsa::generatePem(public_key, private_key));
    ASSERT_FALSE(public_key.empty());
    ASSERT_FALSE(private_key.empty());

    Rsa encrypt, decrypt;
    ASSERT_TRUE(encrypt.readPemPrivateKey(private_key));
    ASSERT_TRUE(decrypt.readPemPublicKey(public_key));

    std::string const TEST_DATA = "TEST_DATA";
    auto const ENCRYPT_DATA = encrypt.encryptPrivate(TEST_DATA);
    ASSERT_FALSE(ENCRYPT_DATA.empty());
    ASSERT_NE(TEST_DATA, ENCRYPT_DATA);

    auto const DECRYPT_DATA = decrypt.decryptPublic(ENCRYPT_DATA);
    ASSERT_FALSE(DECRYPT_DATA.empty());
    ASSERT_EQ(TEST_DATA, DECRYPT_DATA);
}

