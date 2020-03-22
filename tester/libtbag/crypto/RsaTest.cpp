/**
 * @file   RsaTest.cpp
 * @brief  Rsa class tester.
 * @author zer0
 * @date   2017-12-07
 */

#include <gtest/gtest.h>
#include <libtbag/crypto/Rsa.hpp>
#include <libtbag/string/StringUtils.hpp>

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

TEST(RsaTest, PrivateKey_Encode_01)
{
    auto key = generateKeys().second;
    std::string data = R"({"approval_date":{"days":22,"hours":20,"minutes":23,"months":3,"seconds":18,)"
                       R"("years":2020},"authentication_key":"RDRGNDRDQTRFOTZCM0Y2MEU3NDFDOTBCMjA2Rjc5MEY=",)"
                       R"("deadline":{"days":22,"hours":20,"minutes":23,"months":4,"seconds":18,"years":2020},)"
                       R"("features":[""],"initialization_vector":"QjZCODdDM0U3REQ1Qjg4OA==","request_date":)"
                       R"({"days":22,"hours":20,"minutes":23,"months":3,"seconds":18,"years":2020},"session":)"
                       R"({"machine_id":"","product_uuid":"","session_key":"4DAADE70-EAA0-FED3-A2BE-70CD9CAAF4C0"}})";
    ASSERT_TRUE(key.exists());

    auto const default_padding = Rsa::Padding::P_PKCS1;
    ASSERT_TRUE(key.exists());
    ASSERT_FALSE(data.empty());
    auto const max_data_size = key.getMaxDataSize(default_padding);
    auto const tokens = libtbag::string::splitSize(data, max_data_size);
    ASSERT_EQ(3, tokens.size());

    auto const token0_size = tokens[0].size();
    ASSERT_GE(max_data_size, token0_size);
    auto const token1_size = tokens[1].size();
    ASSERT_GE(max_data_size, token1_size);
    auto const token2_size = tokens[2].size();
    ASSERT_GE(max_data_size, token2_size);

    ASSERT_FALSE(key.encrypt(tokens[0]).empty());
    ASSERT_FALSE(key.encrypt(tokens[1]).empty());
    ASSERT_FALSE(key.encrypt(tokens[2]).empty());
}

