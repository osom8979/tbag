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
    tttDir_Automatic();
    auto const FILE_PATH = tttDir_Get() / "test.log";
    auto const FORMAT = TimeFormatUpdater::getDefaultTimeFormatString(FILE_PATH);

    auto rotate = RotatePath(std::make_shared<SizeChecker>(),
                             std::make_shared<TimeFormatUpdater>(FORMAT));
    ASSERT_TRUE(rotate.update());

    auto checker = std::static_pointer_cast<SizeChecker>(rotate.checker);
    util::Buffer const BUFFER(checker->max_size, '\0');

    auto path1 = rotate.path;
    ASSERT_FALSE(path1.exists());
    ASSERT_FALSE(rotate.next());

    ASSERT_EQ(E_SUCCESS, writeFile(path1, BUFFER));
    ASSERT_TRUE(path1.exists());
    ASSERT_FALSE(rotate.next());
    ASSERT_TRUE(rotate.next(nullptr, 1));

    auto path2 = rotate.path;
    ASSERT_FALSE(path2.exists());
    ASSERT_NE(path1, path2);
}

TEST(RotatePathTest, CreateParams_01)
{
    auto params = RotatePath::createParams("size=2k counter=/prefix/path/log,.log,2");
    ASSERT_TRUE((bool)params.checker);
    ASSERT_TRUE((bool)params.updater);
    ASSERT_FALSE((bool)params.cleaner);

    ASSERT_EQ(2048, ((SizeChecker*)params.checker.get())->max_size);
    ASSERT_STREQ("/prefix/path/log", ((CounterUpdater*)params.updater.get())->prefix.c_str());
    ASSERT_STREQ(".log", ((CounterUpdater*)params.updater.get())->suffix.c_str());
    ASSERT_EQ(2, ((CounterUpdater*)params.updater.get())->count);
}

TEST(RotatePathTest, CreateParams_02)
{
    RotatePath::Environments envs;
    envs.push("SIZE", "2m");
    envs.push("FORMAT", "$ps.log");

    auto params = RotatePath::createParams("size=${SIZE} time=${FORMAT} archive=.zip,false", envs);
    ASSERT_TRUE((bool)params.checker);
    ASSERT_TRUE((bool)params.updater);
    ASSERT_TRUE((bool)params.cleaner);

    ASSERT_EQ(1024*1024*2, ((SizeChecker*)params.checker.get())->max_size);
    ASSERT_STREQ("$ps.log", ((TimeFormatUpdater*)params.updater.get())->format.c_str());
    ASSERT_STREQ(".zip", ((ArchiveCleaner*)params.cleaner.get())->archive_suffix.c_str());
    ASSERT_FALSE(((ArchiveCleaner*)params.cleaner.get())->remove_source_file);
}

