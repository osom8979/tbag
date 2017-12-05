/**
 * @file   CpuId.hpp
 * @brief  CpuId class prototype.
 * @author zer0
 * @date   2017-12-05
 *
 * @see <https://clang.llvm.org/doxygen/cpuid_8h_source.html>
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_SYSTEM_CPUID_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_SYSTEM_CPUID_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

/**
 * @defgroup __DOXYGEN_GROUP__CPU_SIGNATURES__ List of cpu signatures
 * @remarks
 *  Responses identification request with %eax 0
 * @{
 */

// AMD: "AuthenticAMD"
#define TBAG_SIGNATURE_AMD_EBX     0x68747541
#define TBAG_SIGNATURE_AMD_EDX     0x69746e65
#define TBAG_SIGNATURE_AMD_ECX     0x444d4163

// CENTAUR: "CentaurHauls"
#define TBAG_SIGNATURE_CENTAUR_EBX 0x746e6543
#define TBAG_SIGNATURE_CENTAUR_EDX 0x48727561
#define TBAG_SIGNATURE_CENTAUR_ECX 0x736c7561

// CYRIX: "CyrixInstead"
#define TBAG_SIGNATURE_CYRIX_EBX   0x69727943
#define TBAG_SIGNATURE_CYRIX_EDX   0x736e4978
#define TBAG_SIGNATURE_CYRIX_ECX   0x64616574

// INTEL: "GenuineIntel"
#define TBAG_SIGNATURE_INTEL_EBX   0x756e6547
#define TBAG_SIGNATURE_INTEL_EDX   0x49656e69
#define TBAG_SIGNATURE_INTEL_ECX   0x6c65746e

// TM1: "TransmetaCPU"
#define TBAG_SIGNATURE_TM1_EBX     0x6e617254
#define TBAG_SIGNATURE_TM1_EDX     0x74656d73
#define TBAG_SIGNATURE_TM1_ECX     0x55504361

// TM2: "GenuineTMx86"
#define TBAG_SIGNATURE_TM2_EBX     0x756e6547
#define TBAG_SIGNATURE_TM2_EDX     0x54656e69
#define TBAG_SIGNATURE_TM2_ECX     0x3638784d

// NSC: "Geode by NSC"
#define TBAG_SIGNATURE_NSC_EBX     0x646f6547
#define TBAG_SIGNATURE_NSC_EDX     0x43534e20
#define TBAG_SIGNATURE_NSC_ECX     0x79622065

// NEXGEN: "NexGenDriven"
#define TBAG_SIGNATURE_NEXGEN_EBX  0x4778654e
#define TBAG_SIGNATURE_NEXGEN_EDX  0x72446e65
#define TBAG_SIGNATURE_NEXGEN_ECX  0x6e657669

// RISE: "RiseRiseRise"
#define TBAG_SIGNATURE_RISE_EBX    0x65736952
#define TBAG_SIGNATURE_RISE_EDX    0x65736952
#define TBAG_SIGNATURE_RISE_ECX    0x65736952

// SIS: "SiS SiS SiS "
#define TBAG_SIGNATURE_SIS_EBX     0x20536953
#define TBAG_SIGNATURE_SIS_EDX     0x20536953
#define TBAG_SIGNATURE_SIS_ECX     0x20536953

// UMC: "UMC UMC UMC "
#define TBAG_SIGNATURE_UMC_EBX      0x20434d55
#define TBAG_SIGNATURE_UMC_EDX      0x20434d55
#define TBAG_SIGNATURE_UMC_ECX      0x20434d55

// VIA: "VIA VIA VIA "
#define TBAG_SIGNATURE_VIA_EBX      0x20414956
#define TBAG_SIGNATURE_VIA_EDX      0x20414956
#define TBAG_SIGNATURE_VIA_ECX      0x20414956

// VORTEX: "Vortex86 SoC"
#define TBAG_SIGNATURE_VORTEX_EBX   0x74726f56
#define TBAG_SIGNATURE_VORTEX_EDX   0x36387865
#define TBAG_SIGNATURE_VORTEX_ECX   0x436f5320

/**
 * @}
 */

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace system {

/**
 * CpuId class prototype.
 *
 * @author zer0
 * @date   2017-12-05
 */
class TBAG_API CpuId : private Noncopyable
{
public:
    CpuId();
    virtual ~CpuId();
};

} // namespace system

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_SYSTEM_CPUID_HPP__

