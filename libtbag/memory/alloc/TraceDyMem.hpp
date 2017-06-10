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
#include <libtbag/Noncopyable.hpp>
#include <libtbag/log/Log.hpp>

#include <cstddef>
#include <cstdio>

#ifndef TBAG_MEMORY_TRACE_LEVEL
#define TBAG_MEMORY_TRACE_LEVEL 2
#endif

#define TBAG_MEMORY_LEVEL0  0
#define TBAG_MEMORY_LEVEL1  1
#define TBAG_MEMORY_LEVEL2  2
#define TBAG_MEMORY_LEVEL3  3
#define TBAG_MEMORY_LEVEL4  4

#define TBAG_MEMORY_NO_TRACE      TBAG_MEMORY_LEVEL0
#define TBAG_MEMORY_STDERR_TRACE  TBAG_MEMORY_LEVEL1
#define TBAG_MEMORY_TDLOGI_TRACE  TBAG_MEMORY_LEVEL2
#define TBAG_MEMORY_SIMPLE_TRACE  TBAG_MEMORY_LEVEL3
#define TBAG_MEMORY_STACK_TRACE   TBAG_MEMORY_LEVEL4

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace memory {
namespace alloc  {

/** Memory trace level. */
TBAG_CONSTEXPR int getTraceLevel() TBAG_NOEXCEPT
{
    return TBAG_MEMORY_TRACE_LEVEL;
}

static_assert(TBAG_MEMORY_LEVEL0 <= COMPARE_AND(getTraceLevel()) <= TBAG_MEMORY_LEVEL4,
              "Invalid TBAG_MEMORY_TRACE_LEVEL macro value.");

/**
 * TraceDyMem class prototype.
 *
 * @author zer0
 * @date   2017-06-10
 */
struct TraceDyMem : private Noncopyable
{
    /** Allocate and zero-initialize array. */
    static void * _calloc(std::size_t num, std::size_t size)
    {
        void * result = ::calloc(num, size);
#if (TBAG_MEMORY_TRACE_LEVEL == TBAG_MEMORY_STDERR_TRACE)
        fprintf(stderr, "_calloc(%zu, %zu) -> %p\n", num, size, result);
#elif (TBAG_MEMORY_TRACE_LEVEL == TBAG_MEMORY_TDLOGI_TRACE)
        tDLogI("_calloc({}, {}) -> {}", num, size, result);
#elif (TBAG_MEMORY_TRACE_LEVEL == TBAG_MEMORY_SIMPLE_TRACE)
#elif (TBAG_MEMORY_TRACE_LEVEL == TBAG_MEMORY_STACK_TRACE)
#endif
        return result;
    }

    /** Deallocate memory block. */
    static void _free(void * ptr)
    {
#if (TBAG_MEMORY_TRACE_LEVEL == TBAG_MEMORY_STDERR_TRACE)
        fprintf(stderr, "_free(%p)\n", ptr);
#elif (TBAG_MEMORY_TRACE_LEVEL == TBAG_MEMORY_TDLOGI_TRACE)
        tDLogI("_free({})", ptr);
#elif (TBAG_MEMORY_TRACE_LEVEL == TBAG_MEMORY_SIMPLE_TRACE)
#elif (TBAG_MEMORY_TRACE_LEVEL == TBAG_MEMORY_STACK_TRACE)
#endif
        ::free(ptr);
    }

    /** Allocate memory block. */
    static void * _malloc(std::size_t size)
    {
        void * result = ::malloc(size);
#if (TBAG_MEMORY_TRACE_LEVEL == TBAG_MEMORY_STDERR_TRACE)
        fprintf(stderr, "_malloc(%zu) -> %p\n", size, result);
#elif (TBAG_MEMORY_TRACE_LEVEL == TBAG_MEMORY_TDLOGI_TRACE)
        tDLogI("_malloc({}) -> {}", size, result);
#elif (TBAG_MEMORY_TRACE_LEVEL == TBAG_MEMORY_SIMPLE_TRACE)
#elif (TBAG_MEMORY_TRACE_LEVEL == TBAG_MEMORY_STACK_TRACE)
#endif
        return result;
    }

    /** Reallocate memory block. */
    static void * _realloc(void * ptr, std::size_t size)
    {
        void * result = ::realloc(ptr, size);
#if (TBAG_MEMORY_TRACE_LEVEL == TBAG_MEMORY_STDERR_TRACE)
        fprintf(stderr, "_realloc(%p, %zu) -> %p\n", ptr, size, result);
#elif (TBAG_MEMORY_TRACE_LEVEL == TBAG_MEMORY_TDLOGI_TRACE)
        tDLogI("_realloc({}, {}) -> {}", ptr, size, result);
#elif (TBAG_MEMORY_TRACE_LEVEL == TBAG_MEMORY_SIMPLE_TRACE)
#elif (TBAG_MEMORY_TRACE_LEVEL == TBAG_MEMORY_STACK_TRACE)
#endif
        return result;
    }
};

} // namespace alloc
} // namespace memory

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#define calloc(num, size)   ::libtbag::memory::alloc::TraceDyMem::_calloc(num, size)
#define free(ptr)           ::libtbag::memory::alloc::TraceDyMem::_free(ptr)
#define malloc(size)        ::libtbag::memory::alloc::TraceDyMem::_malloc( size)
#define realloc(ptr, size)  ::libtbag::memory::alloc::TraceDyMem::_realloc(ptr, size)

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MEMORY_ALLOC_TRACEDYMEM_HPP__

