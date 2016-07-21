/**
 * @file   PosixPath.hpp
 * @brief  PosixPath class prototype.
 * @author zer0
 * @date   2016-04-15
 *
 * @see <http://www.boost.org/doc/libs/1_60_0/libs/filesystem/doc/reference.html>
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_POSIXPATH_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_POSIXPATH_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/filesystem/Common.hpp>
#include <libtbag/string/Strings.hpp>

#include <cassert>
#include <string>
#include <algorithm>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {

/**
 * PosixPath class prototype.
 *
 * @author zer0
 * @date   2016-04-15
 *
 * @remarks
 *  ISO/IEC 9945 POSIX API.
 */
template <typename CharType = char>
class PosixPath
{
public:
    using ValueType = CharType;
    using String    = std::basic_string<ValueType>;

    static_assert(std::is_pod<ValueType>::value
            , "Character type of PosixPath must be a POD");
    static_assert(std::is_same<ValueType, typename String::value_type>::value
            , "String::value_type must be the same type as ValueType");

public:
    static constexpr ValueType const PATH_SEPARATOR
            = static_cast<ValueType const>(common::PATH_SEPARATOR_OF_POSIX);
    static constexpr ValueType const GENERIC_PATH_SEPARATOR
            = static_cast<ValueType const>(common::getGenericPathSeparator());

public:
    inline static String getPathSeparator() {
        return { PATH_SEPARATOR };
    }

    inline static String getGenericPathSeparatorString() {
        return { GENERIC_PATH_SEPARATOR };
    }

    inline static String getRemoveSeparatorRegex() {
        return CHAR_OR_WIDECHAR(ValueType, R"(\/\/*)");
    }

public:
    constexpr PosixPath() noexcept = default;
    ~PosixPath() noexcept = default;

// Filename query.
public:
    /**
     * Characters prohibited in filename: 0x00, '/'
     */
    struct ProhibitedBy
    {
        inline bool operator()(ValueType v) const noexcept {
            if (v == 0x00 || v == static_cast<ValueType>('/')) {
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
    /**
     * No change.
     */
    inline static String makePreferred(String const & path) {
        return path;
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
     * Equals to getNative
     */
    static String getGeneric(String const & path) {
        return getNative(path);
    }

// Decomposition.
public:
    /**
     * Root directory is only '/'.
     */
    static String getRootDir(String const & path) {
        if (path.size() < 1 || path[0] != PATH_SEPARATOR) {
            return String();
        }
        return getPathSeparator();
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
        if (path.size() == 1U && path[0] == PATH_SEPARATOR) {
            return String(); // PARENT OF ROOT.
        }

        String temp = removeLastSeparator(path);
        std::size_t last_separator_index = temp.rfind(getPathSeparator());

        if (last_separator_index == 0U && temp.size() > 1U && temp[0] == PATH_SEPARATOR) {
            return getPathSeparator(); // ROOT DIRECTORY.
        } else if (last_separator_index == String::npos) {
            return String(); // PARENT OF ROOT (Maybe relative path).
        }

        assert(last_separator_index != String::npos);
        return temp.substr(0, last_separator_index + 1); // PARENT DIRECTORY.
    }

// Node operators.
public:
    static std::vector<String> splitNodes(String const & path) {
        std::vector<String> result =
                Strings::splitTokens(getGeneric(path),
                                     getGenericPathSeparatorString());
        String root = getRootDir(path);
        if (!root.empty()) {
            // Force insert the POSIX root directory.
            result.insert(result.begin(), root);
        }
        return result;
    }
};

} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_POSIXPATH_HPP__

