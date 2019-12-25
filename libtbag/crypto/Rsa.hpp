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
#include <utility>

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
         *
         * The message must be no longer than the length of the public modulus minus 11 bytes.
         */
        P_PKCS1,

        /**
         * EME-OAEP as defined in PKCS #1 v2.0 with SHA-1, MGF1 and an empty encoding parameter.
         * This mode is recommended for all new applications.
         *
         * The message must be no longer than the length of the public modulus minus twice the hash length,
         * minus a further 2.
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
    /**
     * @see <https://www.openssl.org/docs/manmaster/man3/RSA_public_encrypt.html>
     *
     */
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

/**
 * PublicKey class prototype.
 *
 * @author zer0
 * @date   2019-12-21
 */
class PublicKey : private Noncopyable
{
public:
    using Padding = Rsa::Padding;

private:
    std::string _key;
    Rsa _rsa;

public:
    PublicKey()
    { /* EMPTY. */ }
    PublicKey(PublicKey && obj) TBAG_NOEXCEPT
            : _key(std::move(obj._key)), _rsa(std::move(obj._rsa))
    { /* EMPTY. */ }
    ~PublicKey()
    { /* EMPTY. */ }

public:
    PublicKey & operator =(PublicKey && obj) TBAG_NOEXCEPT
    {
        if (this != &obj) {
            _key = std::move(obj._key);
            _rsa = std::move(obj._rsa);
        }
        return *this;
    }

public:
    void swap(PublicKey & obj) TBAG_NOEXCEPT
    {
        if (this != &obj) {
            _key.swap(obj._key);
            _rsa.swap(obj._rsa);
        }
    }

    friend void swap(PublicKey & lh, PublicKey & rh) TBAG_NOEXCEPT
    {
        lh.swap(rh);
    }

public:
    std::string const & key() const TBAG_NOEXCEPT
    {
        return _key;
    }

    bool exists() const TBAG_NOEXCEPT_SP_OP(_key.empty())
    {
        return !_key.empty();
    }

    int getMaxDataSize(Padding p) const
    {
        return _rsa.getMaxDataSize(p);
    }

    bool readPem(std::string const & key)
    {
        if (!_rsa.readPemPublicKey(key)) {
            return false;
        }
        _key = key;
        return true;
    }

    std::string encrypt(std::string const & data, Padding p = Padding::P_PKCS1) const
    {
        return _rsa.encryptPublic(data, p);
    }

    std::string decrypt(std::string const & data, Padding p = Padding::P_PKCS1) const
    {
        return _rsa.decryptPublic(data, p);
    }
};

/**
 * PrivateKey class prototype.
 *
 * @author zer0
 * @date   2019-12-21
 */
class PrivateKey : private Noncopyable
{
public:
    using Padding = Rsa::Padding;

private:
    std::string _key;
    Rsa _rsa;

public:
    PrivateKey()
    { /* EMPTY. */ }
    PrivateKey(PrivateKey && obj) TBAG_NOEXCEPT
            : _key(std::move(obj._key)), _rsa(std::move(obj._rsa))
    { /* EMPTY. */ }
    ~PrivateKey()
    { /* EMPTY. */ }

public:
    PrivateKey & operator =(PrivateKey && obj) TBAG_NOEXCEPT
    {
        if (this != &obj) {
            _key = std::move(obj._key);
            _rsa = std::move(obj._rsa);
        }
        return *this;
    }

public:
    void swap(PrivateKey & obj) TBAG_NOEXCEPT
    {
        if (this != &obj) {
            _key.swap(obj._key);
            _rsa.swap(obj._rsa);
        }
    }

    friend void swap(PrivateKey & lh, PrivateKey & rh) TBAG_NOEXCEPT
    {
        lh.swap(rh);
    }

public:
    std::string const & key() const TBAG_NOEXCEPT
    {
        return _key;
    }

    bool exists() const TBAG_NOEXCEPT_SP_OP(_key.empty())
    {
        return !_key.empty();
    }

    int getMaxDataSize(Padding p) const
    {
        return _rsa.getMaxDataSize(p);
    }

    bool readPem(std::string const & key)
    {
        if (!_rsa.readPemPrivateKey(key)) {
            return false;
        }
        _key = key;
        return true;
    }

    std::string encrypt(std::string const & data, Padding p = Padding::P_PKCS1) const
    {
        return _rsa.encryptPrivate(data, p);
    }

    std::string decrypt(std::string const & data, Padding p = Padding::P_PKCS1) const
    {
        return _rsa.decryptPrivate(data, p);
    }
};

TBAG_API std::pair<PublicKey, PrivateKey> generateKeys(
        Rsa::CipherAlgorithm cipher = Rsa::CipherAlgorithm::CA_NONE,
        int key_length = Rsa::DEFAULT_KEY_LENGTH);

} // namespace crypto

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CRYPTO_RSA_HPP__

