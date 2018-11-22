/**
 * @file   PercentEncoding.hpp
 * @brief  PercentEncoding class prototype.
 * @author zer0
 * @date   2018-11-22
 *
 * @remarks
 *  a.k.a URL Encoding.
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_CODEC_PERCENTENCODING_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_CODEC_PERCENTENCODING_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace codec {

/**
 * RFC 3986 section 2.2 Reserved Characters (January 2005)
 */
TBAG_API bool isReservedCharacters(char c) TBAG_NOEXCEPT;

/**
 * RFC 3986 section 2.3 Unreserved Characters (January 2005)
 */
TBAG_API bool isUnreservedCharacters(char c) TBAG_NOEXCEPT;

TBAG_API bool isPlainCharacters(char c) TBAG_NOEXCEPT;
TBAG_API bool isEncodeCharacters(char c) TBAG_NOEXCEPT;

TBAG_API std::string encodePercent(std::string const & text);
TBAG_API std::string decodePercent(std::string const & text);

} // namespace codec

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CODEC_PERCENTENCODING_HPP__

