/**
 * @file   SimdTest.cpp
 * @brief  Simd class tester.
 * @author zer0
 * @date   2018-01-07
 */

#include <gtest/gtest.h>
#include <libtbag/system/Simd.hpp>

using namespace libtbag;
using namespace libtbag::system;

TEST(SimdTest, Default)
{
    SimdArch arch;
    std::cout << "SIMD INFO: " << arch.toString() << "(" << arch.getFlags() << ")" << std::endl;
}

