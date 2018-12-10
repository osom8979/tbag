/**
 * @file   DynamicPythonTest.cpp
 * @brief  DynamicPython class tester.
 * @author zer0
 * @date   2018-10-31
 */

#include <gtest/gtest.h>
#include <libtbag/script/DynamicPython.hpp>
#include <libtbag/log/Log.hpp>

using namespace libtbag;
using namespace libtbag::script;

TEST(DynamicPythonTest, FindLibrary)
{
    libtbag::log::SeverityGuard guard;

    // @formatter:off
    std::cout << "Find python executable: "        << DynamicPython::findPythonExecutable() << std::endl;
    std::cout << "Find python library directory: " << DynamicPython::findPythonLibraryDir() << std::endl;
    std::cout << "Find python library path: "      << DynamicPython::findPythonLibrary()    << std::endl;
    // @formatter:on
}

TEST(DynamicPythonTest, Default)
{
    DynamicPython object;
    ASSERT_TRUE(true);
}

