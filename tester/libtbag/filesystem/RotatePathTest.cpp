/**
 * @file   RotatePathTest.cpp
 * @brief  RotatePath class tester.
 * @author zer0
 * @date   2017-07-31
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/filesystem/RotatePath.hpp>
#include <libtbag/filesystem/File.hpp>
#include <libtbag/util/BufferInfo.hpp>

using namespace libtbag;
using namespace libtbag::filesystem;

TEST(RotatePathTest, Default)
{
    tttDir(true, true);
    auto const file_path = tttDir_Get() / "test.log";

    auto rotate = RotatePath::createDefault(file_path);
    ASSERT_TRUE(rotate.update());

    auto checker = std::static_pointer_cast<RotatePath::SizeChecker>(rotate.getChecker().lock());
    util::Buffer const BUFFER(checker->max_size, '\0');

    auto path1 = rotate.getPath();
    ASSERT_FALSE(path1.exists());
    ASSERT_FALSE(rotate.next());

    ASSERT_EQ(E_SUCCESS, writeFile(path1, BUFFER));
    ASSERT_TRUE(path1.exists());
    ASSERT_FALSE(rotate.next());
    ASSERT_TRUE(rotate.next(nullptr, 1));

    auto path2 = rotate.getPath();
    ASSERT_FALSE(path2.exists());
    ASSERT_NE(path1, path2);
}

