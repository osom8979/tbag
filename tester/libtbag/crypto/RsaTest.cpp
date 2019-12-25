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

TEST(RsaTest, MaxSize)
{
    auto keys = generateKeys();
    auto const max_size = keys.first.getMaxDataSize(Rsa::Padding::P_PKCS1);
    auto const test_data = std::string(max_size, 'a');
    auto const encrypted_test_data = keys.first.encrypt(test_data);
    ASSERT_FALSE(encrypted_test_data.empty());
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

TEST(RsaTest, PublicKey_PrivateKey)
{
    auto keys = generateKeys();
    PublicKey public_key = std::move(keys.first);
    PrivateKey private_key = std::move(keys.second);

    ASSERT_TRUE(public_key.exists());
    ASSERT_TRUE(private_key.exists());

    ASSERT_FALSE(public_key.key().empty());
    ASSERT_FALSE(private_key.key().empty());

    std::string const test_data = "TEST_DATA";

    COMMENT("Public -> Private") {
        auto const encrypt_data = public_key.encrypt(test_data);
        ASSERT_FALSE(encrypt_data.empty());
        ASSERT_NE(test_data, encrypt_data);

        auto const decrypt_data = private_key.decrypt(encrypt_data);
        ASSERT_FALSE(decrypt_data.empty());
        ASSERT_EQ(test_data, decrypt_data);
    }

    COMMENT("Private -> Public") {
        auto const encrypt_data = private_key.encrypt(test_data);
        ASSERT_FALSE(encrypt_data.empty());
        ASSERT_NE(test_data, encrypt_data);

        auto const decrypt_data = public_key.decrypt(encrypt_data);
        ASSERT_FALSE(decrypt_data.empty());
        ASSERT_EQ(test_data, decrypt_data);
    }
}

