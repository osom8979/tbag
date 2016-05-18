/**
 * @file   ProcessManagerTest.cpp
 * @brief  ProcessManager class tester.
 * @author zer0
 * @date   2016-05-17
 */

#include <gtest/gtest.h>
#include <libtbag/process/ProcessManager.hpp>
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

static ProcessResult runTestProgram(std::string const & arg1, std::string const & arg2)
{
    ProcessManager * pm = ProcessManager::getInstance();
    Path exe_dir        = Path(Common::getExeDir());
    Path test_program   = exe_dir / getProcessTestFileName();

    std::vector<std::string> args;
    args.push_back(arg1);
    args.push_back(arg2);

    return pm->run(test_program, args, exe_dir);
}

TEST(ProcessManagerTest, exit_code_success)
{
    auto result = runTestProgram("out", "test");
    ASSERT_EQ(result.exit_status, 0);
    ASSERT_EQ(result.terminate_signal, 0);
}

TEST(ProcessManagerTest, file_test)
{
    auto result = runTestProgram("file", "content\n");
    ASSERT_EQ(result.exit_status, 0);
    ASSERT_EQ(result.terminate_signal, 0);
}

TEST(ProcessManagerTest, exit_code_failure)
{
    auto result = runTestProgram("unknown", "unknown");
    ASSERT_EQ(result.exit_status, 1);
    ASSERT_EQ(result.terminate_signal, 0);
}

