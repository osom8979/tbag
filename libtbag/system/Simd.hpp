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
#include <libtbag/bitwise/BitFlags.hpp>

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
        NONE_NULL       = (0U),
        X86_SSE2        = (1U<< 0U),
        X86_SSE3        = (1U<< 1U),
        X86_SSSE3       = (1U<< 2U),
        X86_SSE4_1      = (1U<< 3U),
        X86_POPCNT_INSN = (1U<< 4U),
        X86_AVX         = (1U<< 5U),
        X86_AVX2        = (1U<< 6U),
        X86_FMA3        = (1U<< 7U),
        X86_FMA4        = (1U<< 8U),
        X86_AVX512F     = (1U<< 9U),
        X86_AVX512BW    = (1U<<10U),
        X86_AVX512DQ    = (1U<<11U),
        X86_AVX512VL    = (1U<<12U),
        X86_XOP         = (1U<<13U),
        ARM_NEON        = (1U<<14U),
        ARM_NEON_FLT_SP = (1U<<15U),
        ARM64_NEON      = (1U<<16U),
        MIPS_MSA        = (1U<<17U),
        POWER_ALTIVEC   = (1U<<18U),
        POWER_VSX_206   = (1U<<19U),
        POWER_VSX_207   = (1U<<20U), // Total 21 flags.
    };

public:
    static char const * const getArchName(Arch arch) TBAG_NOEXCEPT;

public:
    // clang-format off
    TBAG_CONSTEXPR static std::uint32_t const NONE_NULL       = static_cast<std::uint32_t>(Arch::NONE_NULL      );
    TBAG_CONSTEXPR static std::uint32_t const X86_SSE2        = static_cast<std::uint32_t>(Arch::X86_SSE2       );
    TBAG_CONSTEXPR static std::uint32_t const X86_SSE3        = static_cast<std::uint32_t>(Arch::X86_SSE3       );
    TBAG_CONSTEXPR static std::uint32_t const X86_SSSE3       = static_cast<std::uint32_t>(Arch::X86_SSSE3      );
    TBAG_CONSTEXPR static std::uint32_t const X86_SSE4_1      = static_cast<std::uint32_t>(Arch::X86_SSE4_1     );
    TBAG_CONSTEXPR static std::uint32_t const X86_POPCNT_INSN = static_cast<std::uint32_t>(Arch::X86_POPCNT_INSN);
    TBAG_CONSTEXPR static std::uint32_t const X86_AVX         = static_cast<std::uint32_t>(Arch::X86_AVX        );
    TBAG_CONSTEXPR static std::uint32_t const X86_AVX2        = static_cast<std::uint32_t>(Arch::X86_AVX2       );
    TBAG_CONSTEXPR static std::uint32_t const X86_FMA3        = static_cast<std::uint32_t>(Arch::X86_FMA3       );
    TBAG_CONSTEXPR static std::uint32_t const X86_FMA4        = static_cast<std::uint32_t>(Arch::X86_FMA4       );
    TBAG_CONSTEXPR static std::uint32_t const X86_AVX512F     = static_cast<std::uint32_t>(Arch::X86_AVX512F    );
    TBAG_CONSTEXPR static std::uint32_t const X86_AVX512BW    = static_cast<std::uint32_t>(Arch::X86_AVX512BW   );
    TBAG_CONSTEXPR static std::uint32_t const X86_AVX512DQ    = static_cast<std::uint32_t>(Arch::X86_AVX512DQ   );
    TBAG_CONSTEXPR static std::uint32_t const X86_AVX512VL    = static_cast<std::uint32_t>(Arch::X86_AVX512VL   );
    TBAG_CONSTEXPR static std::uint32_t const X86_XOP         = static_cast<std::uint32_t>(Arch::X86_XOP        );
    TBAG_CONSTEXPR static std::uint32_t const ARM_NEON        = static_cast<std::uint32_t>(Arch::ARM_NEON       );
    TBAG_CONSTEXPR static std::uint32_t const ARM_NEON_FLT_SP = static_cast<std::uint32_t>(Arch::ARM_NEON_FLT_SP);
    TBAG_CONSTEXPR static std::uint32_t const ARM64_NEON      = static_cast<std::uint32_t>(Arch::ARM64_NEON     );
    TBAG_CONSTEXPR static std::uint32_t const MIPS_MSA        = static_cast<std::uint32_t>(Arch::MIPS_MSA       );
    TBAG_CONSTEXPR static std::uint32_t const POWER_ALTIVEC   = static_cast<std::uint32_t>(Arch::POWER_ALTIVEC  );
    TBAG_CONSTEXPR static std::uint32_t const POWER_VSX_206   = static_cast<std::uint32_t>(Arch::POWER_VSX_206  );
    TBAG_CONSTEXPR static std::uint32_t const POWER_VSX_207   = static_cast<std::uint32_t>(Arch::POWER_VSX_207  );
    // clang-format on

public:
    // clang-format off
    TBAG_CONSTEXPR static std::uint32_t const FULL_ARCHS =
            X86_SSE2        |
            X86_SSE3        |
            X86_SSSE3       |
            X86_SSE4_1      |
            X86_POPCNT_INSN |
            X86_AVX         |
            X86_AVX2        |
            X86_FMA3        |
            X86_FMA4        |
            X86_AVX512F     |
            X86_AVX512BW    |
            X86_AVX512DQ    |
            X86_AVX512VL    |
            X86_XOP         |
            ARM_NEON        |
            ARM_NEON_FLT_SP |
            ARM64_NEON      |
            MIPS_MSA        |
            POWER_ALTIVEC   |
            POWER_VSX_206   |
            POWER_VSX_207;
    // clang-format on

private:
    std::uint32_t _flags;

public:
    SimdArch() TBAG_NOEXCEPT;
    SimdArch(std::uint32_t flags) TBAG_NOEXCEPT;
    SimdArch(SimdArch const & obj) TBAG_NOEXCEPT;
    SimdArch(SimdArch && obj) TBAG_NOEXCEPT;
    ~SimdArch();

public:
    SimdArch & operator =(SimdArch const & obj) TBAG_NOEXCEPT;
    SimdArch & operator =(SimdArch && obj) TBAG_NOEXCEPT;

public:
    void copy(SimdArch const & obj) TBAG_NOEXCEPT;
    void swap(SimdArch & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(SimdArch & lh, SimdArch & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }

public:
    inline std::uint32_t getFlags() const TBAG_NOEXCEPT { return _flags; }

public:
    // clang-format off
    inline bool isNONE_NULL      () const TBAG_NOEXCEPT { return _flags == NONE_NULL; }
    inline bool isX86_SSE2       () const TBAG_NOEXCEPT { return libtbag::bitwise::checkFlag(_flags, X86_SSE2       ); }
    inline bool isX86_SSE3       () const TBAG_NOEXCEPT { return libtbag::bitwise::checkFlag(_flags, X86_SSE3       ); }
    inline bool isX86_SSSE3      () const TBAG_NOEXCEPT { return libtbag::bitwise::checkFlag(_flags, X86_SSSE3      ); }
    inline bool isX86_SSE4_1     () const TBAG_NOEXCEPT { return libtbag::bitwise::checkFlag(_flags, X86_SSE4_1     ); }
    inline bool isX86_POPCNT_INSN() const TBAG_NOEXCEPT { return libtbag::bitwise::checkFlag(_flags, X86_POPCNT_INSN); }
    inline bool isX86_AVX        () const TBAG_NOEXCEPT { return libtbag::bitwise::checkFlag(_flags, X86_AVX        ); }
    inline bool isX86_AVX2       () const TBAG_NOEXCEPT { return libtbag::bitwise::checkFlag(_flags, X86_AVX2       ); }
    inline bool isX86_FMA3       () const TBAG_NOEXCEPT { return libtbag::bitwise::checkFlag(_flags, X86_FMA3       ); }
    inline bool isX86_FMA4       () const TBAG_NOEXCEPT { return libtbag::bitwise::checkFlag(_flags, X86_FMA4       ); }
    inline bool isX86_AVX512F    () const TBAG_NOEXCEPT { return libtbag::bitwise::checkFlag(_flags, X86_AVX512F    ); }
    inline bool isX86_AVX512BW   () const TBAG_NOEXCEPT { return libtbag::bitwise::checkFlag(_flags, X86_AVX512BW   ); }
    inline bool isX86_AVX512DQ   () const TBAG_NOEXCEPT { return libtbag::bitwise::checkFlag(_flags, X86_AVX512DQ   ); }
    inline bool isX86_AVX512VL   () const TBAG_NOEXCEPT { return libtbag::bitwise::checkFlag(_flags, X86_AVX512VL   ); }
    inline bool isX86_XOP        () const TBAG_NOEXCEPT { return libtbag::bitwise::checkFlag(_flags, X86_XOP        ); }
    inline bool isARM_NEON       () const TBAG_NOEXCEPT { return libtbag::bitwise::checkFlag(_flags, ARM_NEON       ); }
    inline bool isARM_NEON_FLT_SP() const TBAG_NOEXCEPT { return libtbag::bitwise::checkFlag(_flags, ARM_NEON_FLT_SP); }
    inline bool isARM64_NEON     () const TBAG_NOEXCEPT { return libtbag::bitwise::checkFlag(_flags, ARM64_NEON     ); }
    inline bool isMIPS_MSA       () const TBAG_NOEXCEPT { return libtbag::bitwise::checkFlag(_flags, MIPS_MSA       ); }
    inline bool isPOWER_ALTIVEC  () const TBAG_NOEXCEPT { return libtbag::bitwise::checkFlag(_flags, POWER_ALTIVEC  ); }
    inline bool isPOWER_VSX_206  () const TBAG_NOEXCEPT { return libtbag::bitwise::checkFlag(_flags, POWER_VSX_206  ); }
    inline bool isPOWER_VSX_207  () const TBAG_NOEXCEPT { return libtbag::bitwise::checkFlag(_flags, POWER_VSX_207  ); }
    // clang-format on

public:
    std::string toBinaryString() const;
    std::string toString() const;
};

} // namespace system

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_SYSTEM_SIMD_HPP__

