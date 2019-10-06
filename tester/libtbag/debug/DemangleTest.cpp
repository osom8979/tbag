/**
 * @file   DemangleTest.cpp
 * @brief  Demangle class tester.
 * @author zer0
 * @date   2017-09-02
 */

#include <gtest/gtest.h>
#include <libtbag/debug/Demangle.hpp>

using namespace libtbag;
using namespace libtbag::debug;

TEST(DemangleTest, Default)
{
    std::string const TEST_NAME = "_ZN3FooC1Ev";
    std::string const RESULT_NAME = "Foo::Foo()";

    std::string const ABI_DEMANGLE  = getAbiDemangle(TEST_NAME.c_str());
    std::string const DEMANGLE_NAME = getDemangle(TEST_NAME.c_str());

    std::cout << "getAbiDemangle(" << TEST_NAME << "): " << ABI_DEMANGLE << std::endl;
    std::cout << "getDemangle(" << TEST_NAME << "): " << DEMANGLE_NAME << std::endl;

    ASSERT_EQ(RESULT_NAME, DEMANGLE_NAME);
}

TEST(DemangleTest, Demangler)
{
    ASSERT_STREQ("f()", getDemangle("_Z1fv").c_str());
    ASSERT_STREQ("f(int)", getDemangle("_Z1fi").c_str());
    ASSERT_STREQ("Foo::Bar()", getDemangle("_ZN3Foo3BarEv").c_str());
    ASSERT_STREQ("operator%(X, X)", getDemangle("_Zrm1XS_").c_str());
    ASSERT_STREQ("Foo::Foo()", getDemangle("_ZN3FooC1Ev").c_str());
}

TEST(DemangleTest, ComplexDemangle)
{
    char const * const ERROR_STRING =
            "_ZNSt3__114__thread_proxyINS_5tupleIJNS_10unique_ptrINS_15__thread_structENS_14default_deleteIS3_EEEEZN23WorkerTest_Default_Test8TestBodyEvE3$_0EEEEEPvSA_+497";

    //std::string const ABI_DEMANGLE  = getAbiDemangle(ERROR_STRING);
    //std::string const DEMANGLE_NAME = getDemangle(ERROR_STRING);
    //std::cout << "ABI DEMANGLE: " << ABI_DEMANGLE << std::endl;
    //std::cout << "DEMANGLE: " << DEMANGLE_NAME << std::endl;
}

