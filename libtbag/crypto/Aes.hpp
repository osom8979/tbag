/**
 * @file   Aes.hpp
 * @brief  Aes class prototype.
 * @author zer0
 * @date   2020-03-05
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_CRYPTO_AES_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_CRYPTO_AES_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/ErrPair.hpp>
#include <libtbag/util/BufferInfo.hpp>

#include <cstdint>
#include <array>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace crypto {

TBAG_CONSTEXPR std::size_t const AES_256BIT_TO_BYTE = (256/8);
using KeyArray = std::array<std::uint8_t, AES_256BIT_TO_BYTE>;

TBAG_CONSTEXPR std::size_t const IV_128BIT_TO_BYTE = (128/8);
using IvArray = std::array<std::uint8_t, IV_128BIT_TO_BYTE>;

using Buffer = libtbag::util::Buffer;
using ErrBuffer = libtbag::ErrPair<Buffer>;
using ErrString = libtbag::ErrPair<std::string>;

TBAG_API ErrBuffer encryptAes256Cbc(KeyArray const & key, IvArray const & iv, Buffer const & input);
TBAG_API ErrBuffer decryptAes256Cbc(KeyArray const & key, IvArray const & iv, Buffer const & input);

TBAG_API ErrString encryptAes256CbcToBase64(KeyArray const & key, IvArray const & iv, std::string const & input);
TBAG_API ErrString decryptBase64ToAes256Cbc(KeyArray const & key, IvArray const & iv, std::string const & input);

} // namespace crypto

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CRYPTO_AES_HPP__

