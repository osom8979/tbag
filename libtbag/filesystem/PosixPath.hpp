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
#include <libtbag/predef.hpp>

#include <string>
#include <vector>

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
class TBAG_EXPORTS PosixPath
{
public:
    /**
     * Characters prohibited in filename: 0x00, '/'
     */
    struct ProhibitedBy
    {
        inline bool operator()(char v) const TBAG_NOEXCEPT
        { return v == '\0' || v == '/'; }
    };

public:
    PosixPath() TBAG_NOEXCEPT;
    ~PosixPath();

public:
    static std::string getPathSeparator();
    static std::string getGenericPathSeparatorString();
    static std::string getRemoveSeparatorRegex();

// Filename query.
public:
    static bool isProhibitedFilename(std::string const & path) TBAG_NOEXCEPT;

// Remove last separator.
public:
    static std::string removeLastSeparator(std::string const & path);

// Make preferred.
public:
    /**
     * No change.
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
     * Equals to getNative
     */
    static std::string getGeneric(std::string const & path);

// Decomposition.
public:
    /**
     * Root directory is only '/'.
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

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_POSIXPATH_HPP__

