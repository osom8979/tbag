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
#include <libtbag/filesystem/WindowsPath.hpp>
#include <libtbag/filesystem/PosixPath.hpp>

#include <string>
#include <initializer_list>
#include <type_traits>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {

#if defined(__OS_WINDOWS__)
using BaseNativePath = WindowsPath;
#else
using BaseNativePath = PosixPath;
#endif

/**
 * Path class prototype.
 *
 * @author zer0
 * @date   2016-04-07
 * @date   2016-10-14 (Change: class template -> class)
 *
 * @warning
 *  Supports multibyte-string only.
 */
class TBAG_API Path : public BaseNativePath
{
public:
    using NativePath = BaseNativePath;

public:
    /** Update Generic Format. */
    struct update_generic { /* EMPTY */ };

    /** Update Canonical Format. */
    struct update_canonical { /* EMPTY */ };

public:
    inline static TBAG_CONSTEXPR bool isWindowsStyle() TBAG_NOEXCEPT
    {
#if defined(__OS_WINDOWS__)
        return true;
#else
        return false;
#endif
    }

    inline static TBAG_CONSTEXPR bool isPosixStyle() TBAG_NOEXCEPT
    {
        return !isWindowsStyle();
    }

private:
    std::string _path;

// Constructors.
public:
    Path() TBAG_NOEXCEPT_EXPR(std::is_nothrow_default_constructible<std::string>::value);

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
    Path & copy(Path const & obj);
    void swap(Path && obj);
    void swap(Path & obj);

// Accessors & Mutators.
public:
    inline std::string getString() const
    { return _path; }
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

    TBAG_API friend Path operator /(Path const & path, std::string const & child);
    TBAG_API friend Path operator /(Path && path, std::string const & child);

// Casting
public:
    operator std::string() const;
    operator char const *() const;

// Parent.
public:
    std::string getParentString() const;
    Path getParent() const;

// Node operators.
public:
    std::vector<std::string> splitNodes() const;
    std::vector<std::string> splitNodesWithCanonical() const;

public:
    std::string getName() const;

// Filesystem operators.
public:
    bool exists() const;
    bool isExecutable() const;
    bool isWritable() const;
    bool isReadable() const;

    bool isRegularFile() const;
    bool isDirectory() const;

    bool createDir() const;
    bool createDirWithRecursive() const;
    bool remove() const;
    bool removeFile() const;
    bool removeDir() const;
    bool removeDirWithRecursive() const;

    std::vector<Path> scanDir() const;

// Special directories.
public:
    static Path getWorkDir();
    static Path getHomeDir();
    static Path getExePath();
    static Path getExeDir();
};

} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_PATH_HPP__

