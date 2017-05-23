/**
 * @file   Id.hpp
 * @brief  Id class prototype.
 * @author zer0
 * @date   2016-07-26
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_ID_ID_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_ID_ID_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <cstddef>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace id {

/** Common ID type. */
typedef std::size_t Id;

/** Unknown ID. */
TBAG_CONSTEXPR Id const UNKNOWN_ID = 0;

} // namespace id

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ID_ID_HPP__

