/**
 * @file   Path.hpp
 * @brief  Path class prototype.
 * @author zer0
 * @date   2016-04-07
 *
 * @see <http://www.boost.org/doc/libs/1_60_0/libs/filesystem/doc/reference.html>
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_PATH_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_PATH_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/filesystem/details/FsTypes.hpp>

#include <string>
#include <vector>
#include <initializer_list>
#include <type_traits>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {

TBAG_API std::string getLibraryPrefix();
TBAG_API std::string getLibrarySuffix();
TBAG_API std::string getLibraryName(std::string const & name);

TBAG_API std::string getExecutablePrefix();
TBAG_API std::string getExecutableSuffix();
TBAG_API std::string getExecutableName(std::string const & name);

/**
 * Path class prototype.
 *
 * @author zer0
 * @date   2016-04-07
 * @date   2016-10-14 (Change: class template -> class)
 *
 * @warning
 *  Supports UTF-8 (multibyte-string) only.
 */
class TBAG_API Path
{
public:
    /** Update Generic Format. */
    struct update_generic { /* EMPTY */ };

    /** Update Canonical Format. */
    struct update_canonical { /* EMPTY */ };

private:
    std::string _path; ///< UTF-8 string.

// Constructors.
public:
    Path() TBAG_NOEXCEPT_SPECIFIER(std::is_nothrow_default_constructible<std::string>::value);

    explicit Path(std::string const & path);
    explicit Path(char const * path);

    explicit Path(char const * path, update_generic const & UNUSED_PARAM(v));
    explicit Path(std::string const & path, update_generic const & UNUSED_PARAM(v));

    explicit Path(char const * path, update_canonical const & UNUSED_PARAM(v));
    explicit Path(std::string const & path, update_canonical const & UNUSED_PARAM(v));

    Path(std::vector<std::string> const & nodes);
    Path(std::initializer_list<std::string> list);

    Path(Path const & obj);
    Path(Path && obj);

// Destructor.
public:
    ~Path();

// Assign operators.
public:
    Path & operator =(char const * path);
    Path & operator =(std::string const & path);
    Path & operator =(std::string && path);

    Path & operator =(Path const & obj);
    Path & operator =(Path && obj);

public:
    bool operator ==(Path const & path);
    bool operator ==(std::string const & path);

public:
    // @formatter:off
    inline void clear() TBAG_NOEXCEPT_SP_OP(_path.clear())
    { _path.clear(); }
    inline bool empty() const TBAG_NOEXCEPT_SP_OP(_path.empty())
    { return _path.empty(); }
    // @formatter:on

public:
    Path & copy(Path const & obj);
    void swap(Path && obj);
    void swap(Path & obj);

// Accessors
public:
    inline std::string getPathString() const { return _path; }
    inline std::string getString    () const { return _path; }
    inline std::string toString     () const { return _path; }

// Mutators.
public:
    inline void setString(std::string const & path)
    { _path.assign(path); }
    inline void setString(char const * path)
    { _path.assign(path); }

// Path string.
public:
    /**
     * Generic path format.
     */
    std::string getGenericString() const;
    Path getGeneric() const;

    /**
     * Operating system dependent path.
     */
    std::string getNativeString() const;
    Path getNative() const;

    /**
     * Canonical path.
     */
    Path getCanonical() const;
    std::string getCanonicalString() const;

// Modifiers.
public:
    Path & updateGeneric();
    Path & updateNative();
    Path & updateCanonical();

// Decomposition.
public:
    /**
     * root-directory, if @c _path includes root-directory,
     * otherwise empty string.
     */
    std::string getRootDirString() const;
    Path getRootDir() const;

// Query.
public:
    bool isRootDir() const;
    bool isAbsolute() const;
    bool isRelative() const;

// Append.
public:
    static std::string append(std::string const & parent, std::string const & child);

    Path & append(std::string const & child);
    Path & append(std::vector<std::string> const & nodes);

    Path & operator /=(std::string const & child);

public:
    TBAG_API friend Path operator /(Path const & path, std::string const & child);
    TBAG_API friend Path operator /(Path && path, std::string const & child);

// Casting
public:
    inline operator std::string  () const { return _path; }
    inline operator char const * () const { return _path.c_str(); }

// Parent.
public:
    std::string getParentString() const;
    Path getParent() const;

// Node operators.
public:
    std::vector<std::string> splitNodes() const;
    std::vector<std::string> splitNodesWithCanonical() const;

public:
    /** Filename except path. */
    std::string getName() const;

    /** File name without directory or longest extension. */
    std::string getNameWithoutExtension() const;

    /** File extension name except path. */
    std::string getExtensionName() const;

public:
    using TimeSpec  = details::TimeSpec;
    using FileState = details::FileState;

public:
    FileState getState() const;

// Filesystem operators.
public:
    bool exists() const;

public:
    bool isExecutable() const;
    bool isWritable() const;
    bool isReadable() const;

public:
    bool isRegularFile() const;
    bool isDirectory() const;

public:
    /**
     * Create directory.
     *
     * @remarks
     *  If the parent node does not exist, it is created together.
     */
    bool createDir(int mode = 0755) const;

    /** Remove directory or file. */
    bool remove() const;

    /** Remove all child nodes. */
    bool removeAll() const;

public:
    using DirentType = details::DirentType;

public:
    TBAG_CONSTEXPR static DirentType const DIRENT_UNKNOWN = details::DIRENT_UNKNOWN;
    TBAG_CONSTEXPR static DirentType const DIRENT_FILE    = details::DIRENT_FILE;
    TBAG_CONSTEXPR static DirentType const DIRENT_DIR     = details::DIRENT_DIR;
    TBAG_CONSTEXPR static DirentType const DIRENT_LINK    = details::DIRENT_LINK;
    TBAG_CONSTEXPR static DirentType const DIRENT_FIFO    = details::DIRENT_FIFO;
    TBAG_CONSTEXPR static DirentType const DIRENT_SOCKET  = details::DIRENT_SOCKET;
    TBAG_CONSTEXPR static DirentType const DIRENT_CHAR    = details::DIRENT_CHAR;
    TBAG_CONSTEXPR static DirentType const DIRENT_BLOCK   = details::DIRENT_BLOCK;
    TBAG_CONSTEXPR static DirentType const DIRENT_ALL     = details::DIRENT_ALL;

public:
    /** Scan for child nodes. */
    std::vector<Path> scanDir(DirentType type = DIRENT_ALL) const;

public:
    static Path getWorkDir();
    static Path getHomeDir();
    static Path getExePath();
    static Path getExeDir();
};

// ----------
// Utilities.
// ----------

TBAG_API Err checkPrepareFiles(std::string const & source_path, std::string const & destination_path, bool check_dir = true);
TBAG_API Err checkPrepareFiles(Path const & source_path, Path const & destination_path, bool check_dir = true);

} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#include <ostream>

template <class CharT, class TraitsT>
std::basic_ostream<CharT, TraitsT> & operator<<(std::basic_ostream<CharT, TraitsT> & os,
                                                libtbag::filesystem::Path const & path)
{
    os << path.toString();
    return os;
}

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_PATH_HPP__

