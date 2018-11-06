/**
 * @file   SharedLibraryTest.cpp
 * @brief  SharedLibrary class tester.
 * @author zer0
 * @date   2018-11-06
 */

#include <gtest/gtest.h>
#include <libtbag/lib/SharedLibrary.hpp>
#include <libtbag/filesystem/Path.hpp>

using namespace libtbag;
using namespace libtbag::lib;

TEST(SharedLibraryTest, Default)
{
    std::string const LIBRARY_NAME = "tbshare";
    std::string const FUNC1_NAME   = "tbshare_add";
    std::string const FUNC2_NAME   = "tbshare_add2";

    using namespace libtbag::filesystem;
    auto const PATH = Path::getExeDir() / getModuleName(LIBRARY_NAME);

    SharedLibrary lib;
    ASSERT_TRUE(lib.open(PATH.getNativeString()));

    typedef int (*AddFunction)(int,int);
    AddFunction func = (AddFunction) lib.symbol(FUNC1_NAME);
    ASSERT_TRUE(func != nullptr);
    ASSERT_EQ(  2, (*func)(  1,   1));
    ASSERT_EQ(200, (*func)(100, 100));
    ASSERT_EQ( 33, (*func)( 11,  22));

    ASSERT_EQ(20, lib.call<int>(FUNC1_NAME.c_str(), 10, 10));

    int add2_result = 0;
    lib.call<void>(FUNC2_NAME.c_str(), 40, 15, &add2_result);
    ASSERT_EQ(55, add2_result);
}

