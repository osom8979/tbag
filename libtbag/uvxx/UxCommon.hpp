/**
 * @file   UxCommon.hpp
 * @brief  UxCommon class prototype.
 * @author zer0
 * @date   2018-11-07
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXCOMMON_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXCOMMON_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <atomic>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvxx {

enum class UxState : int
{
    UX_NONE,
    UX_READY,
    UX_RUNNING,
    UX_CLOSED,
};

using UxStateAtomic = std::atomic<UxState>;

TBAG_API char const * getUxStateName(UxState state) TBAG_NOEXCEPT;

} // namespace uvxx

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXCOMMON_HPP__

