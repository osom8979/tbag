/**
 * @file   Rsa.hpp
 * @brief  Rsa class prototype.
 * @author zer0
 * @date   2017-12-07
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_CRYPTO_RSA_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_CRYPTO_RSA_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

#include <vector>
#include <string>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace crypto {

/**
 * Rsa class prototype.
 *
 * @author zer0
 * @date   2017-11-11
 * @date   2017-12-07 (Move package: libtbag/encrypt -> libtbag/crypto)
 */
class TBAG_API Rsa : private Noncopyable
{
public:
    enum class CipherAlgorithm
    {
        CA_NONE = 0,

        CA_DES,  ///< Encrypt the generated key with DES in cbc mode.
        CA_DES3, ///< Encrypt the generated key with DES in ede cbc mode (168 bit key).
        CA_IDEA, ///< Encrypt the generated key with IDEA in cbc mode.

        CA_AES128, ///< Encrypt PEM output with cbc aes128.
        CA_AES192, ///< Encrypt PEM output with cbc aes192.
        CA_AES256, ///< Encrypt PEM output with cbc aes256.

        CA_CAMELLIA128, ///< Encrypt PEM output with cbc camellia128.
        CA_CAMELLIA192, ///< Encrypt PEM output with cbc camellia192.
        CA_CAMELLIA256, ///< Encrypt PEM output with cbc camellia256.
    };

    enum class Padding
    {
        /**
         * PKCS #1 v1.5 padding.
         * This currently is the most widely used mode.
         */
        P_PKCS1,

        /**
         * EME-OAEP as defined in PKCS #1 v2.0 with SHA-1, MGF1 and an empty encoding parameter.
         * This mode is recommended for all new applications.
         */
        P_PKCS1_OAEP,

        /**
         * PKCS #1 v1.5 padding with an SSL-specific modification that denotes that the server is SSL3 capable.
         */
        P_SSLV23,

        /**
         * Raw RSA encryption.
         * This mode should only be used to implement cryptographically sound padding modes in the application code.
         * Encrypting user data directly with RSA is insecure.
         */
        P_NO,
    };

public:
    static bool isPrivateKeyEncryptionSupports(Padding p) TBAG_NOEXCEPT
    {
        return (p == Padding::P_PKCS1) || (p == Padding::P_NO);
    }

public:
    TBAG_CONSTEXPR static int const DEFAULT_KEY_LENGTH = 2048;

public:
    using FakeRsa = std::shared_ptr<void>;

private:
    FakeRsa _rsa;

public:
    Rsa();
    Rsa(Rsa && obj) TBAG_NOEXCEPT;
    ~Rsa();

public:
    Rsa & operator =(Rsa && obj) TBAG_NOEXCEPT;

public:
    void swap(Rsa & obj) TBAG_NOEXCEPT;
    inline friend void swap(Rsa & lh, Rsa & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }

public:
    bool gen();
    bool gen(int key_length);
    bool gen(int key_length, unsigned long e);

public:
    bool readPemPublicKey(std::string const & key);
    bool readPemPrivateKey(std::string const & key);

public:
    std::string getPemPublicKey() const;
    std::string getPemPrivateKey(CipherAlgorithm cipher = CipherAlgorithm::CA_NONE) const;

public:
    int getMaxDataSize(Padding p) const;

public:
    std::string encryptPublic(std::string const & data, Padding p = Padding::P_PKCS1) const;
    std::string decryptPublic(std::string const & data, Padding p = Padding::P_PKCS1) const;

    std::string encryptPrivate(std::string const & data, Padding p = Padding::P_PKCS1) const;
    std::string decryptPrivate(std::string const & data, Padding p = Padding::P_PKCS1) const;

public:
    /** Generate PEM(Privacy Enhanced Mail) format string. */
    static bool generatePem(std::string & public_key,
                            std::string & private_key,
                            CipherAlgorithm cipher = CipherAlgorithm::CA_NONE,
                            int key_length = DEFAULT_KEY_LENGTH);
    static std::string generatePemPrivateKey(int key_length = DEFAULT_KEY_LENGTH);
    static std::string generatePemPrivateKey(CipherAlgorithm cipher, int key_length = DEFAULT_KEY_LENGTH);
};

} // namespace crypto

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CRYPTO_RSA_HPP__

