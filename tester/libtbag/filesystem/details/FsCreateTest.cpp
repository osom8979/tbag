/**
 * @file   FsCreateTest.cpp
 * @brief  FsCreate class tester.
 * @author zer0
 * @date   2017-03-21
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/filesystem/details/FsCreate.hpp>
#include <libtbag/filesystem/details/FsAttribute.hpp>
#include <libtbag/filesystem/details/FsRemove.hpp>

#include <iostream>

using namespace libtbag;
using namespace libtbag::filesystem;
using namespace libtbag::filesystem::details;

TEST(FsCreateTest, createDirectory)
{
    TBAG_TESTER_TEMP_DIR(true, true);

    auto const TEST_DIR = TBAG_TESTER_TEMP_DIR_GET() / std::string("test");
    namespace fs = ::libtbag::filesystem::details;

    int const MIN_MODE = 0000;
    int const MAX_MODE = 0777;
    int const PREV_MASK = setUserMask(0);

    for (int mode = MIN_MODE; mode <= MAX_MODE; ++mode) {
        auto path = TEST_DIR.getString() + std::string("-") + std::to_string(mode);
        ASSERT_TRUE(createDirectoryEx(path, mode));
        ASSERT_EQ(mode, static_cast<int>(getMode(path) & MAX_MODE));

        ASSERT_TRUE(setMode(path, 0700));
        ASSERT_TRUE(removeDirectory(path));
    }

    setUserMask(PREV_MASK);
}


