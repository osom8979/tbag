/**
 * @file   Rsa.hpp
 * @brief  Rsa class prototype.
 * @author zer0
 * @date   2017-11-11
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_ENCRYPT_RSA_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_ENCRYPT_RSA_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace encrypt {

enum class EvpCipherAlgorithm
{
    ECA_UNKNOWN = 0,

    ECA_DES,  ///< Encrypt the generated key with DES in cbc mode.
    ECA_DES3, ///< Encrypt the generated key with DES in ede cbc mode (168 bit key).
    ECA_IDEA, ///< Encrypt the generated key with IDEA in cbc mode.

    ECA_AES128, ///< Encrypt PEM output with cbc aes128.
    ECA_AES192, ///< Encrypt PEM output with cbc aes192.
    ECA_AES256, ///< Encrypt PEM output with cbc aes256.

    ECA_CAMELLIA128, ///< Encrypt PEM output with cbc camellia128.
    ECA_CAMELLIA192, ///< Encrypt PEM output with cbc camellia192.
    ECA_CAMELLIA256, ///< Encrypt PEM output with cbc camellia256.
};

TBAG_API bool generateRsa(std::vector<char> & public_key, std::vector<char> & private_key, int key_length = 2048,
                          EvpCipherAlgorithm algorithm = EvpCipherAlgorithm::ECA_UNKNOWN);

/**
 * Rsa class prototype.
 *
 * @author zer0
 * @date   2017-11-11
 */
class TBAG_API Rsa : private Noncopyable
{
public:
    Rsa();
    virtual ~Rsa();
};

} // namespace encrypt

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ENCRYPT_RSA_HPP__

