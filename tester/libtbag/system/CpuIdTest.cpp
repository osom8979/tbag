/**
 * @file   CpuIdTest.cpp
 * @brief  CpuId class tester.
 * @author zer0
 * @date   2017-12-05
 */

#include <gtest/gtest.h>
#include <libtbag/system/CpuId.hpp>

using namespace libtbag;
using namespace libtbag::system;

TEST(CpuIdTest, tbag_cpuid)
{
    unsigned int eax = 0;
    unsigned int ebx = 0;
    unsigned int ecx = 0;
    unsigned int edx = 0;
    __impl::tbag_cpuid(TBAG_CPUID_SIGNATURE_LEVEL, &eax, &ebx, &ecx, &edx);
    std::cout << "CPU Signature: "
              << convertRegisterToString(ebx)
              << convertRegisterToString(edx)
              << convertRegisterToString(ecx)
              << std::endl;
}

TEST(CpuIdTest, Default)
{
    std::cout << "CPU Signature: " << getCpuSignature() << std::endl;
    std::cout << "CPU Serial Number: " << getCpuSerialNumber() << std::endl;
}

