/**
 * @file   rstr_base.hpp
 * @brief  rstr_base class prototype.
 * @author zer0
 * @date   2019-07-02
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_STRING_DETAILS_RSTR_BASE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_STRING_DETAILS_RSTR_BASE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <climits>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace string  {
namespace details {

/**
 * restricted string structure.
 *
 * @author zer0
 * @date   2019-07-01
 * @date   2019-07-02 (Move: libtbag/string -> libtbag/string/details)
 */
struct rstr_base
{
    /** Container options. */
    int flags = 0;

    /** String memory capacity. */
    int capacity = 0;

    /** String length. */
    int size = 0;

    /** String data. */
    char * data = nullptr;
};

TBAG_CONSTEXPR int const RSTR_FLAG_ALIGN  = (int)(1<<0);
TBAG_CONSTEXPR int const RSTR_FLAG_STATIC = (int)(1<<1);

TBAG_CONSTEXPR int const RSTR_STEP_CAPACITY   = 64;
TBAG_CONSTEXPR int const RSTR_SMALL_CAPACITY  = 64;
TBAG_CONSTEXPR int const RSTR_MEDIUM_CAPACITY = 256;
TBAG_CONSTEXPR int const RSTR_LARGE_CAPACITY  = 2048;
TBAG_CONSTEXPR int const RSTR_VLARGE_CAPACITY = 4096;

TBAG_CONSTEXPR int const RSTR_MAX_SIZE = EXCLUDE_NULLSIZE(INT_MAX);

TBAG_API void rstr_base_clear(rstr_base * s) TBAG_NOEXCEPT;
TBAG_API void rstr_base_malloc(rstr_base * s, int capacity, int flags) TBAG_NOEXCEPT;
TBAG_API void rstr_base_free(rstr_base * s) TBAG_NOEXCEPT;

TBAG_API void rstr_base_copy_string(rstr_base * s, char const * src, int size) TBAG_NOEXCEPT;
TBAG_API void rstr_base_capacity_extend(rstr_base * s, int capacity, int flags) TBAG_NOEXCEPT;

TBAG_API int  rstr_base_get_recommand_capacity(int capacity) TBAG_NOEXCEPT;
TBAG_API void rstr_base_recommand_capacity_extend(rstr_base * s, int capacity, int flags) TBAG_NOEXCEPT;

TBAG_API void rstr_base_assign(rstr_base * s, char const * src, int size, int flag) TBAG_NOEXCEPT;
TBAG_API void rstr_base_append(rstr_base * s, char const * src, int size, int flag) TBAG_NOEXCEPT;

} // namespace details
} // namespace string

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_STRING_DETAILS_RSTR_BASE_HPP__

