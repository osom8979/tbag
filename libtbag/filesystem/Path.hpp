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
template <typename CharType>
using BaseNativePath = WindowsPath<CharType>;
#else
template <typename CharType>
using BaseNativePath = PosixPath<CharType>;
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
class TBAG_EXPORTS Path : public BaseNativePath<char>
{
public:
    using NativePath = BaseNativePath<char>;
    using ValueType  = typename NativePath::ValueType;
    using String     = typename NativePath::String;

    static_assert(std::is_pod<ValueType>::value
            , "Character type of PosixPath must be a POD");
    static_assert(std::is_same<ValueType, typename String::value_type>::value
            , "String::value_type must be the same type as ValueType");

public:
    /** Update Generic Format. */
    struct update_generic { /* EMPTY */ };

    /** Update Canonical Format. */
    struct update_canonical { /* EMPTY */ };

public:
    static constexpr bool isWindowsStyle() TBAG_NOEXCEPT
    {
#if defined(__OS_WINDOWS__)
        return true;
#else
        return false;
#endif
    }

    static constexpr bool isPosixStyle() TBAG_NOEXCEPT
    { return !isWindowsStyle(); }

private:
    String _path;

// Constructors.
public:
    Path() TBAG_NOEXCEPT_EXPR(std::is_nothrow_default_constructible<String>::value);
    explicit Path(String const & path);
    explicit Path(ValueType const * path);

    explicit Path(ValueType const * path, update_generic const & UNUSED_PARAM(v));
    explicit Path(String const & path, update_generic const & UNUSED_PARAM(v));

    explicit Path(ValueType const * path, update_canonical const & UNUSED_PARAM(v));
    explicit Path(String const & path, update_canonical const & UNUSED_PARAM(v));

    Path(std::vector<String> const & nodes);
    Path(std::initializer_list<String> list);

    Path(Path const & obj);
    Path(Path && obj);

// Destructor.
public:
    ~Path();

// Assign operators.
public:
    Path & operator =(ValueType const * path);
    Path & operator =(String const & path);
    Path & operator =(String && path);

    Path & operator =(Path const & obj);
    Path & operator =(Path && obj);

public:
    bool operator ==(Path const & path);
    bool operator ==(String const & path);

public:
    Path & copy(Path const & obj);
    void swap(Path && obj);
    void swap(Path & obj);

// Accessors & Mutators.
public:
    inline String getString() const TBAG_NOEXCEPT(TBAG_NOEXCEPT(String(_path)))
    { return _path; }
    inline void setString(String const & path)
    { _path.assign(path); }
    inline void setString(ValueType const * path)
    { _path.assign(path); }

// Path string.
public:
    /**
     * Generic path format.
     */
    String getGenericString() const;
    Path getGeneric() const;

    /**
     * Operating system dependent path.
     */
    String getNativeString() const;
    Path getNative() const;

    /**
     * Canonical path.
     */
    Path getCanonical() const;
    String getCanonicalString() const;

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
    String getRootDirString() const;
    Path getRootDir() const;

// Query.
public:
    bool isRootDir() const;
    bool isAbsolute() const;
    bool isRelative() const;

// Append.
public:
    static String append(String const & parent, String const & child);

    Path & append(String const & child);
    Path & append(std::vector<String> const & nodes);

    Path & operator /=(String const & child);

    TBAG_EXPORTS friend Path operator /(Path const & path, String const & child);
    TBAG_EXPORTS friend Path operator /(Path && path, String const & child);

// Casting
public:
    operator String() const;
    operator ValueType const *() const;

// Parent.
public:
    String getParentString() const;
    Path getParent() const;

// Node operators.
public:
    std::vector<String> splitNodes() const;
    std::vector<String> splitNodesWithCanonical() const;

public:
    String getName() const;

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

