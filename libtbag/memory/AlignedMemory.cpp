/**
 * @file   AlignedMemory.cpp
 * @brief  AlignedMemory class implementation.
 * @author zer0
 * @date   2018-01-08
 */

#include <libtbag/memory/AlignedMemory.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/system/Simd.hpp>

#include <cstdlib>

#if defined(TBAG_COMP_MSVC)
#include <malloc.h>
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace memory {

TBAG_CONSTEXPR static std::size_t const ALIGNMENT_SIZE_16BYTE = 16;
TBAG_CONSTEXPR static std::size_t const ALIGNMENT_SIZE_64BYTE = 64;

static_assert(ALIGNMENT_SIZE_16BYTE >= sizeof(void*), "Alignment size is too small.");

std::size_t alignedMemorySize() TBAG_NOEXCEPT
{
    // On certain architectures, e.g. armv7 NEON, 128 bit vectors are not
    // necessarily aligned to 16 bytes on the stack.
    // NOTE: MSVC 2013 does not support constant expressions within
    // SIMDPP_ALIGN, thus we're aligning to the alignment of the largest V
    // is going to be instantiated with
    static volatile bool USE_POWER_ALTIVEC = system::SimdArch().isPOWER_ALTIVEC();
    return (USE_POWER_ALTIVEC ? ALIGNMENT_SIZE_16BYTE : ALIGNMENT_SIZE_64BYTE);
}

void * alignedMemoryAlloc(std::size_t size)
{
    void * ptr = nullptr;
#if defined(TBAG_COMP_MSVC)
    ptr = _aligned_malloc(size, alignedMemorySize());
#else
    int const CODE = ::posix_memalign(&ptr, alignedMemorySize(), size);
    if (CODE != 0) {
        tDLogE("__impl::alignedMemoryAlloc() posix_memalign({}) error",
               getErrName(convertSystemErrorToErr(CODE)));
        return nullptr;
    }
#endif
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

