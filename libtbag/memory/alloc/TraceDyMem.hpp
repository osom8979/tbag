/**
 * @file   TraceDyMem.hpp
 * @brief  TraceDyMem class prototype.
 * @author zer0
 * @date   2017-06-10
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MEMORY_ALLOC_TRACEDYMEM_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MEMORY_ALLOC_TRACEDYMEM_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <cstddef>

#ifndef TBAG_MEMORY_TRACE_LEVEL
#define TBAG_MEMORY_TRACE_LEVEL 0
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace memory {
namespace alloc  {

TBAG_CONSTEXPR int const TBAG_MEMORY_LEVEL0 = 0;
TBAG_CONSTEXPR int const TBAG_MEMORY_LEVEL1 = 1;
TBAG_CONSTEXPR int const TBAG_MEMORY_LEVEL2 = 2;

TBAG_CONSTEXPR int const TBAG_MEMORY_NO_TRACE     = TBAG_MEMORY_LEVEL0;
TBAG_CONSTEXPR int const TBAG_MEMORY_SIMPLE_TRACE = TBAG_MEMORY_LEVEL1;
TBAG_CONSTEXPR int const TBAG_MEMORY_STACK_TRACE  = TBAG_MEMORY_LEVEL2;

/** Memory trace level. */
TBAG_CONSTEXPR int getTraceLevel() TBAG_NOEXCEPT
{
    return TBAG_MEMORY_TRACE_LEVEL;
}

/** Allocate and zero-initialize array. */
void * calloc(std::size_t num, std::size_t size);

/** Deallocate memory block. */
void free(void * ptr);

/** Allocate memory block. */
void * malloc(std::size_t size);

/** Reallocate memory block. */
void * realloc(void * ptr, std::size_t size);

} // namespace alloc
} // namespace memory

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MEMORY_ALLOC_TRACEDYMEM_HPP__

