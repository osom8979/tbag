/**
 * @file   RandomTest.cpp
 * @brief  Random class tester.
 * @author zer0
 * @date   2016-04-07
 */

#include <gtest/gtest.h>
#include <libtbag/predef.hpp>
#include <libtbag/lib/SharedLibrary.hpp>
#include <libtbag/filesystem/Path.hpp>

using namespace libtbag;
using namespace libtbag::lib;

constexpr char const * const getLibraryName() noexcept
{
#if defined(__OS_WINDOWS__)
    return "tbag_test.dll";
#elif defined(__OS_MACOS__)
    return "libtbag_test.dylib";
#else
    return "libtbag_test.so";
#endif
}

TEST(SharedLibraryTest, Default)
{
    char const * const FUNC1_NAME = "__libtbag_test__add__";
    char const * const FUNC2_NAME = "__libtbag_test__add2__";

    filesystem::Path path = filesystem::Path(filesystem::common::getExeDir());
    path /= getLibraryName();

    SharedLibrary lib;
    ASSERT_TRUE(lib.open(path.getNativeString()));

    typedef int (*AddFunction)(int,int);
    AddFunction func = (AddFunction) lib.symbol(FUNC1_NAME);
    ASSERT_TRUE(func != nullptr);
    ASSERT_EQ(  2, (*func)(  1,   1));
    ASSERT_EQ(200, (*func)(100, 100));
    ASSERT_EQ( 33, (*func)( 11,  22));

    ASSERT_EQ(20, lib.call<int>(FUNC1_NAME, 10, 10));

    int add2_result = 0;
    lib.callVoid(FUNC2_NAME, 40, 15, &add2_result);
    ASSERT_EQ(55, add2_result);
}

