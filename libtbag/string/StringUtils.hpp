/**
 * @file   StringUtils.hpp
 * @brief  String utilities prototype.
 * @author zer0
 * @date   2016-04-04
 * @date   2016-12-05 (Rename: Strings -> StringUtils)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_STRINGUTILS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_STRINGUTILS_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#define CHECK_GNUC_CXX_REGEX

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Type.hpp>

#include <string>
#include <sstream>
#include <vector>
#include <regex>
#include <thread>
#include <type_traits>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace string {

TBAG_CONSTEXPR char const CHAR_SPACE  = ' ';
TBAG_CONSTEXPR char const CHAR_TAB    = '\t';
TBAG_CONSTEXPR char const CHAR_RETURN = '\n';

template <typename T>
TBAG_CONSTEXPR T const * charOrWidechar(char const * c, wchar_t const * w);

template <>
inline TBAG_CONSTEXPR char const * charOrWidechar<char>(char const * c, wchar_t const * w)
{
    return c;
}

template <>
inline TBAG_CONSTEXPR wchar_t const * charOrWidechar<wchar_t>(char const * c, wchar_t const * w)
{
    return w;
}

#ifndef CHAR_OR_WIDECHAR
#define CHAR_OR_WIDECHAR(type, str) ::libtbag::string::charOrWidechar<type>(str, L##str)
#endif

template <typename FloatingType>
std::string convertStringWithFloatingPoint(FloatingType floating, int precision = 2)
{
    static_assert(std::is_floating_point<FloatingType>::value, "Not floating point type.");

    std::stringstream ss;
    ss.setf(std::ios_base::showpoint);
    ss.setf(std::ios_base::fixed, std::ios_base::floatfield);
    ss.precision(precision);
    ss << floating;
    return ss.str();
}

/**
 * Separate tokens.
 *
 * @tparam CharType    Character type.
 * @tparam StringType  std::string type (Don't change this typename).
 *
 * @param source    [in] Original string.
 * @param delimiter [in] Delimiter string.
 *
 * @return
 *  Token vector.
 */
TBAG_API std::vector<std::string> splitTokens(std::string const & source, std::string const & delimiter);

TBAG_API std::vector<std::string> splitUtf8Tokens(std::string const & utf8_source, std::string const & utf8_delimiter);

/**
 * Thread ID to string.
 */
TBAG_API std::string convertStringWithThreadId(std::thread::id const & id);

/**
 * Byte array to HEX string.
 */
TBAG_API char convertHalfByteToHexChar(uint8_t half_byte);
TBAG_API std::string convertByteToHexString(uint8_t hex);
TBAG_API std::string convertByteArrayToHexString(std::vector<uint8_t> const & bytes, std::string const & prefix = "0x");

/**
 * Regex based token.
 */
TBAG_API std::vector<std::string> splitMatch(std::string const & source, std::regex const & match);
TBAG_API std::vector<std::string> splitMatch(std::string const & source, std::string const & match);

// Regexp utilities.
TBAG_API std::string replaceRegex(std::string const & path, std::string const & regex, std::string const & replace);
TBAG_API std::string removeRegex(std::string const & path, std::string const & regex);

TBAG_API bool isMatch(std::string const & original, std::regex const & regex);
TBAG_API bool isMatch(std::string const & original, std::string const & regex);

TBAG_API std::string trimLeft(std::string const & str);
TBAG_API std::string trimRight(std::string const & str);
TBAG_API std::string trim(std::string const & str);

TBAG_API bool createRandomString(char * buffer, std::size_t size);

// -----------------------
// UTF-8 STRING UTILITIES.
// -----------------------

namespace utf8 {

} // namespace utf8

} // namespace string

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_STRINGUTILS_HPP__

