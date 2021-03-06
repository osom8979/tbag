/**
 * @file   AesTest.cpp
 * @brief  Aes class tester.
 * @author zer0
 * @date   2020-03-05
 */

#include <gtest/gtest.h>
#include <libtbag/crypto/Aes.hpp>
#include <libtbag/string/StringUtils.hpp>

using namespace libtbag;
using namespace libtbag::crypto;

TEST(AesTest, Default)
{
    KeyArray key;
    IvArray iv;

    for (auto k = 0; k < key.size(); ++k) {
        key[k] = k;
    }
    for (auto i = 0; i < iv.size(); ++i) {
        iv[i] = i;
    }

    auto const test_length = 128;
    std::string plain_text(test_length, '\0');
    libtbag::string::createRandomString(&plain_text[0], plain_text.size());
    std::cout << "Plain Text: " << plain_text << std::endl;

    auto const encrypt_result = encryptAes256CbcToBase64(key, iv, plain_text);
    ASSERT_EQ(E_SUCCESS, encrypt_result);
    ASSERT_GE(encrypt_result.val.size(), plain_text.size());
    std::cout << "Encrypt Text: " << encrypt_result.val << std::endl;

    auto const decrypt_result = decryptBase64ToAes256Cbc(key, iv, encrypt_result.val);
    ASSERT_EQ(E_SUCCESS, decrypt_result);
    ASSERT_EQ(plain_text, decrypt_result.val);
    std::cout << "Decrypt Text: " << decrypt_result.val << std::endl;
}

