/**
 * @file   ProcessTest.cpp
 * @brief  Process class tester.
 * @author zer0
 * @date   2017-07-30
 */

#include <gtest/gtest.h>
#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/uvpp/func/FunctionalProcess.hpp>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/filesystem/File.hpp>

using namespace libtbag;
using namespace libtbag::uvpp;
using namespace libtbag::uvpp::func;

TEST(ProcessTest, Default)
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

    Loop loop;
    Process::Options options;
    options.setCurrentWorking();
    options.setFile(TBPROC_EXE);
    options.appendArgument("file");
    options.appendArgument(WRITE_BODY);
    auto proc = loop.newHandle<FuncProcess>(loop, options);
    proc->exit_cb = [&](int64_t exit_status, int term_signal){
        result_exit_status = exit_status;
        result_term_signal = term_signal;
        proc->close();
    };

    ASSERT_EQ(Err::E_SUCCESS, loop.run());
    ASSERT_EQ(0, result_exit_status);
    ASSERT_EQ(0, result_term_signal);
    ASSERT_TRUE(TBPROC_TXT.exists());

    std::string buffer;
    ASSERT_EQ(Err::E_SUCCESS, readFile(TBPROC_TXT, buffer));
    ASSERT_EQ(std::string(WRITE_BODY), buffer);
    ASSERT_TRUE(TBPROC_TXT.remove());
}

