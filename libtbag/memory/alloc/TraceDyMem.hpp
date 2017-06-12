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

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace memory {
namespace alloc  {

TBAG_CONSTEXPR int const TBAG_MEMORY_LEVEL0 = 0;
TBAG_CONSTEXPR int const TBAG_MEMORY_LEVEL1 = 1;
TBAG_CONSTEXPR int const TBAG_MEMORY_LEVEL2 = 2;
TBAG_CONSTEXPR int const TBAG_MEMORY_LEVEL3 = 3;
TBAG_CONSTEXPR int const TBAG_MEMORY_LEVEL4 = 4;

TBAG_CONSTEXPR int const TBAG_MEMORY_LEVEL_MIN = TBAG_MEMORY_LEVEL0;
TBAG_CONSTEXPR int const TBAG_MEMORY_LEVEL_MAX = TBAG_MEMORY_LEVEL4;

TBAG_CONSTEXPR int const TBAG_MEMORY_NO_TRACE     = TBAG_MEMORY_LEVEL0;
TBAG_CONSTEXPR int const TBAG_MEMORY_STDERR_TRACE = TBAG_MEMORY_LEVEL1;
TBAG_CONSTEXPR int const TBAG_MEMORY_LOG_TRACE    = TBAG_MEMORY_LEVEL2;
TBAG_CONSTEXPR int const TBAG_MEMORY_SIMPLE_TRACE = TBAG_MEMORY_LEVEL3;
TBAG_CONSTEXPR int const TBAG_MEMORY_STACK_TRACE  = TBAG_MEMORY_LEVEL4;

/** Memory trace level. */
TBAG_CONSTEXPR int getTraceLevel() TBAG_NOEXCEPT
{
    return TBAG_MEMORY_TRACE_LEVEL;
}

static_assert(TBAG_MEMORY_LEVEL_MIN <= COMPARE_AND(getTraceLevel()) <= TBAG_MEMORY_LEVEL_MAX,
              "Invalid TBAG_MEMORY_TRACE_LEVEL macro value.");

enum class TraceLevel : int
{
    TL_NO     = TBAG_MEMORY_NO_TRACE,
    TL_STDERR = TBAG_MEMORY_STDERR_TRACE,
    TL_LOG    = TBAG_MEMORY_LOG_TRACE,
    TL_SIMPLE = TBAG_MEMORY_SIMPLE_TRACE,
    TL_STACK  = TBAG_MEMORY_STACK_TRACE,
};

/**
 * TraceDyMem class prototype.
 *
 * @author zer0
 * @date   2017-06-10
 */
template <int LevelValue = TBAG_MEMORY_LOG_TRACE>
struct TraceDyMem : private Noncopyable
{
    TBAG_CONSTEXPR static TraceLevel getTraceLevel() TBAG_NOEXCEPT
    { return static_cast<TraceLevel>(LevelValue); }

    /** Allocate and zero-initialize array. */
    static void * _calloc(std::size_t num, std::size_t size)
    {
        void * result = ::calloc(num, size);

        if (getTraceLevel() == TraceLevel::TL_STDERR) {
            fprintf(stderr, "_calloc(%zu, %zu) -> %p\n", num, size, result);
        } else if (getTraceLevel() == TraceLevel::TL_LOG) {
            tDLogI("_calloc({}, {}) -> {}", num, size, result);
        } else if (getTraceLevel() == TraceLevel::TL_SIMPLE) {
        } else if (getTraceLevel() == TraceLevel::TL_STACK) {
        }

        return result;
    }

    /** Deallocate memory block. */
    static void _free(void * ptr)
    {
        if (getTraceLevel() == TraceLevel::TL_STDERR) {
            fprintf(stderr, "_free(%p)\n", ptr);
        } else if (getTraceLevel() == TraceLevel::TL_LOG) {
            tDLogI("_free({})", ptr);
        } else if (getTraceLevel() == TraceLevel::TL_SIMPLE) {
        } else if (getTraceLevel() == TraceLevel::TL_STACK) {
        }

        ::free(ptr);
    }

    /** Allocate memory block. */
    static void * _malloc(std::size_t size)
    {
        void * result = ::malloc(size);

        if (getTraceLevel() == TraceLevel::TL_STDERR) {
            fprintf(stderr, "_malloc(%zu) -> %p\n", size, result);
        } else if (getTraceLevel() == TraceLevel::TL_LOG) {
            tDLogI("_malloc({}) -> {}", size, result);
        } else if (getTraceLevel() == TraceLevel::TL_SIMPLE) {
        } else if (getTraceLevel() == TraceLevel::TL_STACK) {
        }

        return result;
    }

    /** Reallocate memory block. */
    static void * _realloc(void * ptr, std::size_t size)
    {
        void * result = ::realloc(ptr, size);

        if (getTraceLevel() == TraceLevel::TL_STDERR) {
            fprintf(stderr, "_realloc(%p, %zu) -> %p\n", ptr, size, result);
        } else if (getTraceLevel() == TraceLevel::TL_LOG) {
            tDLogI("_realloc({}, {}) -> {}", ptr, size, result);
        } else if (getTraceLevel() == TraceLevel::TL_SIMPLE) {
        } else if (getTraceLevel() == TraceLevel::TL_STACK) {
        }

        return result;
    }
};

} // namespace alloc
} // namespace memory

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#if !defined(TBAG_NO_REDEFINE_MEMORY_FUNCTIONS)
# define calloc(num, size)   ::libtbag::memory::alloc::TraceDyMem<TBAG_MEMORY_TRACE_LEVEL>::_calloc(num, size)
# define free(ptr)           ::libtbag::memory::alloc::TraceDyMem<TBAG_MEMORY_TRACE_LEVEL>::_free(ptr)
# define malloc(size)        ::libtbag::memory::alloc::TraceDyMem<TBAG_MEMORY_TRACE_LEVEL>::_malloc(size)
# define realloc(ptr, size)  ::libtbag::memory::alloc::TraceDyMem<TBAG_MEMORY_TRACE_LEVEL>::_realloc(ptr, size)
#endif

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MEMORY_ALLOC_TRACEDYMEM_HPP__

