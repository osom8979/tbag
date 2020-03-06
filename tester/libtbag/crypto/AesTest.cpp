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

    auto const test_length = 2000;
    std::string plain_text(test_length, '\0');

    using namespace libtbag::string;
    ASSERT_TRUE(createRandomString(&plain_text[0], plain_text.size()));

    auto const encrypt_result = encryptAes256Cbc(key, iv, plain_text);
    ASSERT_EQ(E_SUCCESS, encrypt_result.code);
    ASSERT_GE(encrypt_result.value.size(), plain_text.size());

    auto const decrypt_result = decryptAes256Cbc(key, iv, encrypt_result.value);
    ASSERT_EQ(E_SUCCESS, decrypt_result.code);
    ASSERT_EQ(plain_text, decrypt_result.value);
}

