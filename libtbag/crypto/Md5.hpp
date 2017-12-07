/**
 * @file   Md5.hpp
 * @brief  Md5 class prototype.
 * @author zer0
 * @date   2017-12-07
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_CRYPTO_MD5_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_CRYPTO_MD5_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/util/BufferInfo.hpp>

#include <array>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace crypto {

TBAG_CONSTEXPR int const MD5_DIGEST_BYTE = 16;

using Md5Hash = std::array<unsigned char, MD5_DIGEST_BYTE>;

TBAG_API bool encryptMd5(char const * input, std::size_t size, Md5Hash & output);

TBAG_API bool encryptMd5(util::Buffer const & input, Md5Hash & output);
TBAG_API bool encryptMd5(util::Buffer const & input, util::Buffer & output);
TBAG_API bool encryptMd5(util::Buffer const & input, std::string & output);

TBAG_API bool encryptMd5(std::string const & input, Md5Hash & output);
TBAG_API bool encryptMd5(std::string const & input, util::Buffer & output);
TBAG_API bool encryptMd5(std::string const & input, std::string & output);

TBAG_API std::string getMd5(util::Buffer const & input);
TBAG_API std::string getMd5(std::string const & input);

TBAG_API std::string getMd5FromFile(std::string const & file_path);

} // namespace crypto

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CRYPTO_MD5_HPP__

