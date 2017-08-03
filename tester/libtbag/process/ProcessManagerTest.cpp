/**
 * @file   ProcessManagerTest.cpp
 * @brief  ProcessManager class tester.
 * @author zer0
 * @date   2017-07-29
 */

#include <gtest/gtest.h>
#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/process/ProcessManager.hpp>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/filesystem/File.hpp>

using namespace libtbag;
using namespace libtbag::process;
using namespace libtbag::uvpp;

TEST(ProcessManagerTest, Default)
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
    ProcessManager mgr;
    Process::Options options;
    options.setCurrentWorking();
    options.setFile(TBPROC_EXE);
    options.appendArgument("file");
    options.appendArgument(WRITE_BODY);
    int pid = mgr.exec(loop, options);
    ASSERT_LT(0, pid);

    auto shared = mgr.get(pid).lock();
    ASSERT_TRUE(static_cast<bool>(shared));

    ASSERT_EQ(Err::E_SUCCESS, loop.run());
    ASSERT_TRUE(shared->isExit());
    ASSERT_EQ(0, shared->getExitStatus());
    ASSERT_EQ(0, shared->getTermSignal());
    ASSERT_TRUE(TBPROC_TXT.exists());

    std::string buffer;
    ASSERT_EQ(Err::E_SUCCESS, readFile(TBPROC_TXT, buffer));
    ASSERT_EQ(std::string(WRITE_BODY), buffer);
}

