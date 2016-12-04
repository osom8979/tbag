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

#include <libtbag/macro/platform.hpp>
#include <libtbag/macro/compiler.hpp>
#include <libtbag/macro/language.hpp>

// Common.
#include <libtbag/macro/attributes.hpp>
#include <libtbag/macro/features.hpp>
#include <libtbag/macro/utils.hpp>

#include <libtbag/config.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

inline TBAG_CONSTEXPR bool isWindowsPlatform() TBAG_NOEXCEPT
{
#if defined(__PLATFORM_WINDOWS__)
    return true;
#else
    return false;
#endif
}

inline TBAG_CONSTEXPR bool isUnixLikePlatform() TBAG_NOEXCEPT
{
#if defined(__PLATFORM_UNIX_LIKE__)
    return true;
#else
    return false;
#endif
}

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PREDEF_HPP__

