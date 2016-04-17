/**
 * @file   RandomTest.cpp
 * @brief  Random class tester.
 * @author zer0
 * @date   2016-04-07
 */

#include <gtest/gtest.h>
#include <libtbag/SharedLibrary.hpp>
#include <libtbag/filesystem/Path.hpp>

using namespace libtbag;

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
    filesystem::Path path = filesystem::Path(filesystem::Common::getExeDir());
    path /= getLibraryName();

    SharedLibrary lib;
    ASSERT_TRUE(lib.open(path.getNativeString()));

    typedef int (*AddFunction)(int,int);
    AddFunction func = (AddFunction) lib.symbol("__libtbag_test__add__");
    ASSERT_TRUE(func != nullptr);
    ASSERT_EQ((*func)(1, 1), 2);
    ASSERT_EQ((*func)(100, 100), 200);
    ASSERT_EQ((*func)(11, 22), 33);

    ASSERT_EQ(lib.call<int>("__libtbag_test__add__", 10, 10), 20);

    int add2_result = 0;
    lib.callVoid("__libtbag_test__add2__", 40, 15, &add2_result);
    ASSERT_EQ(add2_result, 55);
}

