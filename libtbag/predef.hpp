/**
 * @file   predef.hpp
 * @brief  pre-define header.
 * @author zer0
 * @date   2016-04-18
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PREDEF_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PREDEF_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/macro/architecture.hpp>
#include <libtbag/macro/platform.hpp>
#include <libtbag/macro/compiler.hpp>
#include <libtbag/macro/language.hpp>

// Common.
#include <libtbag/macro/attributes.hpp>
#include <libtbag/macro/features.hpp>
#include <libtbag/macro/types.hpp>
#include <libtbag/macro/utils.hpp>

#include <libtbag/config.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

inline TBAG_CONSTEXPR bool isReleaseMode() TBAG_NOEXCEPT
{
#if defined(NDEBUG)
    return true;
#else
    return false;
#endif
}

inline TBAG_CONSTEXPR bool isDebugMode() TBAG_NOEXCEPT
{
    return !isReleaseMode();
}

inline TBAG_CONSTEXPR bool isWindowsPlatform() TBAG_NOEXCEPT
{
#if defined(TBAG_PLATFORM_WINDOWS)
    return true;
#else
    return false;
#endif
}

inline TBAG_CONSTEXPR bool isUnixLikePlatform() TBAG_NOEXCEPT
{
#if defined(TBAG_PLATFORM_UNIX_LIKE)
    return true;
#else
    return false;
#endif
}

enum class PlatformType : int
{
    PT_WINDOWS,
    PT_MACOS,
    PT_UNIX,
    PT_UNKNOWN,
};

inline PlatformType getPlatformType() TBAG_NOEXCEPT
{
#if defined(TBAG_PLATFORM_WINDOWS)
    return PlatformType::PT_WINDOWS;
#elif defined(TBAG_PLATFORM_MACOS)
    return PlatformType::PT_MACOS;
#elif defined(TBAG_PLATFORM_UNIX_LIKE)
    return PlatformType::PT_UNIX;
#else
    return PlatformType::PT_UNKNOWN;
#endif
}

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#ifndef TBAG_ASSERT_WINDOWS_NOT_IMPLEMENT
#define TBAG_ASSERT_WINDOWS_NOT_IMPLEMENT(retval)       \
    do {                                                \
        if (::libtbag::isWindowsPlatform() == false) {  \
            assert(0 && "Not implement.");              \
            return retval;                              \
        }                                               \
    } while(0)
#endif // TBAG_ASSERT_WINDOWS_NOT_IMPLEMENT

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PREDEF_HPP__

