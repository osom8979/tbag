/**
 * @file   Sha.hpp
 * @brief  Sha class prototype.
 * @author zer0
 * @date   2017-12-07
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_CRYPTO_SHA_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_CRYPTO_SHA_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <string>
#include <array>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace crypto {

TBAG_CONSTEXPR int const SHA1_OUTPUT_BIT  = 160;
TBAG_CONSTEXPR int const SHA1_OUTPUT_BYTE = (SHA1_OUTPUT_BIT / 8);

using Sha1Hash = std::array<unsigned char, SHA1_OUTPUT_BYTE>;

TBAG_API bool encryptSha1(std::string const & input, Sha1Hash & output);
TBAG_API bool encryptSha1(std::string const & input, std::string & output);

} // namespace crypto

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CRYPTO_SHA_HPP__

