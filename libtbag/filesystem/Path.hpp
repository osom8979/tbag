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
#include <libtbag/Strings.hpp>

#include <string>
#include <regex>
#include <type_traits>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {

/**
 * Path class prototype.
 *
 * @author zer0
 * @date   2016-04-07
 *
 * @warning
 *  Supports multibyte-string only.
 */
class Path
{
public:
    /** Update Generic Format. */
    struct update_generic { __EMPTY_BLOCK__ };

public:
    using BaseType = char;
    using BaseString = std::basic_string<BaseType>;

private:
    BaseString _path;

public:
    Path() {
        __EMPTY_BLOCK__
    }

    explicit Path(BaseString const & path) : Path() {
        this->_path = path;
    }

    explicit Path(BaseType const * path) : Path(BaseString(path)) {
        __EMPTY_BLOCK__
    }

    explicit Path(BaseType       const * path
                , update_generic const & UNUSED_PARAM(empty_value))
            : Path(BaseString(path)){
        this->updateGeneric();
    }

    explicit Path(BaseString     const & path
                , update_generic const & UNUSED_PARAM(empty_value))
            : Path(path){
        this->updateGeneric();
    }

    Path(Path const & obj) {
        this->copy(obj);
    }

    Path(Path && obj) {
        this->swap(obj);
    }

    ~Path() {
        __EMPTY_BLOCK__;
    }

public:
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

// Path to String.
public:
    BaseString getString() const noexcept {
        return this->_path;
    }

    /**
     * Generic path format.
     */
    BaseString getGeneric() const {
        return Path::cleanSeparator(this->_path, std::string() + GetGenericPathSeparator());
    }

    Path & updateGeneric() {
        this->_path = getGeneric();
        return *this;
    }

    /**
     * Operating system dependent path.
     */
    BaseString getNative() const {
        return Path::cleanSeparator(this->_path, std::string() + GetPathSeparator());
    }

    Path & updateNative() {
        this->_path = getNative();
        return *this;
    }

    static BaseString removeLastSeparator(BaseString const & path) {
        return std::regex_replace(path, std::regex(R"((\\|\/)(\\|\/)*$)"), "");
    }

    /**
     * Clean an overlapped separators.
     *
     * @param path      [in] Path string.
     * @param separator [in] Separator string.
     *
     * @return Cleared path string.
     */
    static BaseString cleanSeparator(BaseString const & path, BaseString const & separator) {
        std::string temp = std::regex_replace(path, std::regex(R"((\\|\/)(\\|\/)*)"), separator);
        return removeLastSeparator(std::move(temp));
    }

// DECOMPOSITION
public:
    /**
     * root-directory, if @c _path includes root-directory,
     * otherwise empty string.
     *
     * @remarks
     *  Don't use regex library.
     */
    BaseString getRootDir() const {
#if defined(__OS_WINDOWS__)
        return getRootDirOfWindows();
#else
        return getRootDirOfPosix();
#endif
    }

    BaseString getRootDirOfWindows() const {
        if (this->_path.size() < 2) {
            return "";
        }
        if (this->_path[1] != ':') {
            return "";
        }
        if (/**/('a' <= COMPARE_AND(this->_path[0]) <= 'z')
             || ('A' <= COMPARE_AND(this->_path[0]) <= 'Z')) {
            return this->_path.substr(0, 2);
        }
        return "";
    }

    BaseString getRootDirOfPosix() const {
        if (this->_path.empty() || this->_path[0] != '/') {
            return "";
        }
        return "/";
    }

// QUERY
public:
    bool isAbsolute() const {
#if defined(__OS_WINDOWS__)
        return isAbsoluteOfWindows();
#else
        return isAbsoluteOfPosix();
#endif
    }

    bool isAbsoluteOfWindows() const {
        return !(getRootDirOfWindows().empty());
    }

    bool isAbsoluteOfPosix() const {
        return !(getRootDirOfPosix().empty());
    }

// APPEND
public:
    Path & append(BaseString const & sub) {
        this->_path += GetGenericPathSeparator() + sub;
        return *this;
    }

    Path & operator /= (BaseString const & sub) {
        return append(sub);
    }

    Path & operator += (BaseString const & sub) {
        return append(sub);
    }

public:
    std::vector<BaseString> splitNodes() {
        return Path::splitNodes(this->_path);
    }

    static std::vector<BaseString> splitNodes(BaseString const & path) {
        static_assert(std::is_same<Path::BaseString, strings::BaseString>::value, "This is not a same type.");
        std::vector<BaseString> result;
        Path generic(path, Path::update_generic());
        std::string separator = std::string() + GetGenericPathSeparator();
        return strings::splitTokens(generic.getString(), separator);
    }
};

} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_PATH_HPP__

