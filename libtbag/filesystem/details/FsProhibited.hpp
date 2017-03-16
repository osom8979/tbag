/**
 * @file   FsProhibited.hpp
 * @brief  FsProhibited class prototype.
 * @author zer0
 * @date   2017-03-05
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_FSPROHIBITED_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_FSPROHIBITED_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/filesystem/details/FsTypes.hpp>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {
namespace details    {

// -------------
namespace unix {
// -------------

/**
 * Characters prohibited in unix filename.
 *
 * @translate{ko, Unix에서 금지된(Prohibited) 문자를 확인한다.}
 *
 * @remarks
 *  - 0x00
 *  - '/'
 */
template <typename CharType>
inline bool isProhibitedChar(CharType v) TBAG_NOEXCEPT
{
    return v == '\0' || v == '/';
}

template <typename CharType>
inline bool isPathSeparatorChar(CharType v) TBAG_NOEXCEPT
{
    return v == PATH_SEPARATOR_OF_POSIX;
}

TBAG_API bool isProhibitedName(std::string const & utf8_path);

// ----------------
} // namespace unix
// ----------------

// ----------------
namespace windows {
// ----------------

/**
 * Characters prohibited in windows filename.
 *
 * @translate{ko, Windows에서 금지된(Prohibited) 문자를 확인한다.}
 *
 * @remarks
 *  Many operating systems prohibit the ASCII control characters (0x00-0x1F) in filenames.
 *  - 0x00-0x1F
 *  - '"'
 *  - '*'
 *  - '*'
 *  - '<'
 *  - '>'
 *  - '?'
 *  - '\\' (single backslash)
 *  - '/'
 *  - '|'
 */
template <typename CharType>
inline bool isProhibitedChar(CharType v) TBAG_NOEXCEPT
{
    //if (0x00 <= COMPARE_AND(v) <= 0x1F) {
    //    return true;
    //}

    switch (v) {
    case  '*': case '<': case '>':
    case  '?': case '/': case '|':
    case '\\':
        return true;
    }
    return false;
}

template <typename CharType>
inline bool isPathSeparatorChar(CharType v) TBAG_NOEXCEPT
{
    return v == PATH_SEPARATOR_OF_WINDOWS || v == PATH_SEPARATOR_OF_POSIX;
}

TBAG_API bool isProhibitedName(std::string const & utf8_path);

// ------------------
}// namespace windows
// ------------------

template <typename CharType>
inline bool isProhibitedChar(CharType v) TBAG_NOEXCEPT
{
    return TBAG_FS_PLATFORM_NAMESPACE::isProhibitedChar<CharType>(v);
}

template <typename CharType>
inline bool isPathSeparatorChar(CharType v) TBAG_NOEXCEPT
{
    return TBAG_FS_PLATFORM_NAMESPACE::isPathSeparatorChar<CharType>(v);
}

inline bool isProhibitedName(std::string const & utf8_path)
{
    return TBAG_FS_PLATFORM_NAMESPACE::isProhibitedName(utf8_path);
}

} // namespace details
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_FSPROHIBITED_HPP__

