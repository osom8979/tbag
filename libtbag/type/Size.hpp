/**
 * @file   Size.hpp
 * @brief  Size class prototype.
 * @author zer0
 * @date   2019-07-23
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TYPE_SIZE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TYPE_SIZE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

// Not found ssize_t in MSVC.
#if defined(TBAG_COMP_MSVC)
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace type {

// EMPTY.

} // namespace type

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TYPE_SIZE_HPP__

