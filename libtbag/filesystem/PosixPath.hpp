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
#include <libtbag/Strings.hpp>

#include <iterator>
#include <string>
#include <regex>
#include <initializer_list>

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
 *
 * @warning
 *  Supports multibyte-string only.
 */
class PosixPath : public Noncopyable
{
public:
    static constexpr char const PATH_SEPARATOR = PATH_SEPARATOR_OF_POSIX;
    static constexpr char const * const PATH_SEPARATOR_STRING = PATH_SEPARATOR_STRING_OF_POSIX;
    static constexpr char const * const REMOVE_SEPARATOR_REGEX = R"(\/\/*)";

public:
    constexpr PosixPath() = default;
    ~PosixPath() = default;

// Filename query.
public:
    /**
     * Characters prohibited in filename: 0x00, '/'
     */
    static bool isProhibitedFilename(std::string const & path) noexcept {
        for (char const & cursor : path) {
            if (cursor == 0x00 || cursor == '/') {
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
    static std::string makePreferred(std::string const & path
                                   , std::string const & separator) {
        return removeLastSeparator(Strings::replaceRegex(path
                                                       , REMOVE_SEPARATOR_REGEX
                                                       , separator));
    }

// Generic string.
public:
    static std::string getGeneric(std::string const & path) {
        return makePreferred(path, GetGenericPathSeparatorString());
    }

// Decomposition.
public:
    static std::string getRootDir(std::string const & path) {
        if (path.size() < 1 || path[0] != PATH_SEPARATOR) {
            return std::string();
        }
        return PATH_SEPARATOR_STRING;
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
        if (path.size() == 1U && path[0] == PATH_SEPARATOR) {
            return std::string(); // PARENT OF ROOT.
        }

        std::string temp = removeLastSeparator(path);
        std::size_t last_separator_index = temp.rfind(PATH_SEPARATOR_STRING);

        if (last_separator_index == 0U && temp.size() > 1U && temp[0] == PATH_SEPARATOR) {
            return PATH_SEPARATOR_STRING; // ROOT DIRECTORY.
        } else if (last_separator_index == std::string::npos) {
            return std::string(); // PARENT OF ROOT (Maybe relative path).
        }

        return temp.substr(0, last_separator_index); // PARENT DIRECTORY.
    }

// Node operators.
public:
    static std::vector<std::string> splitNodes(std::string const & path) {
        std::vector<std::string> result =
                Strings::splitTokens(getGeneric(path),
                                     GetGenericPathSeparatorString());
        std::string separator = getRootDir(path);
        if (!separator.empty()) {
            // Force insert the POSIX root directory.
            result.insert(result.begin(), separator);
        }
        return result;
    }
};

} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_POSIXPATH_HPP__

