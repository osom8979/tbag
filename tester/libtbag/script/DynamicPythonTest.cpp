/**
 * @file   DynamicPythonTest.cpp
 * @brief  DynamicPython class tester.
 * @author zer0
 * @date   2018-10-31
 */

#include <gtest/gtest.h>
#include <libtbag/script/DynamicPython.hpp>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/log/Log.hpp>

//#define _PRINT_NUMPY_VERSION

using namespace libtbag;
using namespace libtbag::script;

TEST(DynamicPythonTest, FindLibrary)
{
    libtbag::log::SeverityGuard guard;

    // clang-format off
    std::cout << "Find python executable: "        << DynamicPython::findPythonExecutable() << std::endl;
    std::cout << "Find python library directory: " << DynamicPython::findPythonLibraryDir() << std::endl;
    std::cout << "Find python library path: "      << DynamicPython::findPythonLibrary()    << std::endl;
    // clang-format on
}

TEST(DynamicPythonTest, Default)
{
    libtbag::log::SeverityGuard guard;

    using namespace libtbag::filesystem;
    Path const PYTHON_LIBRARY_PATH(DynamicPython::findPythonLibrary());

    if (!PYTHON_LIBRARY_PATH.exists()) {
        std::cout << "Not found python library. Skip this test.\n";
        return;
    }
    if (!PYTHON_LIBRARY_PATH.isReadable()) {
        std::cout << "Not readable python library. Skip this test.\n";
        return;
    }

    DynamicPython py;
    if (!py.open(PYTHON_LIBRARY_PATH.toString())) {
        std::cout << "Python library open failed. Skip this test.\n";
        return;
    }

    py.Initialize();
    py.Run_SimpleString("import sys; print(sys.path)");
#if defined(_PRINT_NUMPY_VERSION)
    py.Run_SimpleString("import numpy; print(numpy.version.version)");
#endif
    py.Finalize();
}

