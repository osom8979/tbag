/**
 * @file   TraceNew.hpp
 * @brief  TraceNew class prototype.
 * @author zer0
 * @date   2017-06-10
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MEMORY_ALLOC_TRACENEW_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MEMORY_ALLOC_TRACENEW_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#ifndef TBAG_MEMORY_TRACE_LEVEL
#define TBAG_MEMORY_TRACE_LEVEL 2
#endif
#include <libtbag/memory/alloc/TraceDyMem.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace memory {
namespace alloc  {

template <typename T>
inline T * addressof(T & val) TBAG_NOEXCEPT
{
    return (T*)&reinterpret_cast<const volatile char&>(val);
}

inline void * allocate(std::size_t size)
{
    return malloc(size);
}

inline void deallocate(void * ptr)
{
    return free(ptr);
}

} // namespace alloc
} // namespace memory

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MEMORY_ALLOC_TRACENEW_HPP__

