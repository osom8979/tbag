/**
 * @file   PseudoRandom.hpp
 * @brief  PseudoRandom class prototype.
 * @author zer0
 * @date   2017-12-07
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_CRYPTO_PSEUDORANDOM_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_CRYPTO_PSEUDORANDOM_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/util/BufferInfo.hpp>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace crypto {

TBAG_API libtbag::util::Buffer generateRandom(std::size_t size);
TBAG_API libtbag::util::Buffer generatePseudoRandom(std::size_t size);

TBAG_API std::string generateRandomString(std::size_t size);
TBAG_API std::string generatePseudoRandomString(std::size_t size);

} // namespace crypto

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CRYPTO_PSEUDORANDOM_HPP__

