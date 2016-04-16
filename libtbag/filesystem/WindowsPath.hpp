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
 *
 * @warning
 *  Supports multibyte-string only.
 */
class WindowsPath
{
public:
    static constexpr char const PATH_SEPARATOR = PATH_SEPARATOR_OF_WINDOWS;
    static constexpr char const * const PATH_SEPARATOR_STRING = PATH_SEPARATOR_STRING_OF_WINDOWS;
    static constexpr char const * const REMOVE_SEPARATOR_REGEX = R"([\\\/][\\\/]*)";

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
    static bool isProhibitedFilename(std::string const & path) noexcept {
        for (char const & cursor : path) {
            if (0x00 <= COMPARE_AND(cursor) <= 0x1F) {
                return true;
            }

            switch (cursor) {
                case '*': case '<': case '>':
                case '?': case '/': case '|': case '\\':
                    return true;
            }
        }
        return false;
    }

// Remove last separator.
public:
    static std::string removeLastSeparator(std::string const & path) {
        return Strings::removeRegex(path, std::string(REMOVE_SEPARATOR_REGEX) + "$");
    }

// Make preferred.
public:
    /**
     * Slashes converted to backslashes.
     */
    inline static std::string makePreferred(std::string const & path) {
        return Strings::replaceRegex(path
                                   , PATH_SEPARATOR_STRING_OF_POSIX
                                   , PATH_SEPARATOR_STRING);
    }

    static std::string removeDuplicateSeparators(std::string const & path) {
        return Strings::replaceRegex(path
                                   , REMOVE_SEPARATOR_REGEX
                                   , PATH_SEPARATOR_STRING);
    }

// Path string.
public:
    /**
     * makePreferred -> removeDuplicateSeparators -> removeLastSeparator
     */
    static std::string getNative(std::string const & path) {
        return removeLastSeparator(removeDuplicateSeparators(makePreferred(path)));
    }

    /**
     * getNative -> generic formatting.
     */
    static std::string getGeneric(std::string const & path) {
        return Strings::replaceRegex(getNative(path)
                                   , REMOVE_SEPARATOR_REGEX
                                   , GetGenericPathSeparatorString());
    }

// Decomposition.
public:
    static std::string getRootDir(std::string const & path) {
        if (path.size() < 2 || path[1] != ':') {
            return std::string();
        }
        if (/**/('a' <= COMPARE_AND(path[0]) <= 'z')
             || ('A' <= COMPARE_AND(path[0]) <= 'Z')) {
            return path.substr(0, 2);
        }
        return std::string();
    }

// Query.
public:
    static bool isAbsolute(std::string const & path) {
        return !getRootDir(path).empty();
    }

    static bool isRelative(std::string const & path) {
        return !isAbsolute(path);
    }

// Parent.
public:
    static std::string getParent(std::string const & path) {
        std::string temp = removeLastSeparator(path);
        for (auto ritr = temp.rbegin(); ritr != temp.rend();  ++ritr) {
            if (*ritr == PATH_SEPARATOR_OF_WINDOWS || *ritr == PATH_SEPARATOR_OF_POSIX) {
                return temp.substr(0, temp.size() - std::distance(temp.rbegin(), ritr));
            }
        }
        return std::string();
    }

// Node operators.
public:
    static std::vector<std::string> splitNodes(std::string const & path) {
        return Strings::splitTokens(getGeneric(path),
                                    GetGenericPathSeparatorString());
    }
};

} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_WINDOWSPATH_HPP__

