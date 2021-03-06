/**
 * @file   Memory.hpp
 * @brief  Memory class prototype.
 * @author zer0
 * @date   2019-03-03
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MEMORY_MEMORY_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MEMORY_MEMORY_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <cstddef>

#if defined(__cplusplus)
extern "C" {
#endif

TBAG_API void * tbCalloc(std::size_t num, std::size_t size);
TBAG_API void   tbFree(void * ptr);
TBAG_API void * tbMalloc(std::size_t size);
TBAG_API void * tbRealloc(void * ptr, std::size_t size);
TBAG_API void   tbAlignedFree(void * ptr);
TBAG_API void * tbAlignedMalloc(std::size_t size, std::size_t align);
TBAG_API int    tbDefaultAlignSize();

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MEMORY_MEMORY_HPP__

