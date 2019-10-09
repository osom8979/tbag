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

TEST(DemangleTest, EnableAbiDemangle)
{
    std::cout << "Enable ABI Demangle: " << enableAbiDemangle() << std::endl;
}

TEST(DemangleTest, Default)
{
    std::string const TEST_NAME = "_ZN3FooC1Ev";
    std::string const RESULT_NAME = "Foo::Foo()";

    std::string const ABI_DEMANGLE  = getAbiDemangle(TEST_NAME.c_str());
    std::string const GTEST_DEMANGLE_NAME = getGtestDemangle(TEST_NAME.c_str());

    std::cout << "getAbiDemangle(" << TEST_NAME << "): " << ABI_DEMANGLE << std::endl;
    std::cout << "getGtestDemangle(" << TEST_NAME << "): " << GTEST_DEMANGLE_NAME << std::endl;

    ASSERT_EQ(RESULT_NAME, GTEST_DEMANGLE_NAME);
}

TEST(DemangleTest, Demangler)
{
    EXPECT_STREQ("f()", getGtestDemangle("_Z1fv").c_str());
    EXPECT_STREQ("f()", getGtestDemangle("_Z1fi").c_str());
    EXPECT_STREQ("Foo::Bar()", getGtestDemangle("_ZN3Foo3BarEv").c_str());
    EXPECT_STREQ("operator%()", getGtestDemangle("_Zrm1XS_").c_str());
    EXPECT_STREQ("Foo::Foo()", getGtestDemangle("_ZN3FooC1Ev").c_str());
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

