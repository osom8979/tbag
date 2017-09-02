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

