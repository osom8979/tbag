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

#include <string>
#include <regex>

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
    using CharType = char;
    using BaseString = std::basic_string<CharType>;

private:
    BaseString _path;

public:
    Path() {
        __EMPTY_BLOCK__;
    }

    explicit Path(CharType const * path) {
        this->_path = BaseString(path);
    }

    Path(BaseString const & path) {
        this->_path = path;
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
    BaseString getGeneric() const noexcept {
        return Path::cleanSeparator(this->_path, std::string() + GetGenericPathSeparator());
    }

    /**
     * Operating system dependent path.
     */
    BaseString getNative() const noexcept {
        return Path::cleanSeparator(this->_path, std::string() + GetPathSeparator());
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
        return std::regex_replace(path, std::regex(R"((\\|\/)(\\|\/)*)"), separator);
    }

// APPEND
public:
    void append(BaseString const & sub) {
    }

    Path & operator /= (BaseString const & sub) {
        return *this;
    }
    Path & operator += (BaseString const & sub) {
        return *this;
    }

public:
    static std::vector<BaseString> splitNodes(BaseString const & path
                                            , BaseString const & separator) {
        std::vector<BaseString> result;
        return result;
    }
};

} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_PATH_HPP__

