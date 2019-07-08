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

    auto rotate = RotatePath(std::make_shared<MaxSizeWriter>(),
                             std::make_shared<TimeFormatUpdater>(FORMAT));
    rotate.update();
    Path const PATH1 = rotate.path;
    ASSERT_FALSE(PATH1.exists());

    auto writer = std::static_pointer_cast<MaxSizeWriter>(rotate.writer);
    auto const MAX_SIZE = writer->max_size;

    libtbag::util::Buffer const BUFFER(MAX_SIZE-1, 'A');

    ASSERT_EQ(E_SUCCESS, rotate.write(BUFFER.data(), BUFFER.size()));
    Path const PATH2 = rotate.path;
    ASSERT_TRUE(PATH2.exists());
    ASSERT_EQ(PATH1, PATH2);

    ASSERT_EQ(E_SUCCESS, rotate.write("1", 1));
    Path const PATH3 = rotate.path;
    ASSERT_TRUE(PATH3.exists());
    ASSERT_NE(PATH1, PATH3);
}

TEST(RotatePathTest, CreateParams_01)
{
    auto params = RotatePath::createParams("size=2k counter=/prefix/path/log,.log,2");
    ASSERT_TRUE((bool)params.writer);
    ASSERT_TRUE((bool)params.updater);
    ASSERT_FALSE((bool)params.cleaner);

    ASSERT_EQ(2048, ((MaxSizeWriter*)params.writer.get())->max_size);
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
    ASSERT_TRUE((bool)params.writer);
    ASSERT_TRUE((bool)params.updater);
    ASSERT_TRUE((bool)params.cleaner);

    ASSERT_EQ(1024*1024*2, ((MaxSizeWriter*)params.writer.get())->max_size);
    ASSERT_STREQ("$ps.log", ((TimeFormatUpdater*)params.updater.get())->format.c_str());
    ASSERT_STREQ(".zip", ((ArchiveCleaner*)params.cleaner.get())->archive_suffix.c_str());
    ASSERT_FALSE(((ArchiveCleaner*)params.cleaner.get())->remove_source_file);
}

