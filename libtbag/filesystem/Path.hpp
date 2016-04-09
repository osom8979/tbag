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

// Constructors.
public:
    Path() noexcept {
        __EMPTY_BLOCK__
    }

    explicit Path(BaseString const & path) : Path() {
        this->_path.assign(path);
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

// Destructor.
public:
    ~Path() {
        __EMPTY_BLOCK__
    }

// Assign operators.
public:
    Path & operator =(BaseType const * path) {
        this->_path.assign(path);
        return *this;
    }

    Path & operator =(BaseString const & path) {
        if (&(this->_path) != &path) {
            this->_path = path;
        }
        return *this;
    }

    Path & operator =(BaseString && path) {
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
    inline BaseString getString() const noexcept {
        return this->_path;
    }

    inline void setString(BaseString const & path) {
        this->_path.assign(path);
    }

    inline void setString(BaseType const * path) {
        this->_path.assign(path);
    }

public:
    constexpr static BaseType const * const REMOVE_SEPARATOR_REGEX_OF_WINDOWS = R"((\\|\/)(\\|\/)*)";
    constexpr static BaseType const * const REMOVE_SEPARATOR_REGEX_OF_POSIX   = R"(\/\/)*)";

    inline static BaseString
    removeRegex(BaseString const & path, BaseString const & regex) {
        return std::regex_replace(path, std::regex(regex), "");
    }

public:
    inline static BaseString
    removeLastSeparatorOfWindows(BaseString const & path) {
        return Path::removeRegex(path, std::string(REMOVE_SEPARATOR_REGEX_OF_WINDOWS) + "$");
    }

    inline static BaseString
    removeLastSeparatorOfPosix(BaseString const & path) {
        return Path::removeRegex(path, std::string(REMOVE_SEPARATOR_REGEX_OF_POSIX) + "$");
    }

    inline static BaseString removeLastSeparator(BaseString const & path) {
#if defined(__OS_WINDOWS__)
        return Path::removeLastSeparatorOfWindows(path);
#else
        return Path::removeLastSeparatorOfPosix(path);
#endif
    }

// Generic string.
public:
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

public:
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

public:
    /**
     * Clean an overlapped separators.
     *
     * @param path      [in] Path string.
     * @param separator [in] Separator string.
     *
     * @return Cleared path string.
     */
    static BaseString cleanSeparator(BaseString const & path, BaseString const & separator) {
#if defined(__OS_WINDOWS__)
        return Path::cleanSeparatorOfWindows(path, separator);
#else
        return Path::cleanSeparatorOfPosix(path, separator);
#endif
    }

    static BaseString cleanSeparatorOfWindows(BaseString const & path, BaseString const & separator) {
        std::string temp = std::regex_replace(path, std::regex(R"((\\|\/)(\\|\/)*)"), separator);
        return Path::removeLastSeparatorOfWindows(std::move(temp));
    }

    static BaseString cleanSeparatorOfPosix(BaseString const & path, BaseString const & separator) {
        std::string temp = std::regex_replace(path, std::regex(R"(\/\/*)"), separator);
        return Path::removeLastSeparatorOfPosix(std::move(temp));
    }

// DECOMPOSITION
public:
    BaseString getRootDir() const noexcept {
        return Path::getRootDir(this->_path);
    }

    /**
     * root-directory, if @c _path includes root-directory,
     * otherwise empty string.
     *
     * @remarks
     *  Don't use regex library.
     */
    static BaseString getRootDir(BaseString const & path) noexcept {
#if defined(__OS_WINDOWS__)
        return Path::getRootDirOfWindows(path);
#else
        return Path::getRootDirOfPosix(path);
#endif
    }

    static BaseString getRootDirOfWindows(BaseString const & path) noexcept {
        if (path.size() < 2 || path[1] != ':') {
            return BaseString();
        }
        if (/**/('a' <= COMPARE_AND(path[0]) <= 'z')
             || ('A' <= COMPARE_AND(path[0]) <= 'Z')) {
            return path.substr(0, 2);
        }
        return BaseString();
    }

    static BaseString getRootDirOfPosix(BaseString const & path) noexcept {
        if (path.size() < 1 || path[0] != PATH_SEPARATOR_OF_POSIX) {
            return BaseString();
        }
        return BaseString(PATH_SEPARATOR_STRING_OF_POSIX);
    }

// QUERY
public:
    bool isAbsolute() const noexcept {
        return Path::isAbsolute(this->_path);
    }

    static bool isAbsolute(BaseString const & path) noexcept {
#if defined(__OS_WINDOWS__)
        return Path::isAbsoluteOfWindows(path);
#else
        return Path::isAbsoluteOfPosix(path);
#endif
    }

    static bool isAbsoluteOfWindows(BaseString const & path) noexcept {
        return (getRootDirOfWindows(path).empty() == false);
    }

    static bool isAbsoluteOfPosix(BaseString const & path) noexcept {
        return (getRootDirOfPosix(path).empty() == false);
    }

// FILENAME QUERY.
public:
    static bool isProhibitedFilename(BaseString const & path) noexcept {
#if defined(__OS_WINDOWS__)
        return Path::isProhibitedFilenameOfWindows(path);
#else
        return Path::isProhibitedFilenameOfPosix(path);
#endif
    }

    static bool isProhibitedFilenameOfWindows(BaseString const & path) noexcept {
        for (auto cursor : path) {
            if (0x00 <= COMPARE_AND(cursor) <= 0x1F) {
                return true;
            }

            switch (cursor) {
            case '*': case '<': case '>':
            case '?': case '/': case '|': case '\\':
                return true;
            }
        }
        return false;
    }

    static bool isProhibitedFilenameOfPosix(BaseString const & path) noexcept {
        for (auto cursor : path) {
            if (cursor == 0x00 || cursor == '/') {
                return true;
            }
        }
        return false;
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
        using ltype   = Path::BaseString;
        using rtype   = strings::BaseString;
        using is_same = std::is_same<ltype, rtype>;
        static_assert(is_same::value, "This is not a same type.");

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

