/**
 * @file   UxProcessTest.cpp
 * @brief  UxProcess class tester.
 * @author zer0
 * @date   2018-12-07
 */

#include <gtest/gtest.h>
#include <libtbag/uvxx/UxLoop.hpp>
#include <libtbag/uvxx/UxProcess.hpp>

#include <libtbag/filesystem/Path.hpp>
#include <libtbag/filesystem/File.hpp>

#include <string>

using namespace libtbag;
using namespace libtbag::uvxx;

TEST(UxProcessTest, Default)
{
    using namespace libtbag::filesystem;
    auto const TBPROC_EXE = Path().getExeDir() / getExecutableName("tbproc");
    auto const TBPROC_TXT = Path().getExeDir() / "tbproc.txt";

    char const * const WRITE_BODY = "TEMP_BODY";

    ASSERT_TRUE(TBPROC_EXE.exists());
    if (TBPROC_TXT.exists()) {
        ASSERT_TRUE(TBPROC_TXT.remove());
    }
    ASSERT_FALSE(TBPROC_TXT.exists());

    int64_t result_exit_status = 0;
    int result_term_signal = 0;

    UxProcess proc;
    ASSERT_FALSE(proc.isInit());

    UxLoop loop;
    ASSERT_TRUE(loop.empty());

    UxProcess::Options options1;
    options1.setCurrentWorking();
    options1.setFile(TBPROC_EXE);

    ASSERT_EQ(E_SUCCESS, proc.spawn(loop, options1));
    ASSERT_TRUE(proc.isInit());
    ASSERT_FALSE(loop.empty());
    ASSERT_EQ(1, loop.size());

    UxProcess::Options options2;
    options2.setCurrentWorking();
    options2.setFile(TBPROC_EXE);
    options2.appendArgument("file");
    options2.appendArgument(WRITE_BODY);

    ASSERT_EQ(E_SUCCESS, proc.spawn(loop, options2));
    ASSERT_TRUE(proc.isInit());
    ASSERT_FALSE(loop.empty());
    ASSERT_EQ(2, loop.size());

    proc.setOnExit([&](int64_t exit_status, int term_signal){
        result_exit_status = exit_status;
        result_term_signal = term_signal;
        proc.close();
    });

    ASSERT_EQ(E_SUCCESS, loop.run());
    ASSERT_TRUE(loop.empty());

    ASSERT_EQ(0, result_exit_status);
    ASSERT_EQ(0, result_term_signal);
    ASSERT_TRUE(TBPROC_TXT.exists());

    std::string buffer;
    ASSERT_EQ(E_SUCCESS, readFile(TBPROC_TXT, buffer));
    ASSERT_EQ(std::string(WRITE_BODY), buffer);
    ASSERT_TRUE(TBPROC_TXT.remove());
}

