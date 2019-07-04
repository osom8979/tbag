/**
 * @file   FileSinkTest.cpp
 * @brief  FileSink class tester.
 * @author zer0
 * @date   2019-07-03
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/log/sink/FileSink.hpp>
#include <libtbag/filesystem/File.hpp>

#include <string>

using namespace libtbag;
using namespace libtbag::log;
using namespace libtbag::log::sink;

TEST(FileSinkTest, Default)
{
    char const * const TEST_FILENAME = "test.file";
    tttDir_Automatic();
    auto const PATH = tttDir_Get() / TEST_FILENAME;

    COMMENT("SINK RAII") {
        FileSink sink(PATH);
        ASSERT_TRUE(sink.write("abcdefg", 4));
        sink.flush();
    }

    ASSERT_TRUE(PATH.isRegularFile());
    std::string buffer;
    ASSERT_EQ(E_SUCCESS, libtbag::filesystem::readFile(PATH, buffer));
    ASSERT_STREQ("abcd", buffer.c_str());
}

