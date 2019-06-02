/**
 * @file   Simd.cpp
 * @brief  Simd class implementation.
 * @author zer0
 * @date   2018-01-07
 */

#include <libtbag/system/Simd.hpp>
#include <libtbag/string/Format.hpp>
#include <config-ex.h>

#include <utility>
#include <algorithm>
#include <sstream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace system {

char const * const SimdArch::getArchName(Arch arch) TBAG_NOEXCEPT
{
    switch (arch) {
    case Arch::NONE_NULL       : return "NONE_NULL";
    case Arch::X86_SSE2        : return "X86_SSE2";
    case Arch::X86_SSE3        : return "X86_SSE3";
    case Arch::X86_SSSE3       : return "X86_SSSE3";
    case Arch::X86_SSE4_1      : return "X86_SSE4_1";
    case Arch::X86_POPCNT_INSN : return "X86_POPCNT_INSN";
    case Arch::X86_AVX         : return "X86_AVX";
    case Arch::X86_AVX2        : return "X86_AVX2";
    case Arch::X86_FMA3        : return "X86_FMA3";
    case Arch::X86_FMA4        : return "X86_FMA4";
    case Arch::X86_AVX512F     : return "X86_AVX512F";
    case Arch::X86_AVX512BW    : return "X86_AVX512BW";
    case Arch::X86_AVX512DQ    : return "X86_AVX512DQ";
    case Arch::X86_AVX512VL    : return "X86_AVX512VL";
    case Arch::X86_XOP         : return "X86_XOP";
    case Arch::ARM_NEON        : return "ARM_NEON";
    case Arch::ARM_NEON_FLT_SP : return "ARM_NEON_FLT_SP";
    case Arch::ARM64_NEON      : return "ARM64_NEON";
    case Arch::MIPS_MSA        : return "MIPS_MSA";
    case Arch::POWER_ALTIVEC   : return "POWER_ALTIVEC";
    case Arch::POWER_VSX_206   : return "POWER_VSX_206";
    case Arch::POWER_VSX_207   : return "POWER_VSX_207";
    default                    : return "UNKNOWN";
    }
}

SimdArch::SimdArch() TBAG_NOEXCEPT : _flags(NONE_NULL)
{
#if defined(SIMDPP_RUNNABLE_ARCH_X86_SSE2)
    _flags |= X86_SSE2;
#endif
#if defined(SIMDPP_RUNNABLE_ARCH_X86_SSE3)
    _flags |= X86_SSE3;
#endif
#if defined(SIMDPP_RUNNABLE_ARCH_X86_SSSE3)
    _flags |= X86_SSSE3;
#endif
#if defined(SIMDPP_RUNNABLE_ARCH_X86_SSE4_1)
    _flags |= X86_SSE4_1;
#endif
#if defined(SIMDPP_RUNNABLE_ARCH_X86_POPCNT_INSN)
    _flags |= X86_POPCNT_INSN;
#endif
#if defined(SIMDPP_RUNNABLE_ARCH_X86_AVX)
    _flags |= X86_AVX;
#endif
#if defined(SIMDPP_RUNNABLE_ARCH_X86_AVX2)
    _flags |= X86_AVX2;
#endif
#if defined(SIMDPP_RUNNABLE_ARCH_X86_FMA3)
    _flags |= X86_FMA3;
#endif
#if defined(SIMDPP_RUNNABLE_ARCH_X86_FMA4)
    _flags |= X86_FMA4;
#endif
#if defined(SIMDPP_RUNNABLE_ARCH_X86_AVX512F)
    _flags |= X86_AVX512F;
#endif
#if defined(SIMDPP_RUNNABLE_ARCH_X86_AVX512BW)
    _flags |= X86_AVX512BW;
#endif
#if defined(SIMDPP_RUNNABLE_ARCH_X86_AVX512DQ)
    _flags |= X86_AVX512DQ;
#endif
#if defined(SIMDPP_RUNNABLE_ARCH_X86_AVX512VL)
    _flags |= X86_AVX512VL;
#endif
#if defined(SIMDPP_RUNNABLE_ARCH_X86_XOP)
    _flags |= X86_XOP;
#endif
#if defined(SIMDPP_RUNNABLE_ARCH_ARM_NEON)
    _flags |= ARM_NEON;
#endif
#if defined(SIMDPP_RUNNABLE_ARCH_ARM_NEON_FLT_SP)
    _flags |= ARM_NEON_FLT_SP;
#endif
#if defined(SIMDPP_RUNNABLE_ARCH_ARM64_NEON)
    _flags |= ARM64_NEON;
#endif
#if defined(SIMDPP_RUNNABLE_ARCH_MIPS_MSA)
    _flags |= MIPS_MSA;
#endif
#if defined(SIMDPP_RUNNABLE_ARCH_POWER_ALTIVEC)
    _flags |= POWER_ALTIVEC;
#endif
#if defined(SIMDPP_RUNNABLE_ARCH_POWER_VSX_206)
    _flags |= POWER_VSX_206;
#endif
#if defined(SIMDPP_RUNNABLE_ARCH_POWER_VSX_207)
    _flags |= POWER_VSX_207;
#endif
}

SimdArch::SimdArch(std::uint32_t flags) TBAG_NOEXCEPT : _flags(flags)
{
    // EMPTY.
}

SimdArch::SimdArch(SimdArch const & obj) TBAG_NOEXCEPT
{
    *this = obj;
}

SimdArch::SimdArch(SimdArch && obj) TBAG_NOEXCEPT
{
    *this = std::move(obj);
}

SimdArch::~SimdArch()
{
    // EMPTY.
}

SimdArch & SimdArch::operator =(SimdArch const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

SimdArch & SimdArch::operator =(SimdArch && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void SimdArch::copy(SimdArch const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _flags = obj._flags;
    }
}

void SimdArch::swap(SimdArch & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        std::swap(_flags, obj._flags);
    }
}

std::string SimdArch::toBinaryString() const
{
    return libtbag::string::fformat("{:0>21b}", _flags);
}

std::string SimdArch::toString() const
{
    std::stringstream ss;
    if (isNONE_NULL()) {
        ss << '[' << getArchName(Arch::NONE_NULL) << ']';
        return ss.str();
    }

    // clang-format off
    if (isX86_SSE2       ()) { ss << '[' << getArchName(Arch::X86_SSE2       ) << ']'; }
    if (isX86_SSE3       ()) { ss << '[' << getArchName(Arch::X86_SSE3       ) << ']'; }
    if (isX86_SSSE3      ()) { ss << '[' << getArchName(Arch::X86_SSSE3      ) << ']'; }
    if (isX86_SSE4_1     ()) { ss << '[' << getArchName(Arch::X86_SSE4_1     ) << ']'; }
    if (isX86_POPCNT_INSN()) { ss << '[' << getArchName(Arch::X86_POPCNT_INSN) << ']'; }
    if (isX86_AVX        ()) { ss << '[' << getArchName(Arch::X86_AVX        ) << ']'; }
    if (isX86_AVX2       ()) { ss << '[' << getArchName(Arch::X86_AVX2       ) << ']'; }
    if (isX86_FMA3       ()) { ss << '[' << getArchName(Arch::X86_FMA3       ) << ']'; }
    if (isX86_FMA4       ()) { ss << '[' << getArchName(Arch::X86_FMA4       ) << ']'; }
    if (isX86_AVX512F    ()) { ss << '[' << getArchName(Arch::X86_AVX512F    ) << ']'; }
    if (isX86_AVX512BW   ()) { ss << '[' << getArchName(Arch::X86_AVX512BW   ) << ']'; }
    if (isX86_AVX512DQ   ()) { ss << '[' << getArchName(Arch::X86_AVX512DQ   ) << ']'; }
    if (isX86_AVX512VL   ()) { ss << '[' << getArchName(Arch::X86_AVX512VL   ) << ']'; }
    if (isX86_XOP        ()) { ss << '[' << getArchName(Arch::X86_XOP        ) << ']'; }
    if (isARM_NEON       ()) { ss << '[' << getArchName(Arch::ARM_NEON       ) << ']'; }
    if (isARM_NEON_FLT_SP()) { ss << '[' << getArchName(Arch::ARM_NEON_FLT_SP) << ']'; }
    if (isARM64_NEON     ()) { ss << '[' << getArchName(Arch::ARM64_NEON     ) << ']'; }
    if (isMIPS_MSA       ()) { ss << '[' << getArchName(Arch::MIPS_MSA       ) << ']'; }
    if (isPOWER_ALTIVEC  ()) { ss << '[' << getArchName(Arch::POWER_ALTIVEC  ) << ']'; }
    if (isPOWER_VSX_206  ()) { ss << '[' << getArchName(Arch::POWER_VSX_206  ) << ']'; }
    if (isPOWER_VSX_207  ()) { ss << '[' << getArchName(Arch::POWER_VSX_207  ) << ']'; }
    // clang-format on
    return ss.str();
}

} // namespace system

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

