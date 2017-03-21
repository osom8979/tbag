/**
 * @file   FsIoTest.cpp
 * @brief  FsIo class tester.
 * @author zer0
 * @date   2017-03-19
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/filesystem/details/FsIo.hpp>

using namespace libtbag;
using namespace libtbag::filesystem;
using namespace libtbag::filesystem::details;

TEST(FsIoTest, rw1)
{
    TBAG_CREATE_TESTER_TEMP_DIR;

    auto TEST_FILE = TBAG_GET_TESTER_TEMP_DIR / "test.txt";
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

TEST(FsIoTest, rw2)
{
    TBAG_CREATE_TESTER_TEMP_DIR;

    auto TEST_FILE = TBAG_GET_TESTER_TEMP_DIR / "test.txt";
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

