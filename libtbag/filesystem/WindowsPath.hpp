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
#include <libtbag/predef.hpp>

#include <string>
#include <vector>

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
class TBAG_EXPORTS WindowsPath
{
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
        inline bool operator()(char v) const TBAG_NOEXCEPT
        {
            if (0x00 <= COMPARE_AND(v) <= 0x1F) {
                return true;
            }
            switch (v) {
            case '*': case '<': case '>':
            case '?': case '/': case '|': case '\\':
                return true;
            }
            return false;
        }
    };

public:
    WindowsPath() TBAG_NOEXCEPT;
    ~WindowsPath();

public:
    static std::string getPathSeparator();
    static std::string getGenericPathSeparatorString();
    static std::string getRemoveSeparatorRegex();

// Filename query.
public:
    static bool isProhibitedFilename(std::string const & path);

// Remove last separator.
public:
    static std::string removeLastSeparator(std::string const & path);

// Make preferred.
public:
    static std::string getReplaceStringOfPosix();

    /**
     * Slashes converted to backslashes.
     */
    static std::string makePreferred(std::string const & path);
    static std::string removeDuplicateSeparators(std::string const & path);

// Path string.
public:
    /**
     * makePreferred -> removeDuplicateSeparators -> removeLastSeparator
     */
    static std::string getNative(std::string const & path);

    /**
     * getNative -> generic formatting.
     */
    static std::string getGeneric(std::string const & path);

// Decomposition.
public:
    /**
     * Root directory is like "C:".
     */
    static std::string getRootDir(std::string const & path);

// Query.
public:
    static bool isAbsolute(std::string const & path);
    static bool isRelative(std::string const & path);

// Parent.
public:
    static std::string getParent(std::string const & path);

// Node operators.
public:
    static std::vector<std::string> splitNodes(std::string const & path);
};

} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_WINDOWSPATH_HPP__

