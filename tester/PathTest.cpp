/**
 * @file   PathTest.cpp
 * @brief  Path class tester.
 * @author zer0
 * @date   2016-04-07
 */

#include <gtest/gtest.h>
#include <libtbag/Path.hpp>

using namespace libtbag;

TEST(PathStaticTest, GlobalFunction)
{
# if defined(WIN32) || defined(_WIN32)
    EXPECT_EQ(GetPathSeparator(), '\\');
    EXPECT_EQ(GetPathSplitter(),  ';');
# else
    EXPECT_EQ(GetPathSeparator(), '/');
    EXPECT_EQ(GetPathSplitter(),  ':');
# endif
}

TEST(PathStaticTest, CopyOperators)
{
    Path path1 = Path("TEMP");
    Path path2 = path1;
    Path path3;
    path3 = path1;

    ASSERT_EQ(path2.getString(), path1.getString());
    ASSERT_EQ(path3.getString(), path1.getString());
}

TEST(PathStaticTest, MoveOperators)
{
    auto rvalue_test = []() -> Path {
        return Path("TEMP");
    };

#if defined(__COMP_LLVM__)
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wpessimizing-move"
#endif

    Path path1 = std::move(rvalue_test());
    Path path2;

#if defined(__COMP_LLVM__)
# pragma GCC diagnostic pop
#endif

    path2 = rvalue_test();
    ASSERT_EQ(path1.getString(), path2.getString());
}

