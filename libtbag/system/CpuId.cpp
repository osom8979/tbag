/**
 * @file   CpuId.cpp
 * @brief  CpuId class implementation.
 * @author zer0
 * @date   2017-12-05
 */

#include <libtbag/system/CpuId.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/config-ex.h>

#if defined(HAVE_CPUID_H)
#include <cpuid.h> // GCC, CLANG
#endif
#if defined(HAVE_INTRIN_H)
#include <intrin.h> // MSVC
#endif

#include <cstdint>
#include <sstream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace system {

#if defined(TBAG_COMP_CLANG)
# define __ASM_KEYWORD__ __asm
#else // GCC, ETC...
# define __ASM_KEYWORD__ __asm__
#endif

bool __tbag_cpuid(unsigned int level, unsigned int * eax, unsigned int * ebx, unsigned int * ecx, unsigned int * edx)
{
#if defined(TBAG_ARCH_X86)
    __ASM_KEYWORD__ __volatile__("cpuid" : "=a"(*eax), "=b"(*ebx), "=c"(*ecx), "=d"(*edx) : "0"(level))
    return true;
#elif defined(TBAG_ARCH_X86_64)
    // x86-64 uses %rbx as the base register, so preserve it.
    __ASM_KEYWORD__ __volatile__(
            "xchgq  %%rbx,%q1   \n"
            "cpuid              \n"
            "xchgq  %%rbx,%q1   \n"
            : "=a"(*eax), "=r"(*ebx), "=c"(*ecx), "=d"(*edx)
            : "0"(level));
    return true;
#else
    return false;
#endif
}

std::string getCpuSignature()
{
    uint32_t eax = 0;
    uint32_t ebx = 0;
    uint32_t ecx = 0;
    uint32_t edx = 0;

#if defined(HAVE_CPUID_H)
    if (__get_cpuid(0, &eax, &ebx, &ecx, &edx) == false) {
        return std::string();
    }
#elif defined(TBAG_COMP_MSVC) && defined(HAVE_INTRIN_H) && (defined(TBAG_ARCH_X86) || defined(TBAG_ARCH_X86_64))
    int cpuInfo[4] = {0,};
    __cpuid(cpuInfo, 0);
    eax = (uint32_t)cpuInfo[0];
    ebx = (uint32_t)cpuInfo[1];
    ecx = (uint32_t)cpuInfo[2];
    edx = (uint32_t)cpuInfo[3];
#else
    unsigned int __eax = 0;
    unsigned int __ebx = 0;
    unsigned int __ecx = 0;
    unsigned int __edx = 0;
    if (__tbag_cpuid(0, &__eax, &__ebx, &__ecx, &__edx) == false) {
        return std::string();
    }
    eax = __eax;
    ebx = __ebx;
    ecx = __ecx;
    edx = __edx;
#endif

    std::stringstream ss;
    int i = 0;
    for (i = 0; i < sizeof(uint32_t); i++) {
        ss << reinterpret_cast<char*>(&ebx)[i];
    }
    for (i = 0; i < sizeof(uint32_t); i++) {
        ss << reinterpret_cast<char*>(&edx)[i];
    }
    for (i = 0; i < sizeof(uint32_t); i++) {
        ss << reinterpret_cast<char*>(&ecx)[i];
    }
    return ss.str();
}

std::string getCpuSerialNumber()
{
    uint32_t eax = 0;
    uint32_t ebx = 0;
    uint32_t ecx = 0;
    uint32_t edx = 0;

#if defined(TBAG_ARCH_X86) || defined(TBAG_ARCH_X86_64)
# if defined(HAVE_CPUID_H)
    if (__get_cpuid(3, &eax, &ebx, &ecx, &edx) == false) {
        return std::string();
    }
# elif defined(TBAG_COMP_MSVC) && defined(HAVE_INTRIN_H)
    int cpuInfo[4] = {0,};
    __cpuid(cpuInfo, 0);
    eax = (uint32_t)cpuInfo[0];
    ebx = (uint32_t)cpuInfo[1];
    ecx = (uint32_t)cpuInfo[2];
    edx = (uint32_t)cpuInfo[3];
# else
    unsigned int __eax = 0;
    unsigned int __ebx = 0;
    unsigned int __ecx = 0;
    unsigned int __edx = 0;
    if (__tbag_cpuid(0, &__eax, &__ebx, &__ecx, &__edx) == false) {
        return std::string();
    }
    eax = __eax;
    ebx = __ebx;
    ecx = __ecx;
    edx = __edx;
# endif
#else  // defined(TBAG_ARCH_X86) || defined(TBAG_ARCH_X86_64)
    return std::string();
#endif // defined(TBAG_ARCH_X86) || defined(TBAG_ARCH_X86_64)

    std::stringstream ss;
    int i = 0;
    for (i = 0; i < sizeof(uint32_t); i++) {
        ss << reinterpret_cast<char*>(&edx)[i];
    }
    for (i = 0; i < sizeof(uint32_t); i++) {
        ss << reinterpret_cast<char*>(&ecx)[i];
    }
    return ss.str();
}

} // namespace system

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

