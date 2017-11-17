/**
 * @file   Font.hpp
 * @brief  Font class prototype.
 * @author zer0
 * @date   2017-11-16
 *
 * @see <https://en.wikipedia.org/wiki/TrueType>
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TYPOGRAPHY_FONT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TYPOGRAPHY_FONT_HPP__

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

namespace typography {

TBAG_CONSTEXPR int  const ASCII_FONT_TABLE_SIZE = 8;
TBAG_CONSTEXPR char const DEFAULT_ASCII_FONT_TABLE[ASCII_FONT_TABLE_SIZE + 1/*NULL*/] = " .:ioVM@";

TBAG_API std::string getAsciiImage(unsigned char const * true_type, std::size_t size, char c,
                                   char const * table, std::size_t table_size, int scale = 12);
TBAG_API std::string getAsciiImage(util::Buffer const & true_type, char c,
                                   std::string const & table = std::string(DEFAULT_ASCII_FONT_TABLE), int scale = 12);
TBAG_API std::string getAsciiImage(util::Buffer const & true_type, std::string const & text,
                                   std::string const & table = std::string(DEFAULT_ASCII_FONT_TABLE), int scale = 12);

} // namespace typography

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TYPOGRAPHY_FONT_HPP__

