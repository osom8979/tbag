/**
 * @file   Simd.cpp
 * @brief  Simd class implementation.
 * @author zer0
 * @date   2018-01-07
 */

#include <libtbag/system/Simd.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace system {

SimdArch::SimdArch() : _flags(static_cast<std::uint32_t>(Arch::NONE_NULL))
{
#if defined(SIMDPP_RUNNABLE_ARCH_X86_SSE2)
    _flags |= static_cast<std::uint32_t>(X86_SSE2);
#endif
#if defined(SIMDPP_RUNNABLE_ARCH_X86_SSE3)
    _flags |= static_cast<std::uint32_t>(Arch::X86_SSE3);
#endif
#if defined(SIMDPP_RUNNABLE_ARCH_X86_SSSE3)
    _flags |= static_cast<std::uint32_t>(Arch::X86_SSSE3);
#endif
#if defined(SIMDPP_RUNNABLE_ARCH_X86_SSE4_1)
    _flags |= static_cast<std::uint32_t>(Arch::X86_SSE4_1);
#endif
#if defined(SIMDPP_RUNNABLE_ARCH_X86_POPCNT_INSN)
    _flags |= static_cast<std::uint32_t>(Arch::X86_POPCNT_INSN);
#endif
#if defined(SIMDPP_RUNNABLE_ARCH_X86_AVX)
    _flags |= static_cast<std::uint32_t>(Arch::X86_AVX);
#endif
#if defined(SIMDPP_RUNNABLE_ARCH_X86_AVX2)
    _flags |= static_cast<std::uint32_t>(Arch::X86_AVX2);
#endif
#if defined(SIMDPP_RUNNABLE_ARCH_X86_FMA3)
    _flags |= static_cast<std::uint32_t>(Arch::X86_FMA3);
#endif
#if defined(SIMDPP_RUNNABLE_ARCH_X86_FMA4)
    _flags |= static_cast<std::uint32_t>(Arch::X86_FMA4);
#endif
#if defined(SIMDPP_RUNNABLE_ARCH_X86_AVX512F)
    _flags |= static_cast<std::uint32_t>(Arch::X86_AVX512F);
#endif
#if defined(SIMDPP_RUNNABLE_ARCH_X86_AVX512BW)
    _flags |= static_cast<std::uint32_t>(Arch::X86_AVX512BW);
#endif
#if defined(SIMDPP_RUNNABLE_ARCH_X86_AVX512DQ)
    _flags |= static_cast<std::uint32_t>(Arch::X86_AVX512DQ);
#endif
#if defined(SIMDPP_RUNNABLE_ARCH_X86_AVX512VL)
    _flags |= static_cast<std::uint32_t>(Arch::X86_AVX512VL);
#endif
#if defined(SIMDPP_RUNNABLE_ARCH_X86_XOP)
    _flags |= static_cast<std::uint32_t>(Arch::X86_XOP);
#endif
#if defined(SIMDPP_RUNNABLE_ARCH_ARM_NEON)
    _flags |= static_cast<std::uint32_t>(Arch::ARM_NEON);
#endif
#if defined(SIMDPP_RUNNABLE_ARCH_ARM_NEON_FLT_SP)
    _flags |= static_cast<std::uint32_t>(Arch::ARM_NEON_FLT_SP);
#endif
#if defined(SIMDPP_RUNNABLE_ARCH_ARM64_NEON)
    _flags |= static_cast<std::uint32_t>(Arch::ARM64_NEON);
#endif
#if defined(SIMDPP_RUNNABLE_ARCH_MIPS_MSA)
    _flags |= static_cast<std::uint32_t>(Arch::MIPS_MSA);
#endif
#if defined(SIMDPP_RUNNABLE_ARCH_POWER_ALTIVEC)
    _flags |= static_cast<std::uint32_t>(Arch::POWER_ALTIVEC);
#endif
#if defined(SIMDPP_RUNNABLE_ARCH_POWER_VSX_206)
    _flags |= static_cast<std::uint32_t>(Arch::POWER_VSX_206);
#endif
#if defined(SIMDPP_RUNNABLE_ARCH_POWER_VSX_207)
    _flags |= static_cast<std::uint32_t>(Arch::POWER_VSX_207);
#endif
}

SimdArch::SimdArch(std::uint32_t flags) : _flags(flags)
{
    // EMPTY.
}

SimdArch::~SimdArch()
{
    // EMPTY.
}

} // namespace system

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

