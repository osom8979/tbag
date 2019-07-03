/**
 * @file   FileSinkTest.cpp
 * @brief  FileSink class tester.
 * @author zer0
 * @date   2019-07-03
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/log/sink/FileSink.hpp>

using namespace libtbag;
using namespace libtbag::log;
using namespace libtbag::log::sink;

TEST(FileSinkTest, Default)
{
    char const * const TEST_FILENAME = "test.file";
    tttDir_Automatic();
    auto const PATH = tttDir_Get() / TEST_FILENAME;

    FileSink object(PATH);
}

