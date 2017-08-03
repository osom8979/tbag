/**
 * @file   ExecuteTest.cpp
 * @brief  Execute class tester.
 * @author zer0
 * @date   2017-08-03
 */

#include <gtest/gtest.h>
#include <libtbag/process/Execute.hpp>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/filesystem/File.hpp>

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
    ASSERT_EQ(0, runShell("echo 'haha'", &code));
    ASSERT_EQ(Err::E_SUCCESS, code);
}

