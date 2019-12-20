/**
 * @file   Rsa.cpp
 * @brief  Rsa class implementation.
 * @author zer0
 * @date   2017-12-07
 */

#include <libtbag/crypto/Rsa.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/log/Log.hpp>

#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/bn.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

#include <cassert>
#include <cstring>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace crypto {

static EVP_CIPHER const * __get_evp_cipher(Rsa::CipherAlgorithm algorithm) TBAG_NOEXCEPT
{
    // clang-format off
    switch (algorithm) {
#ifndef OPENSSL_NO_DES
    case Rsa::CipherAlgorithm::CA_DES:  return EVP_des_cbc();
    case Rsa::CipherAlgorithm::CA_DES3: return EVP_des_ede3_cbc();
#endif
#ifndef OPENSSL_NO_IDEA
    case Rsa::CipherAlgorithm::CA_IDEA: return EVP_idea_cbc();
#endif
#ifndef OPENSSL_NO_AES
    case Rsa::CipherAlgorithm::CA_AES128: return EVP_aes_128_cbc();
    case Rsa::CipherAlgorithm::CA_AES192: return EVP_aes_192_cbc();
    case Rsa::CipherAlgorithm::CA_AES256: return EVP_aes_256_cbc();
#endif
#ifndef OPENSSL_NO_CAMELLIA
    case Rsa::CipherAlgorithm::CA_CAMELLIA128: return EVP_camellia_128_cbc();
    case Rsa::CipherAlgorithm::CA_CAMELLIA192: return EVP_camellia_192_cbc();
    case Rsa::CipherAlgorithm::CA_CAMELLIA256: return EVP_camellia_256_cbc();
#endif
    case Rsa::CipherAlgorithm::CA_NONE:
    default:
        break;
    }
    // clang-format on
    return nullptr;
}

static EVP_CIPHER const * __get_evp_cipher_by_name(std::string const & name) TBAG_NOEXCEPT
{
    if (name.empty()) {
        return nullptr;
    }
    return EVP_get_cipherbyname(name.c_str());
}

int __get_rsa_padding(Rsa::Padding p) TBAG_NOEXCEPT
{
    // clang-format off
    switch (p) {
    case Rsa::Padding::P_PKCS1:      return RSA_PKCS1_PADDING;
    case Rsa::Padding::P_PKCS1_OAEP: return RSA_PKCS1_OAEP_PADDING;
    case Rsa::Padding::P_SSLV23:     return RSA_SSLV23_PADDING;
    case Rsa::Padding::P_NO:         return RSA_NO_PADDING;
    default:
        TBAG_INACCESSIBLE_BLOCK_ASSERT();
        return RSA_NO_PADDING;
    }
    // clang-format on
}

// -------------------
// Rsa implementation.
// -------------------

Rsa::Rsa()
{
    // EMPTY.
}

Rsa::Rsa(Rsa && obj) TBAG_NOEXCEPT : _rsa(std::move(obj._rsa))
{
    // EMPTY.
}

Rsa::~Rsa()
{
    // EMPTY.
}

Rsa & Rsa::operator =(Rsa && obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _rsa = std::move(obj._rsa);
    }
    return *this;
}

void Rsa::swap(Rsa & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _rsa.swap(obj._rsa);
    }
}

bool Rsa::gen()
{
    return gen(DEFAULT_KEY_LENGTH);
}

bool Rsa::gen(int key_length)
{
    return gen(key_length, RSA_F4);
}

bool Rsa::gen(int key_length, unsigned long e)
{
    assert(key_length > 0);

    std::shared_ptr<BIGNUM> shared_bn;
    if (e > 0) {
        shared_bn.reset(BN_new(), [](BIGNUM * bn){
            BN_free(bn);
        });
        assert(static_cast<bool>(shared_bn));

        // The exponent is an odd number, typically 3, 17 or 65537
        // https://www.openssl.org/docs/man1.1.0/crypto/RSA_generate_key.html
        if (BN_set_word(shared_bn.get(), e) != 1) {
            shared_bn.reset();
        }
    }

    _rsa.reset((FakeRsa*)RSA_new(), [](FakeRsa * rsa){
        RSA_free((RSA*)rsa);
    });
    assert(static_cast<bool>(_rsa));

    if (RSA_generate_key_ex((RSA*)_rsa.get(), key_length, shared_bn.get(), nullptr) != 1) {
        _rsa.reset();
        return false;
    }
    return true;
}

bool Rsa::readPemPublicKey(std::string const & key)
{
    std::shared_ptr<BIO> bio(BIO_new_mem_buf((void*)key.data(), (int)key.size()), [](BIO * bio){
        BIO_free_all(bio);
    });
    assert(static_cast<bool>(bio));

    RSA * rsa = nullptr;
    rsa = PEM_read_bio_RSA_PUBKEY(bio.get(), &rsa, nullptr, nullptr);
    if (rsa == nullptr) {
        return false;
    }

    _rsa.reset((FakeRsa*)rsa, [](FakeRsa * rsa){
        RSA_free((RSA*)rsa);
    });
    assert(static_cast<bool>(_rsa));
    return true;
}

bool Rsa::readPemPrivateKey(std::string const & key)
{
    std::shared_ptr<BIO> bio(BIO_new_mem_buf((void*)key.data(), (int)key.size()), [](BIO * bio){
        BIO_free_all(bio);
    });
    assert(static_cast<bool>(bio));

    RSA * rsa = nullptr;
    rsa = PEM_read_bio_RSAPrivateKey(bio.get(), &rsa, nullptr, nullptr);
    if (rsa == nullptr) {
        return false;
    }

    _rsa.reset((FakeRsa*)rsa, [](FakeRsa * rsa){
        RSA_free((RSA*)rsa);
    });
    assert(static_cast<bool>(_rsa));
    return true;
}

std::string Rsa::getPemPublicKey() const
{
    std::shared_ptr<BIO> bio(BIO_new(BIO_s_mem()), [](BIO * bio){
        BIO_free_all(bio);
    });
    assert(static_cast<bool>(bio));
    if (PEM_write_bio_RSA_PUBKEY(bio.get(), (RSA*)_rsa.get()) != 1) {
        return std::string();
    }

    std::string key;
    key.resize(static_cast<std::size_t>(BIO_pending(bio.get())));
    BIO_read(bio.get(), (void*)key.data(), (int)key.size());
    return key;
}

std::string Rsa::getPemPrivateKey(CipherAlgorithm cipher) const
{
    std::shared_ptr<BIO> bio(BIO_new(BIO_s_mem()), [](BIO * bio){
        BIO_free_all(bio);
    });
    assert(static_cast<bool>(bio));
    auto const * evp_cipher = __get_evp_cipher(cipher);
    if (PEM_write_bio_RSAPrivateKey(bio.get(), (RSA*)_rsa.get(), evp_cipher, nullptr, 0, nullptr, nullptr) != 1) {
        return std::string();
    }

    std::string key;
    key.resize(static_cast<std::size_t>(BIO_pending(bio.get())));
    BIO_read(bio.get(), (void*)key.data(), (int)key.size());
    return key;
}

int Rsa::getMaxDataSize(Padding p) const
{
    auto const RSA_SIZE = RSA_size((RSA*)_rsa.get());
    switch (p) {
    case Padding::P_PKCS1:      return RSA_SIZE - 11;
    case Padding::P_PKCS1_OAEP: return RSA_SIZE - 41;
    case Padding::P_SSLV23:     return 0;
    case Padding::P_NO:         return RSA_SIZE;
    default:                    return 0;
    }
}

std::string Rsa::encryptPublic(std::string const & data, Padding p) const
{
    if (data.size() >= getMaxDataSize(p)) {
        return std::string();
    }

    auto const ENCRYPT_SIZE = RSA_size((RSA*)_rsa.get());
    std::vector<unsigned char> buffer(static_cast<std::size_t>(ENCRYPT_SIZE), 0);

    auto const RESULT = RSA_public_encrypt((int)data.size(), (unsigned char const *)data.data(),
                                           &buffer[0], (RSA*)_rsa.get(), __get_rsa_padding(p));
    if (RESULT == -1) {
        return std::string();
    }
    assert(RESULT <= ENCRYPT_SIZE);
    return std::string(buffer.data(), buffer.data() + RESULT);
}

std::string Rsa::decryptPublic(std::string const & data, Padding p) const
{
    auto const ENCRYPT_SIZE = RSA_size((RSA*)_rsa.get());
    if (data.size() > ENCRYPT_SIZE) {
        return std::string();
    }

    std::vector<unsigned char> buffer(static_cast<std::size_t>(ENCRYPT_SIZE), 0);
    auto const RESULT = RSA_public_decrypt((int)data.size(), (unsigned char const *)data.data(),
                                           &buffer[0], (RSA*)_rsa.get(), __get_rsa_padding(p));
    if (RESULT == -1) {
        return std::string();
    }
    assert(RESULT <= ENCRYPT_SIZE);
    return std::string(buffer.data(), buffer.data() + RESULT);
}

std::string Rsa::encryptPrivate(std::string const & data, Padding p) const
{
    if (!isPrivateKeyEncryptionSupports(p)) {
        return std::string();
    }
    if (data.size() >= getMaxDataSize(p)) {
        return std::string();
    }

    auto const ENCRYPT_SIZE = RSA_size((RSA*)_rsa.get());
    std::vector<unsigned char> buffer(static_cast<std::size_t>(ENCRYPT_SIZE), 0);

    auto const RESULT = RSA_private_encrypt((int)data.size(), (unsigned char const *)data.data(),
                                            &buffer[0], (RSA*)_rsa.get(), __get_rsa_padding(p));
    if (RESULT == -1) {
        return std::string();
    }
    assert(RESULT <= ENCRYPT_SIZE);
    return std::string(buffer.data(), buffer.data() + RESULT);
}

std::string Rsa::decryptPrivate(std::string const & data, Padding p) const
{
    auto const ENCRYPT_SIZE = RSA_size((RSA*)_rsa.get());
    if (data.size() > ENCRYPT_SIZE) {
        return std::string();
    }

    std::vector<unsigned char> buffer(static_cast<std::size_t>(ENCRYPT_SIZE), 0);
    auto const RESULT = RSA_private_decrypt((int)data.size(), (unsigned char const *)data.data(),
                                          &buffer[0], (RSA*)_rsa.get(), __get_rsa_padding(p));
    if (RESULT == -1) {
        return std::string();
    }
    assert(RESULT <= ENCRYPT_SIZE);
    return std::string(buffer.data(), buffer.data() + RESULT);
}

bool Rsa::generatePem(std::string & public_key, std::string & private_key, CipherAlgorithm cipher, int key_length)
{
    Rsa rsa;
    if (!rsa.gen(key_length)) {
        return false;
    }
    public_key = rsa.getPemPublicKey();
    private_key = rsa.getPemPrivateKey(cipher);
    return true;
}

std::string Rsa::generatePemPrivateKey(int key_length)
{
    Rsa rsa;
    if (!rsa.gen(key_length)) {
        return std::string();
    }
    return rsa.getPemPrivateKey();
}

std::string Rsa::generatePemPrivateKey(CipherAlgorithm cipher, int key_length)
{
    Rsa rsa;
    if (!rsa.gen(key_length)) {
        return std::string();
    }
    return rsa.getPemPrivateKey(cipher);
}

} // namespace crypto

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

