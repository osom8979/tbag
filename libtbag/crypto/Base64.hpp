/**
 * @file   Base64.hpp
 * @brief  Base64 class prototype.
 * @author zer0
 * @date   2017-12-07
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_CRYPTO_BASE64_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_CRYPTO_BASE64_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/util/BufferInfo.hpp>

#include <cstdint>
#include <string>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace crypto {

/** Calculates the length of a decoded string. */
TBAG_API std::size_t getDecodeLength(std::string const & base64);

TBAG_API bool encodeBase64(std::string const & input, std::string & output);
TBAG_API bool decodeBase64(std::string const & input, std::string & output);

TBAG_API bool encodeBase64(char const * input, std::size_t size, std::string & output);
TBAG_API bool encodeBase64(util::Buffer const & input, std::string & output);
TBAG_API bool decodeBase64(std::string const & input, util::Buffer & output);

} // namespace crypto

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CRYPTO_BASE64_HPP__

