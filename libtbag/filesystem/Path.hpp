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

#include <string>
#include <regex>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {

constexpr char const GetPathSeparator() noexcept
{
#if defined(__OS_WINDOWS__)
    return '\\';
#else
    return '/';
#endif
}

constexpr char const GetGenericPathSeparator() noexcept
{
    return '/';
}

constexpr char const GetPathSplitter() noexcept
{
#if defined(__OS_WINDOWS__)
    return ';';
#else
    return ':';
#endif
}

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

    BaseString getGeneric() const noexcept {
        std::regex  regexp(R"((\\|\/)(\\|\/)*)");
        std::string replace_string = std::string() + GetGenericPathSeparator();
        return std::regex_replace(this->_path, regexp, replace_string);
    }

    BaseString getNative() const noexcept {
        std::regex  regexp(R"((\\|\/)(\\|\/)*)");
        std::string replace_string = std::string() + GetPathSeparator();
        return std::regex_replace(this->_path, regexp, replace_string);
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

