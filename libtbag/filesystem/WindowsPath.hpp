/**
 * @file   WindowsPath.hpp
 * @brief  WindowsPath class prototype.
 * @author zer0
 * @date   2016-04-15
 *
 * @see <http://www.boost.org/doc/libs/1_60_0/libs/filesystem/doc/reference.html>
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_WINDOWSPATH_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_WINDOWSPATH_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/filesystem/Common.hpp>
#include <libtbag/Strings.hpp>

#include <cassert>
#include <string>
#include <iterator> // std::distance
#include <algorithm>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {

/**
 * WindowsPath class prototype.
 *
 * @author zer0
 * @date   2016-04-15
 *
 * @remarks
 *  Windows API.
 */
template <typename CharType = char>
class WindowsPath
{
public:
    using ValueType = CharType;
    using String    = std::basic_string<ValueType>;

    static_assert(std::is_pod<ValueType>::value
            , "Character type of WindowsPath must be a POD");
    static_assert(std::is_same<ValueType, typename String::value_type>::value
            , "String::value_type must be the same type as ValueType");

public:
    static constexpr ValueType const PATH_SEPARATOR
            = static_cast<ValueType const>(PATH_SEPARATOR_OF_WINDOWS);
    static constexpr ValueType const GENERIC_PATH_SEPARATOR
            = static_cast<ValueType const>(GetGenericPathSeparator());

public:
    inline static String getPathSeparator() {
        return { PATH_SEPARATOR };
    }

    inline static String getGenericPathSeparator() {
        return { GENERIC_PATH_SEPARATOR };
    }

    inline static String getRemoveSeparatorRegex() {
        return { '[', '\\', '\\', '\\', '/', ']', '[', '\\', '\\', '\\', '/', ']', '*' };
    }


public:
    constexpr WindowsPath() noexcept = default;
    ~WindowsPath() noexcept = default;

// Filename query.
public:
    /**
     * Characters prohibited in filename:
     *  0x00-0x1F, '"', '*', '*', '<', '>', '?', '\\' (single backslash), '/', '|'
     *
     * @remarks
     *  Many operating systems prohibit the ASCII control characters (0x00-0x1F) in filenames.
     */
    struct ProhibitedBy
    {
        inline bool operator()(ValueType v) const noexcept {
            if (0x00 <= COMPARE_AND(v) <= 0x1F) {
                return true;
            }
            switch (v) {
            case static_cast<ValueType>('*'):
            case static_cast<ValueType>('<'):
            case static_cast<ValueType>('>'):
            case static_cast<ValueType>('?'):
            case static_cast<ValueType>('/'):
            case static_cast<ValueType>('|'):
            case static_cast<ValueType>('\\'):
                return true;
            }
            return false;
        }
    };

    static bool isProhibitedFilename(String const & path) noexcept {
        return std::any_of(path.begin(), path.end(), ProhibitedBy());
    }

// Remove last separator.
public:
    static String removeLastSeparator(String const & path) {
        return Strings::removeRegex(path, getRemoveSeparatorRegex()
                                          + static_cast<ValueType>('$'));
    }

// Make preferred.
public:
    static String getReplaceStringOfPosix() {
        return {PATH_SEPARATOR_OF_POSIX};
    }

    /**
     * Slashes converted to backslashes.
     */
    inline static String makePreferred(String const & path) {
        return Strings::replaceRegex(path
                                   , getReplaceStringOfPosix()
                                   , getPathSeparator());
    }

    static String removeDuplicateSeparators(String const & path) {
        return Strings::replaceRegex(path
                                   , getRemoveSeparatorRegex()
                                   , getPathSeparator());
    }

// Path string.
public:
    /**
     * makePreferred -> removeDuplicateSeparators -> removeLastSeparator
     */
    static String getNative(String const & path) {
        return removeLastSeparator(removeDuplicateSeparators(makePreferred(path)));
    }

    /**
     * getNative -> generic formatting.
     */
    static String getGeneric(String const & path) {
        return Strings::replaceRegex(getNative(path)
                                   , getRemoveSeparatorRegex()
                                   , getGenericPathSeparator());
    }

// Decomposition.
public:
    /**
     * Root directory is like "C:".
     */
    static String getRootDir(String const & path) {
        if (path.size() < 2 || path[1] != static_cast<ValueType>(':')) {
            return String();
        }
        if (/**/(static_cast<ValueType>('a') <= COMPARE_AND(path[0]) <= static_cast<ValueType>('z'))
             || (static_cast<ValueType>('A') <= COMPARE_AND(path[0]) <= static_cast<ValueType>('Z'))) {
            return path.substr(0, 2);
        }
        return String();
    }

// Query.
public:
    static bool isAbsolute(String const & path) {
        return !getRootDir(path).empty();
    }

    static bool isRelative(String const & path) {
        return !isAbsolute(path);
    }

// Parent.
public:
    static String getParent(String const & path) {
        String temp = removeLastSeparator(path);
        for (auto itr = temp.rbegin(); itr != temp.rend(); ++itr) {
            if (/**/*itr == static_cast<ValueType>(PATH_SEPARATOR_OF_WINDOWS)
                 || *itr == static_cast<ValueType>(PATH_SEPARATOR_OF_POSIX)) {
                return temp.substr(0, temp.size() - std::distance(temp.rbegin(), itr));
            }
        }
        return String();
    }

// Node operators.
public:
    static std::vector<String> splitNodes(String const & path) {
        return Strings::splitTokens(getGeneric(path),
                                    getGenericPathSeparator());
    }
};

} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_WINDOWSPATH_HPP__

