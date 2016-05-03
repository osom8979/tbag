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

#include <string>
#include <vector>
#include <regex>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

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

    static_assert(std::is_pod<ValueType>::value
            , "Character type of BaseStrings must be a POD");
    static_assert(std::is_same<ValueType, typename String::value_type>::value
            , "String::value_type must be the same type as ValueType");

public:
    constexpr BaseStrings() noexcept = default;
    ~BaseStrings() noexcept = default;

public:
    /**
     * Separate tokens.
     *
     * @param source    [in] Original string.
     * @param delimiter [in] Delimiter string.
     *
     * @return
     *  Token vector.
     */
    static std::vector<String> splitTokens(String const & source
                                         , String const & delimiter) {
        std::vector<String> result;
        String token;

        std::size_t start = 0;
        std::size_t end = source.find(delimiter);

        while (end != String::npos) {
            token = source.substr(start, end - start);
            if (token.size() > 0) {
                result.push_back(token);
            }

            // Calculate next token index.
            start = end + delimiter.length();
            end = source.find(delimiter, start);
        }

        // Last token.
        token = source.substr(start, end);
        if (token.size() > 0) {
            result.push_back(token);
        }

        return result;
    }

    /**
     * Regex based token.
     */
    static std::vector<String> splitMatch(String     const & source
                                        , std::regex const & match) {
        auto itr = std::sregex_token_iterator(source.begin(), source.end(), match);
        auto end = std::sregex_token_iterator();

        std::vector<std::string> result;
        while (itr != end) {
            result.push_back(itr->str());
            ++itr;
        }
        return result;
    }

// Regexp utilities.
public:
    static String replaceRegex(String const & path
                             , String const & regex
                             , String const & replace) {
        return std::regex_replace(path, std::regex(regex), replace);
    }

    static String removeRegex(String const & path
                            , String const & regex) {
        return replaceRegex(path, regex, String());
    }
};

using Strings = BaseStrings<char>;
using WideStrings = BaseStrings<wchar_t>;

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_STRINGS_HPP__

