/**
 * @file   CdsUtils.hpp
 * @brief  CdsUtils class prototype.
 * @author zer0
 * @date   2018-11-08
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOCKFREE_CDS_CDSUTILS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOCKFREE_CDS_CDSUTILS_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace lockfree {
namespace cds      {

TBAG_API void initialize();
TBAG_API void release();

TBAG_API bool isThreaddsAttached();
TBAG_API void attachThread();
TBAG_API void detachThread();

TBAG_API void initHazardPointerSingleton();

} // namespace cds
} // namespace lockfree

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOCKFREE_CDS_CDSUTILS_HPP__

