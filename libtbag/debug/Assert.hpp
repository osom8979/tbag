/**
 * @file   Assert.hpp
 * @brief  Assert class prototype.
 * @author zer0
 * @date   2017-06-14
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_DEBUG_ASSERT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_DEBUG_ASSERT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <cassert>

#ifndef TBAG_INACCESSIBLE_BLOCK_ASSERT
#define TBAG_INACCESSIBLE_BLOCK_ASSERT() \
    assert(false && "Inaccessible block") // NOLINT
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace debug {

// EMPTY.

} // namespace debug

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_DEBUG_ASSERT_HPP__

