/**
 * @file   Simd.hpp
 * @brief  Simd class prototype.
 * @author zer0
 * @date   2018-01-07
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_SYSTEM_SIMD_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_SYSTEM_SIMD_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

#include <cstdint>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace system {

/**
 * SimdArch class prototype.
 *
 * @author zer0
 * @date   2018-01-07
 */
class TBAG_API SimdArch : private Noncopyable
{
public:
    enum class Arch : std::uint32_t
    {
        NONE_NULL,
        X86_SSE2,
        X86_SSE3,
        X86_SSSE3,
        X86_SSE4_1,
        X86_POPCNT_INSN,
        X86_AVX,
        X86_AVX2,
        X86_FMA3,
        X86_FMA4,
        X86_AVX512F,
        X86_AVX512BW,
        X86_AVX512DQ,
        X86_AVX512VL,
        X86_XOP,
        ARM_NEON,
        ARM_NEON_FLT_SP,
        ARM64_NEON,
        MIPS_MSA,
        POWER_ALTIVEC,
        POWER_VSX_206,
        POWER_VSX_207
    };

private:
    std::uint32_t _flags;

public:
    SimdArch();
    SimdArch(std::uint32_t flags);
    ~SimdArch();

public:
    inline std::uint32_t getFlags() const TBAG_NOEXCEPT { return _flags; }
};

} // namespace system

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_SYSTEM_SIMD_HPP__

