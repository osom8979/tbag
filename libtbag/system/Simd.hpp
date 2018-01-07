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

TBAG_API std::uint32_t tbGetSimdArch();

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
private:
    std::uint32_t _flags;

public:
    SimdArch();
    SimdArch(std::uint32_t flags);
    ~SimdArch();

public:
    inline std::uint32_t getFlags() const TBAG_NOEXCEPT { return _flags; }

public:
    bool isNULL           () const TBAG_NOEXCEPT;
    bool isX86_SSE2       () const TBAG_NOEXCEPT;
    bool isX86_SSE3       () const TBAG_NOEXCEPT;
    bool isX86_SSSE3      () const TBAG_NOEXCEPT;
    bool isX86_SSE4_1     () const TBAG_NOEXCEPT;
    bool isX86_POPCNT_INSN() const TBAG_NOEXCEPT;
    bool isX86_AVX        () const TBAG_NOEXCEPT;
    bool isX86_AVX2       () const TBAG_NOEXCEPT;
    bool isX86_FMA3       () const TBAG_NOEXCEPT;
    bool isX86_FMA4       () const TBAG_NOEXCEPT;
    bool isX86_XOP        () const TBAG_NOEXCEPT;
    bool isX86_AVX512F    () const TBAG_NOEXCEPT;
    bool isX86_AVX512BW   () const TBAG_NOEXCEPT;
    bool isX86_AVX512DQ   () const TBAG_NOEXCEPT;
    bool isX86_AVX512VL   () const TBAG_NOEXCEPT;
    bool isARM_NEON       () const TBAG_NOEXCEPT;
    bool isARM_NEON_FLT_SP() const TBAG_NOEXCEPT;
    bool isPOWER_ALTIVEC  () const TBAG_NOEXCEPT;
    bool isPOWER_VSX_206  () const TBAG_NOEXCEPT;
    bool isPOWER_VSX_207  () const TBAG_NOEXCEPT;
    bool isMIPS_MSA       () const TBAG_NOEXCEPT;

public:
    std::string toString() const;
};

} // namespace system

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_SYSTEM_SIMD_HPP__

