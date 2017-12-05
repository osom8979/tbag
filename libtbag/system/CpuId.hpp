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

#include <cstdint>
#include <string>

/**
 * @defgroup __DOXYGEN_GROUP__CPU_SIGNATURES__ List of cpu signatures
 * @remarks
 *  Responses identification request with %eax 0
 * @{
 */

#define TBAG_SIGNATURE_AMD          "AuthenticAMD"
#define TBAG_SIGNATURE_AMD_EBX      0x68747541
#define TBAG_SIGNATURE_AMD_EDX      0x69746e65
#define TBAG_SIGNATURE_AMD_ECX      0x444d4163

#define TBAG_SIGNATURE_CENTAUR      "CentaurHauls"
#define TBAG_SIGNATURE_CENTAUR_EBX  0x746e6543
#define TBAG_SIGNATURE_CENTAUR_EDX  0x48727561
#define TBAG_SIGNATURE_CENTAUR_ECX  0x736c7561

#define TBAG_SIGNATURE_CYRIX        "CyrixInstead"
#define TBAG_SIGNATURE_CYRIX_EBX    0x69727943
#define TBAG_SIGNATURE_CYRIX_EDX    0x736e4978
#define TBAG_SIGNATURE_CYRIX_ECX    0x64616574

#define TBAG_SIGNATURE_INTEL        "GenuineIntel"
#define TBAG_SIGNATURE_INTEL_EBX    0x756e6547
#define TBAG_SIGNATURE_INTEL_EDX    0x49656e69
#define TBAG_SIGNATURE_INTEL_ECX    0x6c65746e

#define TBAG_SIGNATURE_TM1          "TransmetaCPU"
#define TBAG_SIGNATURE_TM1_EBX      0x6e617254
#define TBAG_SIGNATURE_TM1_EDX      0x74656d73
#define TBAG_SIGNATURE_TM1_ECX      0x55504361

#define TBAG_SIGNATURE_TM2          "GenuineTMx86"
#define TBAG_SIGNATURE_TM2_EBX      0x756e6547
#define TBAG_SIGNATURE_TM2_EDX      0x54656e69
#define TBAG_SIGNATURE_TM2_ECX      0x3638784d

#define TBAG_SIGNATURE_NSC          "Geode by NSC"
#define TBAG_SIGNATURE_NSC_EBX      0x646f6547
#define TBAG_SIGNATURE_NSC_EDX      0x43534e20
#define TBAG_SIGNATURE_NSC_ECX      0x79622065

#define TBAG_SIGNATURE_NEXGEN       "NexGenDriven"
#define TBAG_SIGNATURE_NEXGEN_EBX   0x4778654e
#define TBAG_SIGNATURE_NEXGEN_EDX   0x72446e65
#define TBAG_SIGNATURE_NEXGEN_ECX   0x6e657669

#define TBAG_SIGNATURE_RISE         "RiseRiseRise"
#define TBAG_SIGNATURE_RISE_EBX     0x65736952
#define TBAG_SIGNATURE_RISE_EDX     0x65736952
#define TBAG_SIGNATURE_RISE_ECX     0x65736952

#define TBAG_SIGNATURE_SIS          "SiS SiS SiS "
#define TBAG_SIGNATURE_SIS_EBX      0x20536953
#define TBAG_SIGNATURE_SIS_EDX      0x20536953
#define TBAG_SIGNATURE_SIS_ECX      0x20536953

#define TBAG_SIGNATURE_UMC          "UMC UMC UMC "
#define TBAG_SIGNATURE_UMC_EBX      0x20434d55
#define TBAG_SIGNATURE_UMC_EDX      0x20434d55
#define TBAG_SIGNATURE_UMC_ECX      0x20434d55

#define TBAG_SIGNATURE_VIA          "VIA VIA VIA "
#define TBAG_SIGNATURE_VIA_EBX      0x20414956
#define TBAG_SIGNATURE_VIA_EDX      0x20414956
#define TBAG_SIGNATURE_VIA_ECX      0x20414956

#define TBAG_SIGNATURE_VORTEX       "Vortex86 SoC"
#define TBAG_SIGNATURE_VORTEX_EBX   0x74726f56
#define TBAG_SIGNATURE_VORTEX_EDX   0x36387865
#define TBAG_SIGNATURE_VORTEX_ECX   0x436f5320

/**
 * @}
 */

#define TBAG_CPUID_SIGNATURE_LEVEL      0
#define TBAG_CPUID_SERIAL_NUMBER_LEVEL  3

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace system {

namespace __impl {
TBAG_API bool tbag_cpuid_nasm_x86_or_x86_64(unsigned int level, unsigned int * eax, unsigned int * ebx, unsigned int * ecx, unsigned int * edx);
TBAG_API bool tbag_cpuid_gas_x86           (unsigned int level, unsigned int * eax, unsigned int * ebx, unsigned int * ecx, unsigned int * edx);
TBAG_API bool tbag_cpuid_gas_x86_64        (unsigned int level, unsigned int * eax, unsigned int * ebx, unsigned int * ecx, unsigned int * edx);
TBAG_API bool tbag_cpuid                   (unsigned int level, unsigned int * eax, unsigned int * ebx, unsigned int * ecx, unsigned int * edx);
} // namespace __impl

TBAG_API bool getCpuId(uint32_t level,
                       uint32_t * eax,
                       uint32_t * ebx,
                       uint32_t * ecx,
                       uint32_t * edx);
TBAG_API std::string convertRegisterToString(uint32_t value);

TBAG_API std::string getCpuSignature();
TBAG_API std::string getCpuSerialNumber();

} // namespace system

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_SYSTEM_CPUID_HPP__

