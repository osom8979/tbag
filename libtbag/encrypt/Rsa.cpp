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

#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace encrypt {

// ---------------
namespace __impl {
// ---------------

static EVP_CIPHER const * getEvpCipher(Rsa::CipherAlgorithm algorithm)
{
    // @formatter:off
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
    case Rsa::CipherAlgorithm::CA_UNKNOWN:
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

// ------------------
} // namespace __impl
// ------------------

/**
 * Rsa implementation.
 *
 * @author zer0
 * @date   2017-11-12
 */
struct Rsa::Impl : private Noncopyable
{
    Rsa * parent;

    BIO * public_out;
    BIO * private_out;

    BIGNUM * bn;
    RSA * rsa;

    Impl(Rsa * p) : parent(p)
    {
        public_out = BIO_new(BIO_s_mem());
        private_out = BIO_new(BIO_s_mem());
        bn = BN_new();
        rsa = RSA_new();

        assert(public_out != nullptr);
        assert(private_out != nullptr);
        assert(bn != nullptr);
        assert(rsa != nullptr);

        if (BN_set_word(bn, RSA_F4) != 1) {
            throw std::bad_alloc();
        }
    }

    ~Impl()
    {
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
    }

    bool setBigNumber(unsigned long e = RSA_F4)
    {
        return BN_set_word(bn, e) == 1;
    }

    bool generate(int key_length, EVP_CIPHER const * enc,
                  std::vector<char> & public_key, std::vector<char> & private_key)
    {
        if (RSA_generate_key_ex(rsa, key_length, bn, nullptr) != 1) {
            return false;
        }
        if (PEM_write_bio_RSA_PUBKEY(public_out, rsa) != 1) {
            return false;
        }
        if (PEM_write_bio_RSAPrivateKey(private_out, rsa, enc, nullptr, 0, nullptr, nullptr) != 1) {
            return false;
        }

        int const PUBLIC_LENGTH = BIO_pending(public_out);
        int const PRIVATE_LENGTH = BIO_pending(private_out);

        public_key.resize(static_cast<std::size_t>(PUBLIC_LENGTH));
        private_key.resize(static_cast<std::size_t>(PRIVATE_LENGTH));

        BIO_read( public_out, public_key.data(), PUBLIC_LENGTH);
        BIO_read(private_out, private_key.data(), PRIVATE_LENGTH);

        return true;
    }

    bool generate(int key_length, EVP_CIPHER const * enc,
                  std::string & public_key, std::string & private_key)
    {
        std::vector<char> public_temp;
        std::vector<char> private_temp;
        bool const RESULT = generate(key_length, enc, public_temp, private_temp);
        if (RESULT) {
            public_key.assign(public_temp.begin(), public_temp.end());
            private_key.assign(private_temp.begin(), private_temp.end());
        }
        return RESULT;
    }
};

// -------------------
// Rsa implementation.
// -------------------

Rsa::Rsa() : _impl(new Impl(this))
{
    assert(_impl != nullptr);
}

Rsa::~Rsa()
{
    // EMPTY.
}

bool Rsa::setBigNumber(unsigned long e)
{
    return _impl->setBigNumber(e);
}

bool Rsa::gen(int key_length)
{
    return _impl->generate(key_length, nullptr, _public_key, _private_key);
}

bool Rsa::gen(CipherAlgorithm algorithm, int key_length)
{
    return _impl->generate(key_length, __impl::getEvpCipher(algorithm), _public_key, _private_key);
}

bool Rsa::gen(std::string const & name, int key_length)
{
    return _impl->generate(key_length, __impl::getEvpCipherByName(name), _public_key, _private_key);
}

// ---------------
// Static methods.
// ---------------

bool Rsa::generate(std::string & public_key, std::string & private_key, int key_length)
{
    Rsa rsa;
    if (rsa.gen(key_length)) {
        public_key = rsa.getPublicKey();
        private_key = rsa.getPrivateKey();
        return true;
    }
    return false;
}

bool Rsa::generate(std::string & public_key, std::string & private_key, CipherAlgorithm algorithm, int key_length)
{
    Rsa rsa;
    if (rsa.gen(algorithm, key_length)) {
        public_key = rsa.getPublicKey();
        private_key = rsa.getPrivateKey();
        return true;
    }
    return false;
}

bool Rsa::generate(std::string & public_key, std::string & private_key, std::string const & name, int key_length)
{
    Rsa rsa;
    if (rsa.gen(name, key_length)) {
        public_key = rsa.getPublicKey();
        private_key = rsa.getPrivateKey();
        return true;
    }
    return false;
}

} // namespace encrypt

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

