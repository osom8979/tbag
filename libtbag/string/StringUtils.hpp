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
#include <libtbag/Err.hpp>
#include <libtbag/Type.hpp>
#include <libtbag/3rd/fmt/format.h>

#include <cstdarg>
#include <string>
#include <sstream>
#include <vector>
#include <array>
#include <regex>
#include <thread>
#include <type_traits>
#include <utility>

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

template <typename ... Args>
std::string fformat(std::string const & f, Args && ... args)
{
    return ::fmt::format(f, std::forward<Args>(args) ...);
}

TBAG_CONSTEXPR std::size_t const DEFAULT_FORMAT_BUFFER_SIZE = 1024;
TBAG_CONSTEXPR std::size_t const MAX_FORMAT_RESIZE_LOOP_COUNT = 5;

TBAG_API std::string  format(char const * f, ...);
TBAG_API std::string vformat(char const * f, std::size_t buffer_size, va_list & l);

#ifndef TBAG_FORMAT_VA_LIST
#define TBAG_FORMAT_VA_LIST(result, format_string, last_argument_name, buffer_size)     \
    do {                                                                                \
        va_list __args_list__;                                                          \
        va_start(__args_list__, last_argument_name);                                    \
        result = ::libtbag::string::vformat(format_string, buffer_size, __args_list__); \
        va_end(__args_list__);                                                          \
    } while (0)
#endif

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

TBAG_API std::string mergeTokens(std::vector<std::string> const & tokens, std::string const & delimiter);

/**
 * Thread ID to string.
 */
TBAG_API std::string convertStringWithThreadId(std::thread::id const & id);

/**
 * Byte array to HEX string.
 */
TBAG_API char convertHalfByteToHexChar(uint8_t half_byte);
TBAG_API std::string convertByteToHexString(uint8_t hex);
TBAG_API std::string convertByteArrayToHexString(std::vector<uint8_t> const & bytes, std::string const & prefix = "0x", std::string const & separator = "");

TBAG_CONSTEXPR std::size_t const HEX_STRING_ADDRESS_BYTE_SIZE =
        (/*PREFIX(0x)*/2) + (sizeof(void*) * 2/*HEX STRING ADDRESS*/) + (1/*NULL*/);
using AddressHexString = std::array<char, HEX_STRING_ADDRESS_BYTE_SIZE>;

/**
 * HEX string to Byte array.
 */
TBAG_API Err convertHexCharToHalfByte(char hex_char, uint8_t & result);
TBAG_API Err convertHexStringToByte(char high_char, char low_char, uint8_t & result);

/**
 * Address to HEX string.
 */
TBAG_API AddressHexString convertAddressToHexString(void const * address) TBAG_NOEXCEPT;
TBAG_API std::string convertAddressHexStringToString(AddressHexString const & address);

/**
 * Regex based token.
 */
TBAG_API std::vector<std::string> splitMatch(std::string const & source, std::regex const & match);
TBAG_API std::vector<std::string> splitMatch(std::string const & source, std::string const & match);

// Regexp utilities.
TBAG_API std::string replaceRegex(std::string const & source, std::string const & regex, std::string const & replace);
TBAG_API std::string removeRegex(std::string const & source, std::string const & regex);

TBAG_API bool isMatch(std::string const & source, std::regex const & regex);
TBAG_API bool isMatch(std::string const & source, std::string const & regex);
TBAG_API bool isUtf8Match(std::string const & utf8_source, std::string const & regex);

TBAG_API std::string trimLeft(std::string const & str);
TBAG_API std::string trimRight(std::string const & str);
TBAG_API std::string trim(std::string const & str);

TBAG_API bool createRandomString(char * buffer, std::size_t size);

TBAG_API std::string lower(std::string const & str);
TBAG_API std::string upper(std::string const & str);

} // namespace string

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_STRINGUTILS_HPP__

