/**
 * @file   Md5.hpp
 * @brief  Md5 class prototype.
 * @author zer0
 * @date   2017-08-15
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_ENCRYPT_MD5_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_ENCRYPT_MD5_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <array>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace encrypt {

TBAG_CONSTEXPR int const MD5_DIGEST_BYTE = 16;

using Md5Hash = std::array<unsigned char, MD5_DIGEST_BYTE>;

TBAG_API bool encryptMd5(std::string const & input, Md5Hash & output);
TBAG_API bool encryptMd5(std::string const & input, std::string & output);

} // namespace encrypt

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ENCRYPT_MD5_HPP__

