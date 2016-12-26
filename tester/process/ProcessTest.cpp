/**
 * @file   ProcessTest.cpp
 * @brief  Process class tester.
 * @author zer0
 * @date   2016-11-27
 */

#include <gtest/gtest.h>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/process/Process.hpp>

#include <csignal>

using namespace libtbag;
using namespace libtbag::process;

static char const * const EXE_NAME = "tbproc";

TEST(ProcessTest, Default)
{
    using Path = libtbag::filesystem::Path;
    Path const EXE_PATH = Path::getExeDir() / getExecutableName(EXE_NAME);

    Process process;
    ASSERT_TRUE(process.exe(EXE_PATH, Path::getExeDir()));
    ASSERT_EQ(1, process.getExitStatus());
    ASSERT_EQ(0, process.getTerminateSignal());
}

TEST(ProcessTest, asyncExe)
{
    using Path = libtbag::filesystem::Path;
    Path const EXE_PATH = Path::getExeDir() / getExecutableName(EXE_NAME);

    Process process;
    Process::Param param;
    param.setExePath(EXE_PATH);
    param.setWorkingDir(Path::getExeDir());

    auto future = Process::asyncExe(process, param);
    // Insert another jobs...
    if (future.valid()) { future.wait(); }
    ASSERT_TRUE(future.get());
}

TEST(ProcessTest, KillSignal)
{
    using Path = libtbag::filesystem::Path;
    Path const EXE_PATH = Path::getExeDir() / getExecutableName(EXE_NAME);

    Process::Param param;
    param.setExePath(EXE_PATH);
    param.setWorkingDir(Path::getExeDir());
    param.pushArgument("sleep");
    param.setDetached();

    Process process;
    ASSERT_TRUE(process.exe(param));
    ASSERT_LT(0, process.getProcessId());
    process.kill(SIGTERM);

#if defined(__PLATFORM_UNIX_LIKE__)
    {   // Check command:
        //  ./tester --gtest_filter=ProcessTest* &&  ps -e | grep tbproc
    }
#endif

    //EXPECT_EQ(SIGKILL, process.getExitStatus());
    //EXPECT_EQ(SIGKILL, process.getTerminateSignal());
}

