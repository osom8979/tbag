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

#include <sstream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace system {

// ---------------
namespace __impl {
// ---------------

bool tbag_cpuid_x86(unsigned int level, unsigned int * eax, unsigned int * ebx, unsigned int * ecx, unsigned int * edx)
{
#if defined(TBAG_ARCH_X86) && (defined(TBAG_COMP_CLANG) || defined(TBAG_COMP_GCC))
    TBAG_ASM_VOLATILE("cpuid" : "=a"(*eax), "=b"(*ebx), "=c"(*ecx), "=d"(*edx) : "0"(level))
    return true;
#else
    return false;
#endif
}

bool tbag_cpuid_x86_64(unsigned int level, unsigned int * eax, unsigned int * ebx, unsigned int * ecx, unsigned int * edx)
{
#if defined(TBAG_ARCH_X86_64) && (defined(TBAG_COMP_CLANG) || defined(TBAG_COMP_GCC))
    // x86-64 uses %rbx as the base register, so preserve it.
    TBAG_ASM_VOLATILE(
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

bool tbag_cpuid(unsigned int level, unsigned int * eax, unsigned int * ebx, unsigned int * ecx, unsigned int * edx)
{
#if defined(TBAG_ARCH_X86)
    return tbag_cpuid_x86(level, eax, ebx, ecx, edx);
#elif defined(TBAG_ARCH_X86_64)
    return tbag_cpuid_x86_64(level, eax, ebx, ecx, edx);
#else
    return false;
#endif
}

// ------------------
} // namespace __impl
// ------------------

bool getCpuId(uint32_t level, uint32_t * eax, uint32_t * ebx, uint32_t * ecx, uint32_t * edx)
{
#if defined(HAVE_CPUID_H)
    if (__get_cpuid(level, eax, ebx, ecx, edx) == false) {
        return false;
    }
#elif defined(TBAG_COMP_MSVC) && defined(HAVE_INTRIN_H) && (defined(TBAG_ARCH_X86) || defined(TBAG_ARCH_X86_64))
    int cpu_info[4] = {0,};
    __cpuid(cpuInfo, level);
    *eax = (uint32_t)cpu_info[0];
    *ebx = (uint32_t)cpu_info[1];
    *ecx = (uint32_t)cpu_info[2];
    *edx = (uint32_t)cpu_info[3];
#else
    unsigned int _level = level;
    unsigned int _eax   = 0;
    unsigned int _ebx   = 0;
    unsigned int _ecx   = 0;
    unsigned int _edx   = 0;
    if (__impl::tbag_cpuid(_level, &_eax, &_ebx, &_ecx, &_edx) == false) {
        return false;
    }
    *eax = _eax;
    *ebx = _ebx;
    *ecx = _ecx;
    *edx = _edx;
#endif
    return true;
}

std::string convertRegisterToString(uint32_t value)
{
    std::stringstream ss;
    for (int i = 0; i < sizeof(uint32_t); i++) {
        ss << reinterpret_cast<char*>(&value)[i];
    }
    return ss.str();
}

std::string getCpuSignature()
{
    uint32_t eax = 0;
    uint32_t ebx = 0;
    uint32_t ecx = 0;
    uint32_t edx = 0;
    if (getCpuId(TBAG_CPUID_SIGNATURE_LEVEL, &eax, &ebx, &ecx, &edx)) {
        std::stringstream ss;
        ss << convertRegisterToString(ebx) << convertRegisterToString(edx) << convertRegisterToString(ecx);
        return ss.str();
    }
    return std::string();
}

std::string getCpuSerialNumber()
{
    uint32_t eax = 0;
    uint32_t ebx = 0;
    uint32_t ecx = 0;
    uint32_t edx = 0;
    if (getCpuId(TBAG_CPUID_SERIAL_NUMBER_LEVEL, &eax, &ebx, &ecx, &edx)) {
        std::stringstream ss;
        ss << convertRegisterToString(edx) << convertRegisterToString(ecx);
        return ss.str();
    }
    return std::string();
}

} // namespace system

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

