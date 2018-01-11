/**
 * @file   AlignedMemory.cpp
 * @brief  AlignedMemory class implementation.
 * @author zer0
 * @date   2018-01-08
 */

#include <libtbag/memory/AlignedMemory.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/system/Simd.hpp>
#include <libtbag/math/Number.hpp>

#include <cstdlib>

#if defined(TBAG_COMP_MSVC)
#include <malloc.h>
#endif

/** Throw exception if not aligned memory. */
#define _THROW_MEMORY_ERROR

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace memory {

TBAG_CONSTEXPR bool isThrowMemoryError() TBAG_NOEXCEPT
{
#if defined(_THROW_MEMORY_ERROR)
    return true;
#else
    return false;
#endif
}

std::size_t getDefaultAlignedSize() TBAG_NOEXCEPT
{
    // [WARNING] On certain architectures, e.g. armv7 NEON,
    // 128 bit vectors are not necessarily aligned to 16 bytes on the stack.
    static bool USE_POWER_ALTIVEC = system::SimdArch().isPOWER_ALTIVEC();
    return (USE_POWER_ALTIVEC ? TBAG_ALIGNMENT_SIZE_16BYTE : TBAG_ALIGNMENT_SIZE_64BYTE);
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
        tDLogE("__impl::alignedMemoryAlloc() posix_memalign({}) error",
               getErrName(convertSystemErrorToErr(CODE)));
        return nullptr;
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

} // namespace memory

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

