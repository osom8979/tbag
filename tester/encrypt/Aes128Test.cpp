/**
 * @file   Aes128Test.cpp
 * @brief  Aes128 class tester.
 * @author zer0
 * @date   2016-11-18
 */

#include <gtest/gtest.h>
#include <libtbag/encrypt/Aes128.hpp>

using namespace libtbag;
using namespace libtbag::encrypt;

TEST(Aes128Test, EncodeAndDecode_01)
{
    Aes128::Key key;
    key.fill(0x00);

    char const * const CONTENT = "0123456789abcdef";
    std::string const TEST_BODY = CONTENT;
    ASSERT_EQ(16, TEST_BODY.size());
    ASSERT_EQ(16, Aes128::predictSize(TEST_BODY.size()));

    std::vector<uint8_t> input;
    input.assign(TEST_BODY.begin(), TEST_BODY.end());

    std::vector<uint8_t> encrypt;
    ASSERT_EQ(16, Aes128::encrypt(encrypt, key, &input[0], input.size()));

    std::vector<uint8_t> decrypt;
    ASSERT_EQ(16, Aes128::decrypt(decrypt, key, &encrypt[0], encrypt.size()));

    ASSERT_NE(input, encrypt);
    ASSERT_NE(encrypt, decrypt);

    std::string result;
    result.assign(decrypt.begin(), decrypt.begin() + TEST_BODY.size());
    ASSERT_EQ(TEST_BODY, result);
}

TEST(Aes128Test, EncodeAndDecode_02)
{
    Aes128::Key key;
    key.fill(0x00);

    char const * const CONTENT = "0123456789abcdef+";
    std::string const TEST_BODY = CONTENT;
    ASSERT_EQ(17, TEST_BODY.size());
    ASSERT_EQ(16 * 2, Aes128::predictSize(TEST_BODY.size()));

    std::vector<uint8_t> input;
    input.assign(TEST_BODY.begin(), TEST_BODY.end());

    std::vector<uint8_t> encrypt;
    ASSERT_EQ(16 * 2, Aes128::encrypt(encrypt, key, &input[0], input.size()));

    std::vector<uint8_t> decrypt;
    ASSERT_EQ(16 * 2, Aes128::decrypt(decrypt, key, &encrypt[0], encrypt.size()));

    ASSERT_NE(input, encrypt);
    ASSERT_NE(encrypt, decrypt);

    std::string result;
    result.assign(decrypt.begin(), decrypt.begin() + TEST_BODY.size());
    ASSERT_EQ(TEST_BODY, result);
}

TEST(Aes128Test, EncodeAndDecode_Fail)
{
    Aes128::Key key;
    key.fill(0x00);

    char const * const CONTENT = "0123456789abcdef";
    std::string const TEST_BODY = CONTENT;
    ASSERT_EQ(16, TEST_BODY.size());
    ASSERT_EQ(16, Aes128::predictSize(TEST_BODY.size()));

    std::vector<uint8_t> input;
    input.assign(TEST_BODY.begin(), TEST_BODY.end());

    std::vector<uint8_t> encrypt;
    ASSERT_EQ(16, Aes128::encrypt(encrypt, key, &input[0], input.size()));

    Aes128::Key key2;
    key2.fill(0x00);
    key2[0] = 1;

    std::vector<uint8_t> decrypt;
    ASSERT_EQ(16, Aes128::decrypt(decrypt, key2, &encrypt[0], encrypt.size()));

    ASSERT_NE(input, encrypt);
    ASSERT_NE(encrypt, decrypt);

    std::string result;
    result.assign(decrypt.begin(), decrypt.begin() + TEST_BODY.size());
    ASSERT_NE(TEST_BODY, result);
}

