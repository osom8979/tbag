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
 *
 * @warning
 *  Supports multibyte-string only.
 */
template <typename CharType = char>
class BasePath : public BaseNativePath<CharType>
{
public:
    using ImplPath   = BasePath<CharType>;
    using NativePath = BaseNativePath<CharType>;
    using ValueType  = typename NativePath::ValueType;
    using String     = typename NativePath::String;

    static_assert(std::is_pod<ValueType>::value
            , "Character type of PosixPath must be a POD");
    static_assert(std::is_same<ValueType, typename String::value_type>::value
            , "String::value_type must be the same type as ValueType");

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
        return !isWindowsStyle();
    }

private:
    String _path;

// Constructors.
public:
    BasePath() noexcept(std::is_nothrow_default_constructible<String>::value) {
        __EMPTY_BLOCK__
    }

    explicit BasePath(String const & path) : BasePath() {
        this->_path.assign(path);
    }

    explicit BasePath(ValueType const * path) : BasePath(String(path)) {
        __EMPTY_BLOCK__
    }

    explicit BasePath(ValueType      const * path
                    , update_generic const & UNUSED_PARAM(empty_value))
            : BasePath(String(path)){
        this->updateGeneric();
    }

    explicit BasePath(String         const & path
                    , update_generic const & UNUSED_PARAM(empty_value))
            : BasePath(path){
        this->updateGeneric();
    }

    BasePath(std::initializer_list<String> list) {
        for (auto cursor : list) {
            this->append(cursor);
        }
    }

    BasePath(BasePath const & obj) {
        this->copy(obj);
    }

    BasePath(BasePath && obj) {
        this->swap(obj);
    }

// Destructor.
public:
    ~BasePath() {
        __EMPTY_BLOCK__
    }

// Assign operators.
public:
    BasePath & operator =(ValueType const * path) {
        this->_path.assign(path);
        return *this;
    }

    BasePath & operator =(String const & path) {
        if (&(this->_path) != &path) {
            this->_path = path;
        }
        return *this;
    }

    BasePath & operator =(String && path) {
        if (&(this->_path) != &path) {
            this->_path.swap(path);
        }
        return *this;
    }

    BasePath & operator =(BasePath const & obj) {
        return this->copy(obj);
    }

    BasePath & operator =(BasePath && obj) {
        this->swap(obj);
        return *this;
    }

public:
    BasePath & copy(BasePath const & obj) {
        if (this != &obj) {
            this->_path = obj._path;
        }
        return *this;
    }

    void swap(BasePath & obj) {
        if (this != &obj) {
            this->_path.swap(obj._path);
        }
    }

// Accessors & Mutators.
public:
    inline String getString() const noexcept {
        return this->_path;
    }

    inline void setString(String const & path) {
        this->_path.assign(path);
    }

    inline void setString(ValueType const * path) {
        this->_path.assign(path);
    }

// Path string.
public:
    /**
     * Generic path format.
     */
    String getGenericString() const {
        return NativePath::getGeneric(this->_path);
    }

    BasePath getGeneric() const {
        return BasePath(getGenericString());
    }

    /**
     * Operating system dependent path.
     */
    String getNativeString() const {
        return NativePath::getNative(this->_path);
    }

    BasePath getNative() const {
        return BasePath(getNativeString());
    }

// Modifiers.
public:
    BasePath & updateGeneric() {
        setString(getGenericString());
        return *this;
    }

    BasePath & updateNative() {
        setString(getNativeString());
        return *this;
    }

// Decomposition.
public:
    /**
     * root-directory, if @c _path includes root-directory,
     * otherwise empty string.
     */
    String getRootDirString() const {
        return NativePath::getRootDir(this->_path);
    }

    BasePath getRootDir() const {
        return BasePath(getRootDirString());
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
    static String append(String const & parent, String const & child) {
        String result = parent;
        // 문지열이 공백일 경우, 경로 분리자를 삽입하면 루트가 되는 현상을 방지한다.
        if (!parent.empty() && parent.back() != NativePath::PATH_SEPARATOR) {
            result += NativePath::getPathSeparator();
        }
        result += child;
        return result;
    }

    BasePath & append(String const & child) {
        this->_path = append(this->_path, child);
        return *this;
    }

    BasePath & operator /=(String const & child) {
        return append(child);
    }

    BasePath & operator +=(String const & child) {
        return append(child);
    }

// Casting
public:
    operator String () const {
        return this->_path;
    }

    operator ValueType const * () const {
        return this->_path.c_str();
    }

// Parent.
public:
    String getParentString() const {
        return NativePath::getParent(this->_path);
    }

    BasePath getParent() const {
        return BasePath(getParentString());
    }

// Node operators.
public:
    std::vector<String> splitNodes() const {
        return NativePath::splitNodes(this->_path);
    }
};

using Path = BasePath<char>;
using WidePath = BasePath<wchar_t>;

} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_PATH_HPP__

