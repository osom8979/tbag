/**
 * @file   Simd.simd
 * @brief  Simd class implementation.
 * @author zer0
 * @date   2018-01-07
 */

#include <libtbag/system/Simd.hpp>
#include <libtbag/log/Log.hpp>
#include <simdpp/simd.h>

#include <sstream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace system {

// ---------------
namespace __impl {
// ---------------

inline bool testArch(std::uint32_t flags, simdpp::Arch arch) TBAG_NOEXCEPT
{
    return simdpp::test_arch_subset(simdpp::Arch(flags), arch);
}

// ------------------
} // namespace __impl
// ------------------

SimdArch::SimdArch() : _flags(tbGetSimdArch())
{
    // EMPTY.
}

SimdArch::SimdArch(std::uint32_t flags) : _flags(flags)
{
    // EMPTY.
}

SimdArch::~SimdArch()
{
    // EMPTY.
}

// @formatter:off
bool SimdArch::isNULL           () const TBAG_NOEXCEPT { return _flags == static_cast<std::uint32_t>(simdpp::Arch::NONE_NULL); }
bool SimdArch::isX86_SSE2       () const TBAG_NOEXCEPT { return __impl::testArch(_flags, simdpp::Arch::X86_SSE2       ); }
bool SimdArch::isX86_SSE3       () const TBAG_NOEXCEPT { return __impl::testArch(_flags, simdpp::Arch::X86_SSE3       ); }
bool SimdArch::isX86_SSSE3      () const TBAG_NOEXCEPT { return __impl::testArch(_flags, simdpp::Arch::X86_SSSE3      ); }
bool SimdArch::isX86_SSE4_1     () const TBAG_NOEXCEPT { return __impl::testArch(_flags, simdpp::Arch::X86_SSE4_1     ); }
bool SimdArch::isX86_POPCNT_INSN() const TBAG_NOEXCEPT { return __impl::testArch(_flags, simdpp::Arch::X86_POPCNT_INSN); }
bool SimdArch::isX86_AVX        () const TBAG_NOEXCEPT { return __impl::testArch(_flags, simdpp::Arch::X86_AVX        ); }
bool SimdArch::isX86_AVX2       () const TBAG_NOEXCEPT { return __impl::testArch(_flags, simdpp::Arch::X86_AVX2       ); }
bool SimdArch::isX86_FMA3       () const TBAG_NOEXCEPT { return __impl::testArch(_flags, simdpp::Arch::X86_FMA3       ); }
bool SimdArch::isX86_FMA4       () const TBAG_NOEXCEPT { return __impl::testArch(_flags, simdpp::Arch::X86_FMA4       ); }
bool SimdArch::isX86_XOP        () const TBAG_NOEXCEPT { return __impl::testArch(_flags, simdpp::Arch::X86_XOP        ); }
bool SimdArch::isX86_AVX512F    () const TBAG_NOEXCEPT { return __impl::testArch(_flags, simdpp::Arch::X86_AVX512F    ); }
bool SimdArch::isX86_AVX512BW   () const TBAG_NOEXCEPT { return __impl::testArch(_flags, simdpp::Arch::X86_AVX512BW   ); }
bool SimdArch::isX86_AVX512DQ   () const TBAG_NOEXCEPT { return __impl::testArch(_flags, simdpp::Arch::X86_AVX512DQ   ); }
bool SimdArch::isX86_AVX512VL   () const TBAG_NOEXCEPT { return __impl::testArch(_flags, simdpp::Arch::X86_AVX512VL   ); }
bool SimdArch::isARM_NEON       () const TBAG_NOEXCEPT { return __impl::testArch(_flags, simdpp::Arch::ARM_NEON       ); }
bool SimdArch::isARM_NEON_FLT_SP() const TBAG_NOEXCEPT { return __impl::testArch(_flags, simdpp::Arch::ARM_NEON_FLT_SP); }
bool SimdArch::isPOWER_ALTIVEC  () const TBAG_NOEXCEPT { return __impl::testArch(_flags, simdpp::Arch::POWER_ALTIVEC  ); }
bool SimdArch::isPOWER_VSX_206  () const TBAG_NOEXCEPT { return __impl::testArch(_flags, simdpp::Arch::POWER_VSX_206  ); }
bool SimdArch::isPOWER_VSX_207  () const TBAG_NOEXCEPT { return __impl::testArch(_flags, simdpp::Arch::POWER_VSX_207  ); }
bool SimdArch::isMIPS_MSA       () const TBAG_NOEXCEPT { return __impl::testArch(_flags, simdpp::Arch::MIPS_MSA       ); }
// @formatter:on

std::string SimdArch::toString() const
{
    std::stringstream ss;
    // @formatter:off
    if (isX86_SSE2       ()) { ss << "[X86_SSE2]"; }
    if (isX86_SSE3       ()) { ss << "[X86_SSE3]"; }
    if (isX86_SSSE3      ()) { ss << "[X86_SSSE3]"; }
    if (isX86_SSE4_1     ()) { ss << "[X86_SSE4_1]"; }
    if (isX86_POPCNT_INSN()) { ss << "[X86_POPCNT_INSN]"; }
    if (isX86_AVX        ()) { ss << "[X86_AVX]"; }
    if (isX86_AVX2       ()) { ss << "[X86_AVX2]"; }
    if (isX86_FMA3       ()) { ss << "[X86_FMA3]"; }
    if (isX86_FMA4       ()) { ss << "[X86_FMA4]"; }
    if (isX86_XOP        ()) { ss << "[X86_XOP]"; }
    if (isX86_AVX512F    ()) { ss << "[X86_AVX512F]"; }
    if (isX86_AVX512BW   ()) { ss << "[X86_AVX512BW]"; }
    if (isX86_AVX512DQ   ()) { ss << "[X86_AVX512DQ]"; }
    if (isX86_AVX512VL   ()) { ss << "[X86_AVX512VL]"; }
    if (isARM_NEON       ()) { ss << "[ARM_NEON]"; }
    if (isARM_NEON_FLT_SP()) { ss << "[ARM_NEON_FLT_SP]"; }
    if (isPOWER_ALTIVEC  ()) { ss << "[POWER_ALTIVEC]"; }
    if (isPOWER_VSX_206  ()) { ss << "[POWER_VSX_206]"; }
    if (isPOWER_VSX_207  ()) { ss << "[POWER_VSX_207]"; }
    if (isMIPS_MSA       ()) { ss << "[MIPS_MSA]"; }
    // @formatter:on
    return ss.str();
}

} // namespace system

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

