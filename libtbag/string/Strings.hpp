/**
 * @file   Strings.hpp
 * @brief  Strings class prototype.
 * @author zer0
 * @date   2016-04-04
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_STRINGS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_STRINGS_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/type.hpp>

#define CHECK_GNUC_CXX_REGEX
#include <libtbag/predef.hpp>

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

template <typename T>
constexpr T const * charOrWidechar(char const * c, wchar_t const * w);

template <>
constexpr char const * charOrWidechar<char>(char const * c, wchar_t const * w)
{
    return c;
}

template <>
constexpr wchar_t const * charOrWidechar<wchar_t>(char const * c, wchar_t const * w)
{
    return w;
}

#ifndef CHAR_OR_WIDECHAR
#define CHAR_OR_WIDECHAR(type, str) ::libtbag::string::charOrWidechar<type>(str, L##str)
#endif

/**
 * Separate tokens.
 *
 * @tparam CharType    Character type.
 * @tparam StringType  String type (Don't change this typename).
 *
 * @param source    [in] Original string.
 * @param delimiter [in] Delimiter string.
 *
 * @return
 *  Token vector.
 */
template <typename CharType   = char
        , typename StringType = std::basic_string<typename libtbag::remove_cr<CharType>::type> >
std::vector<StringType> splitTokens(StringType const & source
                                  , StringType const & delimiter)
{
    if (source.empty() || delimiter.empty()) {
        return std::vector<StringType>();
    }

    std::vector<StringType> result;
    StringType token;

    std::size_t start = 0;
    std::size_t end   = source.find(delimiter);


    while (end != StringType::npos) {
        token = source.substr(start, end - start);
        if (token.empty() == false) {
            result.push_back(token);
        }

        // Calculate next token index.
        start = end + delimiter.length();
        end   = source.find(delimiter, start);
    }

    // Last token.
    token = source.substr(start, end);
    if (token.empty() == false) {
        result.push_back(token);
    }

    return result;
}

template <typename FloatingType, typename CharType = char>
std::basic_string<CharType> convertStringWithFloatingPoint(FloatingType floating, int precision = 2)
{
    static_assert(std::is_floating_point<FloatingType>::value, "Not floating point type.");

    std::basic_stringstream<CharType> ss;
    ss.setf(std::ios_base::showpoint);
    ss.setf(std::ios_base::fixed, std::ios_base::floatfield);
    ss.precision(precision);
    ss << floating;
    return ss.str();
}

/**
 * Thread ID to string.
 */
template <typename CharType = char>
std::basic_string<CharType> convertStringWithThreadId(std::thread::id const & id)
{
    std::basic_stringstream<CharType> ss;
    ss << id;
    return ss.str();
}

} // namespace string

/**
 * String utility class.
 *
 * @author zer0
 * @date   2016-04-04
 *
 * @remarks
 *  Without using the inline function, class for using static method.
 *
 * @translate{ko, inline function을 사용하지 않고\, static method를 사용하기 위한 클래스.}
 */
template <typename CharType = char>
class BaseStrings : public Noncopyable
{
public:
    using ValueType = CharType;
    using String    = std::basic_string<ValueType>;

    using Regex = std::basic_regex<ValueType>;
    using RegexTokenIterator = std::regex_token_iterator<typename String::const_iterator>;

    static_assert(std::is_pod<ValueType>::value
            , "Character type of BaseStrings must be a POD");
    static_assert(std::is_same<ValueType, typename String::value_type>::value
            , "String::value_type must be the same type as ValueType");

public:
    constexpr BaseStrings() noexcept = default;
    ~BaseStrings() noexcept = default;

public:
    /**
     * Regex based token.
     */
    static std::vector<String> splitMatch(String const & source
                                        , Regex  const & match) {
        auto itr = RegexTokenIterator(source.begin(), source.end(), match);
        auto end = RegexTokenIterator();

        std::vector<String> result;
        while (itr != end) {
            result.push_back(itr->str());
            ++itr;
        }
        return result;
    }

    static std::vector<String> splitMatch(String const & source
                                        , String const & match) {
        return splitMatch(source, Regex(match));
    }

// Regexp utilities.
public:
    static String replaceRegex(String const & path
                             , String const & regex
                             , String const & replace) {
        return std::regex_replace(path, Regex(regex), replace);
    }

    static String removeRegex(String const & path
                            , String const & regex) {
        return replaceRegex(path, regex, String());
    }

    static bool isMatch(String const & original
                      , Regex  const & regex) {
        return std::regex_match(original, regex);
    }

    static bool isMatch(String const & original
                      , String const & regex) {
        return isMatch(original, Regex(regex));
    }

public:
    static constexpr ValueType const CHAR_SPACE  = ' ';
    static constexpr ValueType const CHAR_TAB    = '\t';
    static constexpr ValueType const CHAR_RETURN = '\n';

    static String trimLeft(String const & str) {
        auto itr = str.begin();
        for (; itr != str.end(); ++itr) {
            if (*itr != CHAR_SPACE && *itr != CHAR_TAB && *itr != CHAR_RETURN) {
                break;
            }
        }
        return String(itr, str.end());
    }

    static String trimRight(String const & str) {
        auto itr = str.rbegin();
        for (; itr != str.rend(); ++itr) {
            if (*itr != CHAR_SPACE && *itr != CHAR_TAB && *itr != CHAR_RETURN) {
                break;
            }
        }
        return String(str.begin(), itr.base());
    }

    static String trim(String const & str) {
        return trimRight(trimLeft(str));
    }
};

using Strings = BaseStrings<char>;
using WideStrings = BaseStrings<wchar_t>;

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_STRINGS_HPP__

