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
#include <cstdint>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace id {

using LargeId  = uint64_t;
using MediumId = uint32_t;
using SmallId  = uint16_t;

/** Common ID type. */
using Id = LargeId;

TBAG_CONSTEXPR LargeId  const UNKNOWN_LARGE_ID  = 0;
TBAG_CONSTEXPR MediumId const UNKNOWN_MEDIUM_ID = 0;
TBAG_CONSTEXPR SmallId  const UNKNOWN_SMALL_ID  = 0;

/** Unknown ID. */
TBAG_CONSTEXPR Id const UNKNOWN_ID = 0;

} // namespace id

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ID_ID_HPP__

