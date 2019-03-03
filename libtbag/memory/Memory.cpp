/**
 * @file   Memory.cpp
 * @brief  Memory class implementation.
 * @author zer0
 * @date   2019-03-03
 */

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/memory/details/AlignedMemory.hpp>

#include <cstddef>
#include <cstdlib>
#include <cstdio>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace memory {

TBAG_CONSTEXPR int const TBAG_MEMORY_LEVEL0 = 0;
TBAG_CONSTEXPR int const TBAG_MEMORY_LEVEL1 = 1;
TBAG_CONSTEXPR int const TBAG_MEMORY_LEVEL2 = 2;

TBAG_CONSTEXPR int const TBAG_MEMORY_LEVEL_MIN = TBAG_MEMORY_LEVEL0;
TBAG_CONSTEXPR int const TBAG_MEMORY_LEVEL_MAX = TBAG_MEMORY_LEVEL2;

TBAG_CONSTEXPR int const TBAG_MEMORY_NO_TRACE     = TBAG_MEMORY_LEVEL0;
TBAG_CONSTEXPR int const TBAG_MEMORY_STDERR_TRACE = TBAG_MEMORY_LEVEL1;
TBAG_CONSTEXPR int const TBAG_MEMORY_FILE_TRACE   = TBAG_MEMORY_LEVEL2;

/** Memory trace level. */
TBAG_CONSTEXPR int getTraceLevel() TBAG_NOEXCEPT
{
#if defined(TBAG_MEMORY_TRACE_LEVEL)
    return TBAG_MEMORY_TRACE_LEVEL;
#else
    return TBAG_MEMORY_NO_TRACE;
#endif
}

static_assert(TBAG_MEMORY_LEVEL_MIN <= COMPARE_AND(getTraceLevel()) <= TBAG_MEMORY_LEVEL_MAX,
              "Invalid TBAG_MEMORY_TRACE_LEVEL macro value.");

/**
 * TraceDyMem class prototype.
 *
 * @author zer0
 * @date   2017-06-10
 * @date   2019-03-03 (Change namespace: libtbag::memory::alloc -> libtbag::memory)
 */
template <int Level>
struct TraceDyMem;

template <>
struct TraceDyMem<TBAG_MEMORY_NO_TRACE>
{
    TBAG_CONSTEXPR static int const value = TBAG_MEMORY_NO_TRACE;

    /** Allocate and zero-initialize array. */
    static void * _calloc(std::size_t num, std::size_t size)
    {
        return ::calloc(num, size);
    }

    /** Deallocate memory block. */
    static void _free(void * ptr)
    {
        ::free(ptr);
    }

    /** Allocate memory block. */
    static void * _malloc(std::size_t size)
    {
        return ::malloc(size);
    }

    /** Reallocate memory block. */
    static void * _realloc(void * ptr, std::size_t size)
    {
        return ::realloc(ptr, size);
    }

    /** Deallocate aligned memory block. */
    static void _aligned_free(void * ptr)
    {
        libtbag::memory::details::alignedMemoryFree(ptr);
    }

    /** Allocate aligned memory block. */
    static void * _aligned_malloc(std::size_t size, std::size_t align)
    {
        return libtbag::memory::details::alignedMemoryAlloc(size, align);
    }
};

template <>
struct TraceDyMem<TBAG_MEMORY_STDERR_TRACE>
{
    TBAG_CONSTEXPR static int const value = TBAG_MEMORY_STDERR_TRACE;

    /** Allocate and zero-initialize array. */
    static void * _calloc(std::size_t num, std::size_t size)
    {
        void * result = ::calloc(num, size);
        fprintf(stderr, "_calloc(%zu, %zu) -> %p\n", num, size, result);
        return result;
    }

    /** Deallocate memory block. */
    static void _free(void * ptr)
    {
        fprintf(stderr, "_free(%p)\n", ptr);
        ::free(ptr);
    }

    /** Allocate memory block. */
    static void * _malloc(std::size_t size)
    {
        void * result = ::malloc(size);
        fprintf(stderr, "_malloc(%zu) -> %p\n", size, result);
        return result;
    }

    /** Reallocate memory block. */
    static void * _realloc(void * ptr, std::size_t size)
    {
        void * result = ::realloc(ptr, size);
        fprintf(stderr, "_realloc(%p, %zu) -> %p\n", ptr, size, result);
        return result;
    }

    /** Deallocate aligned memory block. */
    static void _aligned_free(void * ptr)
    {
        fprintf(stderr, "_aligned_free(%p)\n", ptr);
        libtbag::memory::details::alignedMemoryFree(ptr);
    }

    /** Allocate aligned memory block. */
    static void * _aligned_malloc(std::size_t size, std::size_t align)
    {
        void * result = libtbag::memory::details::alignedMemoryAlloc(size, align);
        fprintf(stderr, "_aligned_malloc(%zu, %zu) -> %p\n", size, align, result);
        return result;
    }
};

template <typename T>
inline T * addressof(T & val) TBAG_NOEXCEPT
{
    return (T*)&reinterpret_cast<const volatile char&>(val);
}

} // namespace memory

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#include <libtbag/memory/Memory.hpp>

void * tbCalloc(std::size_t num, std::size_t size)
{
    using namespace libtbag::memory;
    return TraceDyMem<getTraceLevel()>::_calloc(num, size);
}

void tbFree(void * ptr)
{
    using namespace libtbag::memory;
    TraceDyMem<getTraceLevel()>::_free(ptr);
}

void * tbMalloc(std::size_t size)
{
    using namespace libtbag::memory;
    return TraceDyMem<getTraceLevel()>::_malloc(size);
}

void * tbRealloc(void * ptr, std::size_t size)
{
    using namespace libtbag::memory;
    return TraceDyMem<getTraceLevel()>::_realloc(ptr, size);
}

void tbAlignedFree(void * ptr)
{
    using namespace libtbag::memory;
    TraceDyMem<getTraceLevel()>::_aligned_free(ptr);
}

void * tbAlignedMalloc(std::size_t size, std::size_t align)
{
    using namespace libtbag::memory;
    return TraceDyMem<getTraceLevel()>::_aligned_malloc(size, align);
}

int tbDefaultAlignSize()
{
    using namespace libtbag::memory::details;
    return static_cast<int>(getDefaultAlignedSize());
}

