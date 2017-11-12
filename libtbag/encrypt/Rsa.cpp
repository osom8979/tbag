/**
 * @file   Rsa.cpp
 * @brief  Rsa class implementation.
 * @author zer0
 * @date   2017-11-11
 */

#include <libtbag/encrypt/Rsa.hpp>
#include <libtbag/log/Log.hpp>

#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/bn.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

#include <iostream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace encrypt {

static int genrsa_cb(int p, int n, BN_GENCB * cb)
{
    char c = '*';

    if (p == 0)
        c = '.';
    if (p == 1)
        c = '+';
    if (p == 2)
        c = '*';
    if (p == 3)
        c = '\n';
//    BIO_write(cb->arg, &c, 1);
//    (void) BIO_flush(cb->arg);
    return 1;
}

static EVP_CIPHER const * getEvpCipher(EvpCipherAlgorithm algorithm)
{
    // @formatter:off
    switch (algorithm) {
#ifndef OPENSSL_NO_DES
    case EvpCipherAlgorithm::ECA_DES:  return EVP_des_cbc();
    case EvpCipherAlgorithm::ECA_DES3: return EVP_des_ede3_cbc();
#endif
#ifndef OPENSSL_NO_IDEA
    case EvpCipherAlgorithm::ECA_IDEA: return EVP_idea_cbc();
#endif
#ifndef OPENSSL_NO_AES
    case EvpCipherAlgorithm::ECA_AES128: return EVP_aes_128_cbc();
    case EvpCipherAlgorithm::ECA_AES192: return EVP_aes_192_cbc();
    case EvpCipherAlgorithm::ECA_AES256: return EVP_aes_256_cbc();
#endif
#ifndef OPENSSL_NO_CAMELLIA
    case EvpCipherAlgorithm::ECA_CAMELLIA128: return EVP_camellia_128_cbc();
    case EvpCipherAlgorithm::ECA_CAMELLIA192: return EVP_camellia_192_cbc();
    case EvpCipherAlgorithm::ECA_CAMELLIA256: return EVP_camellia_256_cbc();
#endif
    case EvpCipherAlgorithm::ECA_UNKNOWN:
    default:
        break;
    }
    // @formatter:on
    return nullptr;
}

static EVP_CIPHER const * getEvpCipherByName(std::string const & name)
{
    if (name.empty()) {
        return nullptr;
    }
    return EVP_get_cipherbyname(name.c_str());
}

bool generateRsa(std::vector<char> & public_key, std::vector<char> & private_key, int key_length, EvpCipherAlgorithm algorithm)
{
    EVP_CIPHER const * enc = getEvpCipher(algorithm);

    BIO *  public_out = BIO_new(BIO_s_mem());
    BIO * private_out = BIO_new(BIO_s_mem());

    // https://www.openssl.org/docs/man1.1.0/crypto/RSA_generate_key.html
    // The exponent is an odd number, typically 3, 17 or 65537
    unsigned long const E = RSA_F4;

    // Generate rsa key
    BIGNUM * bn = BN_new();;
    assert(bn != nullptr);

    if (BN_set_word(bn, E) != 1) {
        return false;
    }

    RSA * rsa = RSA_new();
    assert(rsa != nullptr);

    if (RSA_generate_key_ex(rsa, key_length, bn, nullptr) != 1) {
        return false;
    }

    PEM_write_bio_RSA_PUBKEY(public_out, rsa);
    PEM_write_bio_RSAPrivateKey(private_out, rsa, enc, nullptr, 0, nullptr, nullptr);

    int  public_length = BIO_pending( public_out);
    int private_length = BIO_pending(private_out);

    public_key.resize(static_cast<std::size_t>(public_length));
    private_key.resize(static_cast<std::size_t>(private_length));

    BIO_read( public_out,  public_key.data(),  public_length);
    BIO_read(private_out, private_key.data(), private_length);

    if (rsa != nullptr) {
        RSA_free(rsa);
    }
    if (bn != nullptr) {
        BN_free(bn);
    }

    if (public_out != nullptr) {
        BIO_free_all(public_out);
    }
    if (private_out != nullptr) {
        BIO_free_all(private_out);
    }

    return true;
}

Rsa::Rsa()
{
    // EMPTY.
}

Rsa::~Rsa()
{
    // EMPTY.
}

} // namespace encrypt

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

