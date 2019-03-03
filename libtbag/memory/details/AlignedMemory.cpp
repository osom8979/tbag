/**
 * @file   AlignedMemory.cpp
 * @brief  Method implementations that support memory alignment.
 * @author zer0
 * @date   2018-01-08
 * @date   2019-03-03 (Change namespace: libtbag::memory -> libtbag::memory::details)
 */

#include <libtbag/memory/details/AlignedMemory.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/math/Number.hpp>
#include <libtbag/config-ex.h>

#include <cstdlib>

#if defined(TBAG_COMP_MSVC)
#include <malloc.h>
#endif

/** Throw exception if not aligned memory. */
#define _THROW_MEMORY_ERROR

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace memory  {
namespace details {

TBAG_CONSTEXPR static bool isThrowMemoryError() TBAG_NOEXCEPT
{
#if defined(_THROW_MEMORY_ERROR)
    return true;
#else
    return false;
#endif
}

TBAG_CONSTEXPR static bool isPowerAltivec() TBAG_NOEXCEPT
{
#if defined(SIMDPP_RUNNABLE_ARCH_POWER_ALTIVEC)
    return true;
#else
    return false;
#endif
}

std::size_t getDefaultAlignedSize() TBAG_NOEXCEPT
{
    // [WARNING] On certain architectures, e.g. armv7 NEON,
    // 128 bit vectors are not necessarily aligned to 16 bytes on the stack.
    return (isPowerAltivec() ? TBAG_ALIGNMENT_SIZE_16BYTE : TBAG_ALIGNMENT_SIZE_64BYTE);
}

void * alignedMemoryAlloc(std::size_t size, std::size_t align)
{
    if (math::isMultiplesOfTwo(align) && align < sizeof(void*)) {
        align = sizeof(void*);
    }

    void * ptr = nullptr;
#if defined(TBAG_COMP_MSVC)
    ptr = _aligned_malloc(size, align);
#else
    int const CODE = ::posix_memalign(&ptr, align, size);
    if (CODE != 0) {
        tDLogE("alignedMemoryAlloc() posix_memalign({}) error",
               getErrName(convertSystemErrorToErr(CODE)));
        throw std::bad_alloc();
    }
#endif
    if (isThrowMemoryError() && reinterpret_cast<std::uintptr_t>(ptr) % align != 0) {
        throw std::bad_alloc();
    }
    return ptr;
}

void alignedMemoryFree(void * ptr)
{
#if defined(TBAG_COMP_MSVC)
    _aligned_free(ptr);
#else
    ::free(ptr);
#endif
}

} // namespace details
} // namespace memory

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

