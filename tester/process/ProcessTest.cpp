/**
 * @file   ProcessTest.cpp
 * @brief  Process class tester.
 * @author zer0
 * @date   2016-11-27
 */

#include <gtest/gtest.h>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/process/Process.hpp>

using namespace libtbag;
using namespace libtbag::process;

static char const * const EXE_NAME = "tbproc";
using Path = libtbag::filesystem::Path;

TEST(ProcessTest, Default)
{
    Path const EXE_PATH = Path::getExeDir() / getExecutableName(EXE_NAME);

    Process process;
    //ASSERT_TRUE(process.exe(EXE_PATH, Path::getExeDir()));
    //ASSERT_EQ(1, process.getExitStatus());
    //ASSERT_EQ(0, process.getTerminateSignal());
}

