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
#include <libtbag/util/Noncopyable.hpp>

#include <string>
#include <set>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

/**
 * String utility class.
 *
 * @author zer0
 * @date   2016-04-04
 */
template <typename T = char>
class Strings : public Noncopyable
{
public:
    using BaseType = T;
    using BaseString = std::basic_string<BaseType>;

public:
    constexpr Strings() = default;
    ~Strings() = default;

public:
    static std::set<BaseString> splitTokens(BaseString const & source, BaseString const & delimiter)
    {
        std::set<BaseString> result;
        std::string token;

        std::size_t start = 0;
        std::size_t end = source.find(delimiter);

        while (end != std::string::npos) {
            token = source.substr(start, end - start);
            if (token.size() > 0) {
                result.insert(token);
            }

            start = end + delimiter.length();
            end = source.find(delimiter, start);
        }

        // Last token.
        token = source.substr(start, end);
        if (token.size() > 0) {
            result.insert(token);
        }

        return result;
    }
};

using strings  = Strings<char>;
using wstrings = Strings<wchar_t>;

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_STRINGS_HPP__

