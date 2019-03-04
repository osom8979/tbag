/**
 * @file   RedefineMemory.hpp
 * @brief  RedefineMemory class prototype.
 * @author zer0
 * @date   2019-03-04
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MEMORY_REDEFINEMEMORY_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MEMORY_REDEFINEMEMORY_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/memory/Memory.hpp>

// ---------------
// Standard macros
// ---------------

#ifdef calloc
#undef calloc
#endif
#define calloc(num, size) tbCalloc(num, size)

#ifdef free
#undef free
#endif
#define free(ptr) tbFree(ptr)

#ifdef malloc
#undef malloc
#endif
#define malloc(size) tbMalloc(size)

#ifdef realloc
#undef realloc
#endif
#define realloc(ptr, size) tbRealloc(ptr, size)

// ----------------
// Extension macros
// ----------------

#ifndef aligned_free
#define aligned_free(ptr) tbAlignedFree(ptr)
#endif

#ifndef aligned_malloc
#define aligned_malloc(size, align) tbAlignedMalloc(size, align)
#endif

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MEMORY_REDEFINEMEMORY_HPP__

