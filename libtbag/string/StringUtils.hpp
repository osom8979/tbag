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
#include <libtbag/util/BufferInfo.hpp>

#include <cstdint>
#include <algorithm>
#include <string>
#include <sstream>
#include <vector>
#include <array>
#include <regex>
#include <thread>
#include <type_traits>
#include <iterator>
#include <utility>

#define TBAG_WINDOWS_NEW_LINE  "\r\n"
#define TBAG_UNIX_NEW_LINE     "\n"

#if defined(TBAG_PLATFORM_WINDOWS)
#define TBAG_NEW_LINE TBAG_WINDOWS_NEW_LINE
#else
#define TBAG_NEW_LINE TBAG_UNIX_NEW_LINE
#endif

#define TBAG_EMPTY_STRING       ""
#define TBAG_EMPTY_STRING_WIDE  L""
#define TBAG_SPACE_STRING       " "

#define TBAG_STRCMP1(l, r)  l[0] == r[0]
#define TBAG_STRCMP2(l, r)  TBAG_STRCMP1(l, r)  && l[1] == r[1]
#define TBAG_STRCMP3(l, r)  TBAG_STRCMP2(l, r)  && l[2] == r[2]
#define TBAG_STRCMP4(l, r)  TBAG_STRCMP3(l, r)  && l[3] == r[3]
#define TBAG_STRCMP5(l, r)  TBAG_STRCMP4(l, r)  && l[4] == r[4]
#define TBAG_STRCMP6(l, r)  TBAG_STRCMP5(l, r)  && l[5] == r[5]
#define TBAG_STRCMP7(l, r)  TBAG_STRCMP6(l, r)  && l[6] == r[6]
#define TBAG_STRCMP8(l, r)  TBAG_STRCMP7(l, r)  && l[7] == r[7]
#define TBAG_STRCMP9(l, r)  TBAG_STRCMP8(l, r)  && l[8] == r[8]
#define TBAG_STRCMP10(l, r) TBAG_STRCMP9(l, r)  && l[9] == r[9]
#define TBAG_STRCMP11(l, r) TBAG_STRCMP10(l, r) && l[10] == r[10]
#define TBAG_STRCMP12(l, r) TBAG_STRCMP11(l, r) && l[11] == r[11]
#define TBAG_STRCMP13(l, r) TBAG_STRCMP12(l, r) && l[12] == r[12]
#define TBAG_STRCMP14(l, r) TBAG_STRCMP13(l, r) && l[13] == r[13]
#define TBAG_STRCMP15(l, r) TBAG_STRCMP14(l, r) && l[14] == r[14]

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace string {

TBAG_CONSTEXPR char const * const WINDOWS_NEW_LINE = TBAG_WINDOWS_NEW_LINE;
TBAG_CONSTEXPR char const * const UNIX_NEW_LINE = TBAG_UNIX_NEW_LINE;
TBAG_CONSTEXPR char const * const NEW_LINE = TBAG_NEW_LINE;

TBAG_CONSTEXPR char const CHAR_NULL = '\0';
TBAG_CONSTEXPR char const CHAR_ALERT = '\a';
TBAG_CONSTEXPR char const CHAR_BACKSPACE = '\b';
TBAG_CONSTEXPR char const CHAR_FORM_FEED = '\f';
TBAG_CONSTEXPR char const CHAR_PAGE_BREAK = CHAR_FORM_FEED;
TBAG_CONSTEXPR char const CHAR_NEWLINE = '\n';
TBAG_CONSTEXPR char const CHAR_LINE_FEED = CHAR_NEWLINE;
TBAG_CONSTEXPR char const CHAR_CARRIAGE_RETURN = '\r';
TBAG_CONSTEXPR char const CHAR_HORIZONTAL_TAB = '\t';
TBAG_CONSTEXPR char const CHAR_VERTICAL_TAB = '\v';
TBAG_CONSTEXPR char const CHAR_BACKSLASH = '\\';
TBAG_CONSTEXPR char const CHAR_APOSTROPHE = '\'';
TBAG_CONSTEXPR char const CHAR_SINGLE_QUOTATION_MARK = CHAR_APOSTROPHE;
TBAG_CONSTEXPR char const CHAR_DOUBLE_QUOTATION_MARK = '\"';
TBAG_CONSTEXPR char const CHAR_QUESTION_MARK = '\?';

TBAG_CONSTEXPR char const CHAR_SPACE  = ' ';
TBAG_CONSTEXPR char const CHAR_TAB    = CHAR_HORIZONTAL_TAB;
TBAG_CONSTEXPR char const CHAR_RETURN = CHAR_NEWLINE;

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

template <typename CharT>
inline TBAG_CONSTEXPR std::size_t string_length(CharT const * text)
{
    return (text == nullptr || *text == '\0') ? 0U : 1U + string_length(text + 1U);
}

template <typename CharT>
inline TBAG_CONSTEXPR bool string_equal(CharT const * lt, CharT const * rt)
{
    if (lt == nullptr || rt == nullptr) {
        return false;
    }
    if (*lt == '\0' && *rt == '\0') {
        return true;
    }
    if (*lt == *rt) {
        return string_equal(lt+1, rt+1);
    } else {
        return false;
    }
}

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

enum class LineFeedStyle
{
    LFS_NONE,
    LFS_UNIX,
    LFS_WINDOWS,
    LFS_AUTO,
};

TBAG_API LineFeedStyle getLineFeedStyle(std::string const & name);
TBAG_API char const * getLineFeedStyleText(LineFeedStyle style) TBAG_NOEXCEPT;

TBAG_API std::string getLineFeed(LineFeedStyle style = LineFeedStyle::LFS_AUTO);
TBAG_API std::string getStdEndLine();
TBAG_API bool appendLineFeed(LineFeedStyle style, char * buffer, int buffer_size, int offset) TBAG_NOEXCEPT;

/**
 * Separate tokens.
 *
 * @tparam CharType    Character type.
 * @tparam StringType  std::string type (Don't change this typename).
 *
 * @param[in] source       Original string.
 * @param[in] delimiter    Delimiter string.
 * @param[in] remove_empty Remove all empty tokens.
 *
 * @return
 *  Token vector.
 */
TBAG_API std::vector<std::string> splitTokens(std::string const & source, std::string const & delimiter, bool remove_empty = true);
TBAG_API std::vector<std::string> splitUtf8Tokens(std::string const & utf8_source, std::string const & utf8_delimiter, bool remove_empty = true);
TBAG_API std::vector<std::string> splitSize(std::string const & source, std::size_t size);

TBAG_API std::pair<std::string, std::string> divideTwo(std::string const & source, std::string const & delimiter);

template <typename Iterator>
std::string mergeTokens(Iterator begin, Iterator end, std::string const & delimiter)
{
    if (begin == end) {
        return {};
    }
    if (std::distance(begin, end) == 1) {
        return *begin;
    }
    std::stringstream ss;
    ss << *begin;
    ++begin;
    for (; begin != end; ++begin) {
        ss << delimiter << *begin;
    }
    return ss.str();
}

TBAG_API std::string mergeTokens(std::vector<std::string> const & tokens, std::string const & delimiter);
TBAG_API std::string mergeTokens(std::vector<std::string> const & tokens);
TBAG_API std::string mergeArgv(char ** argv);

/**
 * Thread ID to string.
 */
TBAG_API std::string convertStringWithThreadId(std::thread::id const & id);

TBAG_CONSTEXPR char const * const STRING_EMPTY = TBAG_EMPTY_STRING;
TBAG_CONSTEXPR char const * const STRING_SPACE = TBAG_SPACE_STRING;
TBAG_CONSTEXPR char const * const STRING_HEX_PREFIX = "0x";
TBAG_CONSTEXPR int const DEFAULT_LINE_WIDTH = 2 * 8;

/**
 * Byte array to HEX string.
 */
TBAG_API char convertHalfByteToHexChar(uint8_t half_byte);
TBAG_API std::string convertByteToHexString(uint8_t hex);
TBAG_API std::string convertByteArrayToHexString(
        uint8_t const * bytes,
        std::size_t size,
        std::string const & prefix = STRING_HEX_PREFIX,
        std::string const & separator = STRING_EMPTY);
TBAG_API std::string convertByteArrayToHexStringBox(
        uint8_t const * bytes,
        std::size_t size,
        int line_width = DEFAULT_LINE_WIDTH,
        std::string const & prefix = STRING_HEX_PREFIX,
        std::string const & separator = STRING_SPACE,
        std::string const & new_line = NEW_LINE);

template <typename T>
std::string convertByteVectorToHexString(
        std::vector<T> const & bytes,
        std::string const & prefix = STRING_HEX_PREFIX,
        std::string const & separator = STRING_EMPTY)
{
    return convertByteArrayToHexString((uint8_t const *)bytes.data(), bytes.size(), prefix, separator);
}

template <typename T>
std::string convertByteVectorToHexStringBox(
        std::vector<T> const & bytes,
        int line_width = DEFAULT_LINE_WIDTH,
        std::string const & prefix = STRING_HEX_PREFIX,
        std::string const & separator = STRING_SPACE,
        std::string const & new_line = NEW_LINE)
{
    return convertByteArrayToHexStringBox((uint8_t const *)bytes.data(),
                                          bytes.size(), line_width, prefix, separator, new_line);
}

TBAG_CONSTEXPR std::size_t const HEX_STRING_ADDRESS_BYTE_SIZE =
        (/*PREFIX(0x)*/2) + (sizeof(std::uint64_t) * 2/*HEX STRING ADDRESS*/) + (1/*NULL*/);
using AddressHexString = std::array<char, HEX_STRING_ADDRESS_BYTE_SIZE>;

/**
 * HEX string to Byte array.
 */
TBAG_API Err convertHexCharToHalfByte(char hex_char, uint8_t & result);
TBAG_API Err convertHexCharToByte(char high_char, char low_char, uint8_t & result);
TBAG_API Err convertHexStringToBuffer(char const * hex_string, std::size_t length, libtbag::util::Buffer & buffer);

/**
 * Address to HEX string.
 */
TBAG_API AddressHexString convertAddressToHexString(void const * address) TBAG_NOEXCEPT;
TBAG_API AddressHexString convertUnsignedIntegerToHexString(std::uint64_t value) TBAG_NOEXCEPT;
TBAG_API std::string convertAddressHexStringToString(AddressHexString const & address);
TBAG_API std::string convertAddressToString(void const * address);

TBAG_API std::string convertByteArrayToPrettyHexStringLine(
        uint8_t const * bytes,
        std::size_t size,
        int line_width,
        bool print_address = true,
        bool print_ascii = true);
TBAG_API std::string convertByteArrayToPrettyHexStringBox(
        uint8_t const * bytes,
        std::size_t size,
        int line_width = DEFAULT_LINE_WIDTH,
        bool print_address = true,
        bool print_ascii = true);

template <typename T>
std::string convertByteVectorToPrettyHexStringBox(
        std::vector<T> const & bytes,
        int line_width = DEFAULT_LINE_WIDTH,
        bool print_address = true,
        bool print_ascii = true)
{
    return convertByteArrayToPrettyHexStringBox((uint8_t const *)bytes.data(), bytes.size(),
                                                line_width, print_address, print_ascii);
}

template <typename T>
std::string toPrettyHexBox(T const * bytes, std::size_t size)
{
    return convertByteArrayToPrettyHexStringBox((uint8_t const *)bytes, size);
}

template <typename T>
std::string toPrettyHexBox(std::vector<T> const & bytes)
{
    return convertByteVectorToPrettyHexStringBox(bytes);
}

/**
 * Regex based token.
 */
TBAG_API std::vector<std::string> splitMatch(std::string const & source, std::regex const & match);
TBAG_API std::vector<std::string> splitMatch(std::string const & source, std::string const & match);

// Regexp utilities.
TBAG_API std::string replaceRegex(std::string const & source, std::string const & regex, std::string const & replace);
TBAG_API std::string removeRegex(std::string const & source, std::string const & regex);

/**
 * Simple replace.
 */
TBAG_API std::string replace(std::string const & source, std::string const & find, std::string const & replace);
TBAG_API bool isMatch(std::string const & source, std::regex const & regex);
TBAG_API bool isMatch(std::string const & source, std::string const & regex);
TBAG_API bool isUtf8Match(std::string const & utf8_source, std::string const & regex);

/**
 * Checks whether str is a decimal digit character.
 *
 * @remarks
 *  Decimal digits are any of: 0 1 2 3 4 5 6 7 8 9
 */
TBAG_API bool isDigit(std::string const & str);
TBAG_API bool isVariableName(char c) TBAG_NOEXCEPT;
TBAG_API bool isVariableName(std::string const & str);
TBAG_API bool isWhiteSpace(char c) TBAG_NOEXCEPT;

TBAG_API std::string trimLeft(std::string const & str, std::string const & trim_list);
TBAG_API std::string trimRight(std::string const & str, std::string const & trim_list);
TBAG_API std::string trim(std::string const & str, std::string const & trim_list);

TBAG_API std::string trimLeft(std::string const & str, char trim_value);
TBAG_API std::string trimRight(std::string const & str, char trim_value);
TBAG_API std::string trim(std::string const & str, char trim_value);

TBAG_API std::string trimLeft(std::string const & str);
TBAG_API std::string trimRight(std::string const & str);
TBAG_API std::string trim(std::string const & str);
TBAG_API std::vector<std::string> trim(std::vector<std::string> const & strs, bool remove_empty = false);

TBAG_API void createRandomString(char * buffer, std::size_t size, char const * table, std::size_t min, std::size_t max);
TBAG_API void createRandomString(char * buffer, std::size_t size);
TBAG_API std::string createRandomString(std::size_t size);

TBAG_API std::string lower(std::string const & str);
TBAG_API std::string upper(std::string const & str);

TBAG_API void lowerSelf(std::string & str);
TBAG_API void upperSelf(std::string & str);

TBAG_CONSTEXPR char const * const TRUE_LOWER_STR1 =   "on";
TBAG_CONSTEXPR char const * const TRUE_LOWER_STR2 = "true";
TBAG_CONSTEXPR char const * const TRUE_LOWER_STR3 =   "ok";
TBAG_CONSTEXPR char const * const TRUE_LOWER_STR4 =  "yes";
TBAG_CONSTEXPR char const * const TRUE_LOWER_STR5 =    "y";

TBAG_CONSTEXPR char const * const FALSE_LOWER_STR1 =   "off";
TBAG_CONSTEXPR char const * const FALSE_LOWER_STR2 = "false";
TBAG_CONSTEXPR char const * const FALSE_LOWER_STR3 =    "no";
TBAG_CONSTEXPR char const * const FALSE_LOWER_STR4 =     "n";

TBAG_CONSTEXPR char const * const DEFAULT_TRUE_LOWER_STR = TRUE_LOWER_STR2;
TBAG_CONSTEXPR char const * const DEFAULT_FALSE_LOWER_STR = FALSE_LOWER_STR2;

TBAG_CONSTEXPR char const * const DEFAULT_TRUE_STR = DEFAULT_TRUE_LOWER_STR;
TBAG_CONSTEXPR char const * const DEFAULT_FALSE_STR = DEFAULT_FALSE_LOWER_STR;

TBAG_API bool toVal(std::string const & str,               bool & to, std::size_t * index = nullptr, int base = 10);
TBAG_API bool toVal(std::string const & str,               char & to, std::size_t * index = nullptr, int base = 10);
TBAG_API bool toVal(std::string const & str, unsigned      char & to, std::size_t * index = nullptr, int base = 10);
TBAG_API bool toVal(std::string const & str,              short & to, std::size_t * index = nullptr, int base = 10);
TBAG_API bool toVal(std::string const & str, unsigned     short & to, std::size_t * index = nullptr, int base = 10);
TBAG_API bool toVal(std::string const & str,                int & to, std::size_t * index = nullptr, int base = 10);
TBAG_API bool toVal(std::string const & str, unsigned       int & to, std::size_t * index = nullptr, int base = 10);
TBAG_API bool toVal(std::string const & str,               long & to, std::size_t * index = nullptr, int base = 10);
TBAG_API bool toVal(std::string const & str, unsigned      long & to, std::size_t * index = nullptr, int base = 10);
TBAG_API bool toVal(std::string const & str,          long long & to, std::size_t * index = nullptr, int base = 10);
TBAG_API bool toVal(std::string const & str, unsigned long long & to, std::size_t * index = nullptr, int base = 10);
TBAG_API bool toVal(std::string const & str,              float & to, std::size_t * index = nullptr, int base = 10);
TBAG_API bool toVal(std::string const & str,             double & to, std::size_t * index = nullptr, int base = 10);
TBAG_API bool toVal(std::string const & str,        long double & to, std::size_t * index = nullptr, int base = 10);
TBAG_API bool toVal(std::string const & str,        std::string & to, std::size_t * index = nullptr, int base = 10);

template <typename T>
inline T toValue(std::string const & str, T default_value = T(), int base = 10)
{
    T result;
    if (toVal(str, result, nullptr, base)) {
        return result;
    }
    return default_value;
}

TBAG_API std::string trimRightOfRealNumberText(std::string const & real_number_text);

template <typename T, bool is_floating>
struct __internal_to_string;

template <typename T>
struct __internal_to_string<T, true>
{
    static std::string run(T val)
    {
        return trimRightOfRealNumberText(std::to_string(val));
    }
};

template <typename T>
struct __internal_to_string<T, false>
{
    static std::string run(T val)
    {
        return std::to_string(val);
    }
};

template <typename T>
inline std::string toString(T val)
{
    return __internal_to_string<T, std::is_floating_point<T>::value>::run(val);
}

inline std::string toString(bool val)
{
    return val ? DEFAULT_TRUE_STR : DEFAULT_FALSE_STR;
}

inline std::string toString(char const * val)
{
    return std::string(val);
}

inline std::string toString(char * val)
{
    return std::string(val);
}

inline std::string toString(std::string && val)
{
    return std::move(val);
}

inline std::string toString(std::string const & val)
{
    return val;
}

TBAG_API std::string toHexString(unsigned char val, bool upper = true);
TBAG_API std::string toHexString(unsigned short val, bool upper = true);
TBAG_API std::string toHexString(unsigned int val, bool upper = true);
TBAG_API std::string toHexString(unsigned long val, bool upper = true);
TBAG_API std::string toHexString(unsigned long long val, bool upper = true);

} // namespace string

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_STRINGUTILS_HPP__

