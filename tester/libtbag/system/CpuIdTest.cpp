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

TEST(CpuIdTest, __tbag_cpuid)
{
    unsigned int eax = 0;
    unsigned int ebx = 0;
    unsigned int ecx = 0;
    unsigned int edx = 0;

    __tbag_cpuid(0, &eax, &ebx, &ecx, &edx);

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

    std::cout << "CPU Signature: " << ss.str() << std::endl;
}

TEST(CpuIdTest, Default)
{
    std::cout << "CPU Signature: " << getCpuSignature() << std::endl;
    std::cout << "CPU Serial Number: " << getCpuSerialNumber() << std::endl;
}

