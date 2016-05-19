/**
 * @file   ProcessTest.cpp
 * @brief  Process class tester.
 * @author zer0
 * @date   2016-05-19
 */

#include <gtest/gtest.h>
#include <libtbag/process/Process.hpp>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/predef.hpp>

using namespace libtbag;
using namespace libtbag::filesystem;
using namespace libtbag::process;

constexpr char const * const getProcessTestFileName() noexcept
{
#if __OS_WINDOWS__
    return "process_test.exe";
#else
    return "process_test";
#endif
}

static int64_t runTestProgram(std::string const & arg1, std::string const & arg2)
{
    Process process;
    Path exe_dir        = Path(Common::getExeDir());
    Path test_program   = exe_dir / getProcessTestFileName();

    std::vector<std::string> args;
    args.push_back(arg1);
    args.push_back(arg2);

    process.exe(test_program, args, exe_dir);
    return process.getExitStatus();
}

TEST(ProcessTest, exit_code_success)
{
    ASSERT_EQ(runTestProgram("out", "test"), 0);
}

TEST(ProcessTest, file_test)
{
    ASSERT_EQ(runTestProgram("file", "content\n"), 0);
}

TEST(ProcessTest, exit_code_failure)
{
    ASSERT_EQ(runTestProgram("unknown", "unknown"), 1);
}

