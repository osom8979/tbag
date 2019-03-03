/**
 * @file   AlignedMemory.hpp
 * @brief  Method prototype that support memory alignment.
 * @author zer0
 * @date   2018-01-08
 * @date   2019-03-03 (Change namespace: libtbag::memory -> libtbag::memory::details)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MEMORY_DETAILS_ALIGNEDMEMORY_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MEMORY_DETAILS_ALIGNEDMEMORY_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <cstddef>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace memory  {
namespace details {

/**
 * @remarks
 *  - The address of the allocated memory will be a multiple of alignment, @n
 *    which must be a power of two and a multiple of sizeof(void *).
 *  - Variables of type _m128 are automatically aligned on 16-byte boundaries.
 *
 * @see <https://linux.die.net/man/3/posix_memalign>
 * @see <https://msdn.microsoft.com/en-US/library/ayeb3ayc.aspx>
 */
TBAG_API std::size_t getDefaultAlignedSize() TBAG_NOEXCEPT;

TBAG_API void * alignedMemoryAlloc(std::size_t size, std::size_t align);

TBAG_API void alignedMemoryFree(void * ptr);

} // namespace details
} // namespace memory

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MEMORY_DETAILS_ALIGNEDMEMORY_HPP__

