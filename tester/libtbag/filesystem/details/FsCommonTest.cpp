/**
 * @file   FsCommonTest.cpp
 * @brief  FsCommon class tester.
 * @author zer0
 * @date   2017-03-21
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/filesystem/details/FsCommon.hpp>

#include <string>
#include <iostream>

using namespace libtbag;
using namespace libtbag::filesystem;
using namespace libtbag::filesystem::details;

TEST(FsCommonTest, Utils)
{
    std::cout << "TempDir: " << getTempDir() << std::endl
              << "WorkDir: " << getWorkDir() << std::endl
              << "HomeDir: " << getHomeDir() << std::endl
              << "ExePath: " << getExePath() << std::endl;
}

TEST(FsCommonTest, Create)
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


TEST(FsCommonTest, rw1)
{
    TBAG_TESTER_TEMP_DIR(true, true);

    auto TEST_FILE = TBAG_TESTER_TEMP_DIR_GET() / "test";
    namespace fs = ::libtbag::filesystem::details;

    // Create test file.
    ufile f = fs::open(TEST_FILE);
    ASSERT_LT(0, f);
    ASSERT_TRUE(TEST_FILE.exists());

    std::string BUFFER_TEST1 = "11111";
    std::string BUFFER_TEST2 = "2222222222";

    fs::binf wbinf[2];
    wbinf[0].buffer = &BUFFER_TEST1[0];
    wbinf[0].size   = BUFFER_TEST1.size();
    wbinf[1].buffer = &BUFFER_TEST2[0];
    wbinf[1].size   = BUFFER_TEST2.size();

    int wsize = fs::write(f, wbinf, 2, 0);
    ASSERT_EQ(BUFFER_TEST1.size() + BUFFER_TEST2.size(), wsize);

    std::string rbuf1(BUFFER_TEST1.size(), '\0');
    std::string rbuf2(BUFFER_TEST2.size(), '\0');

    fs::binf rbinf[2];
    rbinf[0].buffer = &rbuf1[0];
    rbinf[0].size   = rbuf1.size();
    rbinf[1].buffer = &rbuf2[0];
    rbinf[1].size   = rbuf2.size();

    int rsize = fs::read(f, rbinf, 2, 0);
    ASSERT_EQ(BUFFER_TEST1.size() + BUFFER_TEST2.size(), rsize);
    ASSERT_EQ(BUFFER_TEST1, rbuf1);
    ASSERT_EQ(BUFFER_TEST2, rbuf2);

    // Close file.
    ASSERT_TRUE(fs::close(f));
}

TEST(FsCommonTest, rw2)
{
    TBAG_TESTER_TEMP_DIR(true, true);

    auto TEST_FILE = TBAG_TESTER_TEMP_DIR_GET() / "test";
    namespace fs = ::libtbag::filesystem::details;

    // Create test file.
    ufile f = fs::open(TEST_FILE);
    ASSERT_LT(0, f);
    ASSERT_TRUE(TEST_FILE.exists());

    std::string BUFFER = "FsIoTest/Default";

    // Write file.
    int wsize = fs::write2(f, BUFFER.c_str(), BUFFER.size(), 0);
    ASSERT_EQ(BUFFER.size(), wsize);

    // Read file.
    std::string rbuf(BUFFER.size(), '\0');
    int rsize = fs::read2(f, &rbuf[0], rbuf.size(), 0);
    ASSERT_EQ(BUFFER.size(), rsize);
    ASSERT_EQ(BUFFER, rbuf);

    // Close file.
    ASSERT_TRUE(fs::close(f));
}


