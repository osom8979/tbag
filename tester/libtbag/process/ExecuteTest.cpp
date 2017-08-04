/**
 * @file   ExecuteTest.cpp
 * @brief  Execute class tester.
 * @author zer0
 * @date   2017-08-03
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/process/Execute.hpp>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/filesystem/File.hpp>
#include <libtbag/filesystem/FindPath.hpp>
#include <libtbag/filesystem/details/FsCommon.hpp>

using namespace libtbag;
using namespace libtbag::process;

TEST(ExecuteTest, Default)
{
    using namespace libtbag::filesystem;
    auto const TBPROC_EXE = Path().getExeDir() / getExecutableName("tbproc");
    auto const TBPROC_TXT = Path().getExeDir() / "tbproc.txt";

    char const * const WRITE_BODY = "EXECUTE_TEST_DEFAULT";

    ASSERT_TRUE(TBPROC_EXE.exists());
    if (TBPROC_TXT.exists()) {
        ASSERT_TRUE(TBPROC_TXT.remove());
    }
    ASSERT_FALSE(TBPROC_TXT.exists());

    Err code = Err::E_UNKNOWN;
    ASSERT_EQ(0, execute(TBPROC_EXE.toString(), {"file", WRITE_BODY}, &code));
    ASSERT_EQ(Err::E_SUCCESS, code);

    ASSERT_TRUE(TBPROC_TXT.exists());
    std::string buffer;
    ASSERT_EQ(Err::E_SUCCESS, readFile(TBPROC_TXT, buffer));
    ASSERT_EQ(std::string(WRITE_BODY), buffer);
    ASSERT_TRUE(TBPROC_TXT.remove());
}

TEST(ExecuteTest, RunShell)
{
    Err code = Err::E_UNKNOWN;
    ASSERT_EQ(0, runShellCommand("echo 'Test echo message'", &code));
    ASSERT_EQ(Err::E_SUCCESS, code);
}

TEST(ExecuteTest, TestLink)
{
    if (isWindowsPlatform()) {
        std::cout << "Skip this test in Windows Platform.\n";
        return;
    }

    TBAG_TEST_TEMP_DIR(true, true);
    auto link_path = TBAG_TEST_TEMP_DIR_GET() / "link";

    using namespace filesystem;
    ASSERT_FALSE(link_path.exists());
    ASSERT_TRUE(filesystem::details::symlink(findDefaultShell().toString(), link_path.toString()));
    ASSERT_TRUE(link_path.exists());

    Err code = Err::E_UNKNOWN;
    ASSERT_EQ(0, execute(link_path, {(isWindowsPlatform() ? "/C" : "-c"), "echo 'Test(Link) echo message'"}, &code));
    ASSERT_EQ(Err::E_SUCCESS, code);
}

