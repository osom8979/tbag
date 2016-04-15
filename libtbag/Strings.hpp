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
class Strings : public Noncopyable
{
public:
    static constexpr char const * const EMPTY_STRING = "";

public:
    constexpr Strings() = default;
    ~Strings() = default;

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
    static std::vector<std::string> splitTokens(std::string const & source
                                              , std::string const & delimiter) {
        std::vector<std::string> result;
        std::string token;

        std::size_t start = 0;
        std::size_t end = source.find(delimiter);

        while (end != std::string::npos) {
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

// Regexp utilities.
public:
    static std::string replaceRegex(std::string const & path
                                  , std::string const & regex
                                  , std::string const & replace) {
        return std::regex_replace(path, std::regex(regex), replace);
    }

    static std::string removeRegex(std::string const & path
                                 , std::string const & regex) {
        return Strings::replaceRegex(path, regex, Strings::EMPTY_STRING);
    }
};

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_STRINGS_HPP__

