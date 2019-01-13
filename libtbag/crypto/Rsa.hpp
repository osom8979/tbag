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
        CA_UNKNOWN = 0,

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

public:
    struct Impl;
    friend struct Impl;

public:
    using UniqueImpl = std::unique_ptr<Impl>;

public:
    TBAG_CONSTEXPR static int const DEFAULT_KEY_LENGTH = 2014;

private:
    UniqueImpl _impl;

private:
    std::string _public_key;
    std::string _private_key;

public:
    Rsa();
    virtual ~Rsa();

public:
    std::string getPublicKey() { return _public_key; }
    std::string getPrivateKey() { return _private_key; }

public:
    /**
     * @remarks
     *  The exponent is an odd number, typically 3, 17 or 65537
     *
     * @see <https://www.openssl.org/docs/man1.1.0/crypto/RSA_generate_key.html>
     */
    bool setBigNumber(unsigned long e);

public:
    bool gen(int key_length = DEFAULT_KEY_LENGTH);
    bool gen(CipherAlgorithm algorithm, int key_length = DEFAULT_KEY_LENGTH);
    bool gen(std::string const & name, int key_length = DEFAULT_KEY_LENGTH);

public:
    /** Generate PEM(Privacy Enhanced Mail) format string. */
    static bool generatePem(std::string & public_key, std::string & private_key,
                            int key_length = DEFAULT_KEY_LENGTH);
    static bool generatePem(std::string & public_key, std::string & private_key,
                            CipherAlgorithm algorithm, int key_length = DEFAULT_KEY_LENGTH);
    static bool generatePem(std::string & public_key, std::string & private_key,
                            std::string const & name, int key_length = DEFAULT_KEY_LENGTH);
};

} // namespace crypto

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CRYPTO_RSA_HPP__

