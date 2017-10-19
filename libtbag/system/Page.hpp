/**
 * @file   Page.hpp
 * @brief  Page class prototype.
 * @author zer0
 * @date   2017-10-19
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_SYSTEM_PAGE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_SYSTEM_PAGE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace system {

TBAG_API int getPageSize();

} // namespace system

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_SYSTEM_PAGE_HPP__

