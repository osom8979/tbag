/**
 * @file   Pbkdf2.hpp
 * @brief  Pbkdf2 class prototype.
 * @author zer0
 * @date   2017-12-07
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_CRYPTO_PBKDF2_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_CRYPTO_PBKDF2_HPP__

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

/**
 * RFC 2898 suggests an iteration count of at least 1000.
 */
TBAG_CONSTEXPR int const PBKDF2_MINIMUM_ITERATION_COUNT = 1000;

enum class Pbkdf2Type
{
    PT_MD_NULL = 0,
    PT_MD5,
    PT_SHA1,
    PT_SHA224,
    PT_SHA256,
    PT_SHA384,
    PT_SHA512,
};

TBAG_API util::Buffer generatePbkdf2(std::string const & password, util::Buffer const & salt, std::size_t size,
                                     Pbkdf2Type type = Pbkdf2Type::PT_SHA512,
                                     int iteration_count = PBKDF2_MINIMUM_ITERATION_COUNT);
TBAG_API util::Buffer generatePbkdf2(std::string const & password, std::string const & salt, std::size_t size,
                                     Pbkdf2Type type = Pbkdf2Type::PT_SHA512,
                                     int iteration_count = PBKDF2_MINIMUM_ITERATION_COUNT);

TBAG_API std::string generatePbkdf2String(std::string const & password, util::Buffer const & salt, std::size_t size,
                                          Pbkdf2Type type = Pbkdf2Type::PT_SHA512,
                                          int iteration_count = PBKDF2_MINIMUM_ITERATION_COUNT);
TBAG_API std::string generatePbkdf2String(std::string const & password, std::string const & salt, std::size_t size,
                                          Pbkdf2Type type = Pbkdf2Type::PT_SHA512,
                                          int iteration_count = PBKDF2_MINIMUM_ITERATION_COUNT);

} // namespace crypto

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CRYPTO_PBKDF2_HPP__

