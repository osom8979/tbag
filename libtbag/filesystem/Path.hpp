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
#include <libtbag/filesystem/Common.hpp>
#include <libtbag/filesystem/WindowsPath.hpp>
#include <libtbag/filesystem/PosixPath.hpp>
#include <libtbag/Strings.hpp>

#include <string>
#include <initializer_list>
#include <type_traits>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {

#if defined(__OS_WINDOWS__)
using NativePath = WindowsPath;
#else
using NativePath = PosixPath;
#endif

/**
 * Path class prototype.
 *
 * @author zer0
 * @date   2016-04-07
 *
 * @warning
 *  Supports multibyte-string only.
 */
class Path : public NativePath
{
public:
    /** Update Generic Format. */
    struct update_generic { __EMPTY_BLOCK__ };

public:
    static constexpr bool isWindowsStyle() noexcept {
#if defined(__OS_WINDOWS__)
        return true;
#else
        return false;
#endif
    }

    static constexpr bool isPosixStyle() noexcept {
        return !Path::isWindowsStyle();
    }

private:
    std::string _path;

// Constructors.
public:
    Path() noexcept(std::is_nothrow_default_constructible<std::string>::value) {
        __EMPTY_BLOCK__
    }

    explicit Path(std::string const & path) : Path() {
        this->_path.assign(path);
    }

    explicit Path(char const * path) : Path(std::string(path)) {
        __EMPTY_BLOCK__
    }

    explicit Path(char       const * path
                , update_generic const & UNUSED_PARAM(empty_value))
            : Path(std::string(path)){
        this->updateGeneric();
    }

    explicit Path(std::string     const & path
                , update_generic const & UNUSED_PARAM(empty_value))
            : Path(path){
        this->updateGeneric();
    }

    Path(std::initializer_list<std::string> list) {
        for (auto cursor : list) {
            this->append(cursor);
        }
    }

    Path(Path const & obj) {
        this->copy(obj);
    }

    Path(Path && obj) {
        this->swap(obj);
    }

// Destructor.
public:
    ~Path() {
        __EMPTY_BLOCK__
    }

// Assign operators.
public:
    Path & operator =(char const * path) {
        this->_path.assign(path);
        return *this;
    }

    Path & operator =(std::string const & path) {
        if (&(this->_path) != &path) {
            this->_path = path;
        }
        return *this;
    }

    Path & operator =(std::string && path) {
        if (&(this->_path) != &path) {
            this->_path.swap(path);
        }
        return *this;
    }

    Path & operator =(Path const & obj) {
        return this->copy(obj);
    }

    Path & operator =(Path && obj) {
        this->swap(obj);
        return *this;
    }

public:
    Path & copy(Path const & obj) {
        if (this != &obj) {
            this->_path = obj._path;
        }
        return *this;
    }

    void swap(Path & obj) {
        if (this != &obj) {
            this->_path.swap(obj._path);
        }
    }

// Accessors & Mutators.
public:
    inline std::string getString() const noexcept {
        return this->_path;
    }

    inline void setString(std::string const & path) {
        this->_path.assign(path);
    }

    inline void setString(char const * path) {
        this->_path.assign(path);
    }

// Path string.
public:
    /**
     * Generic path format.
     */
    std::string getGenericString() const {
        return NativePath::getGeneric(this->_path);
    }

    Path getGeneric() const {
        return Path(getGenericString());
    }

    /**
     * Operating system dependent path.
     */
    std::string getNativeString() const {
        return NativePath::getNative(this->_path);
    }

    Path getNative() const {
        return Path(getNativeString());
    }

// Modifiers.
public:
    Path & updateGeneric() {
        setString(getGenericString());
        return *this;
    }

    Path & updateNative() {
        setString(getNativeString());
        return *this;
    }

// Decomposition.
public:
    /**
     * root-directory, if @c _path includes root-directory,
     * otherwise empty string.
     */
    std::string getRootDirString() const {
        return NativePath::getRootDir(this->_path);
    }

    Path getRootDir() const {
        return Path(getRootDirString());
    }

// Query.
public:
    bool isAbsolute() const {
        return NativePath::isAbsolute(this->_path);
    }

    bool isRelative() const {
        return NativePath::isRelative(this->_path);
    }

// Append.
public:
    Path & append(std::string const & child) {
        // 문지열이 공백일 경우, 경로 분리자를 삽입하면 루트가 되는 현상을 방지한다.
        if (!this->_path.empty() && this->_path.back() != PATH_SEPARATOR) {
            this->_path += PATH_SEPARATOR_STRING;
        }
        this->_path += child;
        return *this;
    }

    Path & operator /=(std::string const & child) {
        return append(child);
    }

    Path & operator +=(std::string const & child) {
        return append(child);
    }

// Casting
public:
    operator std::string () const {
        return this->_path;
    }

// Parent.
public:
    std::string getParentString() const {
        return NativePath::getParent(this->_path);
    }

    Path getParent() const {
        return Path(getParentString());
    }

// Node operators.
public:
    std::vector<std::string> splitNodes() const {
        return NativePath::splitNodes(this->_path);
    }

// Directory shortcut.
public:
    static std::string replaceHomeDirectoryShortcut(std::string const & path) {
        if (!path.empty() && path[0] == DIRECTORY_SHORTCUT_HOME[0]) {
            return Common::getHomeDir() + path.substr(1);
        }
        return path;
    }

// Canonical operators.
public:
};

} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_PATH_HPP__

