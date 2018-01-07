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
#include <limits>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace id {

using LargeId  = uint64_t;
using MediumId = uint32_t;
using SmallId  = uint16_t;

/** Common ID type. */
using Id = std::size_t;

#if defined(TBAG_COMP_MSVC)
# if defined(max)
TBAG_PUSH_MACRO(max);
# undef max
# define __RESTORE_MAX__
# endif // defined(max)
#endif

TBAG_CONSTEXPR LargeId  const UNKNOWN_LARGE_ID  = std::numeric_limits< LargeId>::max();
TBAG_CONSTEXPR MediumId const UNKNOWN_MEDIUM_ID = std::numeric_limits<MediumId>::max();
TBAG_CONSTEXPR SmallId  const UNKNOWN_SMALL_ID  = std::numeric_limits< SmallId>::max();

/** Unknown ID. */
TBAG_CONSTEXPR Id const UNKNOWN_ID = std::numeric_limits<Id>::max();

#if defined(TBAG_COMP_MSVC)
# if defined(__RESTORE_MAX__)
TBAG_POP_MACRO(max);
# undef __RESTORE_MAX__
# endif // defined(__RESTORE_MAX__)
#endif

static_assert(sizeof(Id)          == sizeof(std::size_t)   , "Id size mismatch.");
static_assert(sizeof(std::size_t) == sizeof(void*)         , "Id size mismatch.");
static_assert(sizeof(void*)       == sizeof(std::ptrdiff_t), "Id size mismatch.");

inline bool isUnknownId( LargeId id) TBAG_NOEXCEPT { return id ==  UNKNOWN_LARGE_ID; }
inline bool isUnknownId(MediumId id) TBAG_NOEXCEPT { return id == UNKNOWN_MEDIUM_ID; }
inline bool isUnknownId( SmallId id) TBAG_NOEXCEPT { return id ==  UNKNOWN_SMALL_ID; }
inline bool isUnknownId(      Id id) TBAG_NOEXCEPT { return id ==        UNKNOWN_ID; }

inline bool existsId( LargeId id) TBAG_NOEXCEPT { return !isUnknownId(id); }
inline bool existsId(MediumId id) TBAG_NOEXCEPT { return !isUnknownId(id); }
inline bool existsId( SmallId id) TBAG_NOEXCEPT { return !isUnknownId(id); }
inline bool existsId(      Id id) TBAG_NOEXCEPT { return !isUnknownId(id); }

} // namespace id

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ID_ID_HPP__

