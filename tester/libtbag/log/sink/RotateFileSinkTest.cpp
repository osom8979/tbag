/**
 * @file   RotateFileSinkTest.cpp
 * @brief  RotateFileSink class tester.
 * @author zer0
 * @date   2019-07-03
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/log/sink/RotateFileSink.hpp>

using namespace libtbag;
using namespace libtbag::log;
using namespace libtbag::log::sink;

TEST(RotateFileSinkTest, Default)
{
    char const * const TEST_FILENAME = "test";
    tttDir_Automatic();
    auto const PREFIX = tttDir_Get() / TEST_FILENAME;
    auto const ARGUMENTS = std::string("size=1024m counter=") + PREFIX.getGenericString();

    RotateFileSink object(ARGUMENTS);
}

