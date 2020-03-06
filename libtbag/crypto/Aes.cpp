/**
 * @file   Aes.cpp
 * @brief  Aes class implementation.
 * @author zer0
 * @date   2020-03-05
 */

#include <libtbag/crypto/Aes.hpp>

#include <cassert>
#include <memory>

#include <openssl/conf.h>
#include <openssl/evp.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace crypto {

using UniqueEvpCipherContext = std::unique_ptr<::EVP_CIPHER_CTX, decltype(&::EVP_CIPHER_CTX_free)>;

ErrBuffer encryptAes256Cbc(KeyArray const & key, IvArray const & iv, Buffer const & input)
{
    UniqueEvpCipherContext context(EVP_CIPHER_CTX_new(), ::EVP_CIPHER_CTX_free);

    auto const init_result = EVP_EncryptInit_ex(context.get(),
                                                EVP_aes_256_cbc(),
                                                nullptr,
                                                key.data(),
                                                iv.data());
    if (init_result != 1) {
        return E_SSL;
    }

    auto const key_size = key.size();
    auto const iv_size = iv.size();
    auto const block_size = EVP_CIPHER_CTX_block_size(context.get());

    assert(key_size == EVP_CIPHER_CTX_key_length(context.get()));
    assert(iv_size == EVP_CIPHER_CTX_iv_length(context.get()));
    assert(iv_size == block_size);

    Buffer output;
    output.resize(input.size() + block_size);

    int updated_size = static_cast<int>(output.size());
    auto const update_result = EVP_EncryptUpdate(context.get(),
                                                 (unsigned char *)&output[0],
                                                 &updated_size,
                                                 (unsigned char const *)&input[0],
                                                 (int)input.size());
    if (update_result != 1) {
        return E_SSL;
    }

    int remaining_size = static_cast<int>(output.size()) - updated_size;
    auto const final_result = EVP_EncryptFinal_ex(context.get(),
                                                  (unsigned char *)&output[0]+updated_size,
                                                  &remaining_size);
    if (final_result != 1) {
        return E_SSL;
    }

    output.resize(updated_size + remaining_size);
    return { E_SUCCESS, output };
}

ErrBuffer decryptAes256Cbc(KeyArray const & key, IvArray const & iv, Buffer const & input)
{
    UniqueEvpCipherContext context(EVP_CIPHER_CTX_new(), ::EVP_CIPHER_CTX_free);

    auto const init_result = EVP_DecryptInit_ex(context.get(),
                                                EVP_aes_256_cbc(),
                                                nullptr,
                                                key.data(),
                                                iv.data());
    if (init_result != 1) {
        return E_SSL;
    }

    auto const key_size = key.size();
    auto const iv_size = iv.size();
    auto const block_size = EVP_CIPHER_CTX_block_size(context.get());

    assert(key_size == EVP_CIPHER_CTX_key_length(context.get()));
    assert(iv_size == EVP_CIPHER_CTX_iv_length(context.get()));
    assert(iv_size == block_size);

    Buffer output;
    output.resize(input.size() + block_size);

    int updated_size = static_cast<int>(output.size());
    auto const update_result = EVP_DecryptUpdate(context.get(),
                                                 (unsigned char *)&output[0],
                                                 &updated_size,
                                                 (unsigned char const *)&input[0],
                                                 (int)input.size());
    if (update_result != 1) {
        return E_SSL;
    }

    int remaining_size = static_cast<int>(output.size()) - updated_size;
    auto const final_result = EVP_DecryptFinal_ex(context.get(),
                                                  (unsigned char *)&output[0]+updated_size,
                                                  &remaining_size);
    if (final_result != 1) {
        return E_SSL;
    }

    output.resize(updated_size + remaining_size);
    return { E_SUCCESS, output };
}

ErrString encryptAes256Cbc(KeyArray const & key, IvArray const & iv, std::string const & input)
{
    Buffer input_buffer(input.cbegin(), input.cend());
    auto const result = encryptAes256Cbc(key, iv, input_buffer);
    if (!result) {
        return result.code;
    }
    return { E_SUCCESS, std::string(result.value.cbegin(), result.value.cend()) };
}

ErrString decryptAes256Cbc(KeyArray const & key, IvArray const & iv, std::string const & input)
{
    Buffer input_buffer(input.cbegin(), input.cend());
    auto const result = decryptAes256Cbc(key, iv, input_buffer);
    if (!result) {
        return result.code;
    }
    return { E_SUCCESS, std::string(result.value.cbegin(), result.value.cend()) };
}

} // namespace crypto

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

