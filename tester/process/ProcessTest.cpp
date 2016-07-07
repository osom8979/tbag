/**
 * @file   ProcessTest.cpp
 * @brief  Process & PipeProcess class tester.
 * @author zer0
 * @date   2016-05-19
 */

#include <gtest/gtest.h>
//#include <libtbag/process/Process.hpp>
//#include <libtbag/process/PipeProcess.hpp>
//#include <libtbag/filesystem/Path.hpp>
//#include <libtbag/predef.hpp>
//
//#include <fstream>
//
//constexpr char const * const getProcessTestFileName() noexcept
//{
//#if __OS_WINDOWS__
//    return "process_test.exe";
//#else
//    return "process_test";
//#endif
//}
//
//using namespace libtbag;
//using namespace libtbag::filesystem;
//using namespace libtbag::process;
//
//static bool runTestProgram(Process           * process
//                         , std::string const & arg1
//                         , std::string const & arg2)
//{
//    Path exe_dir      = Path(common::getExeDir());
//    Path test_program = exe_dir / getProcessTestFileName();
//
//    std::vector<std::string> args;
//    args.push_back(arg1);
//    if (arg2.empty() == false) {
//        args.push_back(arg2);
//    }
//
//    process->setExecuteFile(test_program);
//    process->setArguments(args);
//    process->setWorkingDirectory(exe_dir);
//    process->setFlags(0);
//
//    return process->exe();
//}
//
//TEST(PipeProcessTest, StandardOutput)
//{
//    std::string const  TEST_STRING = "TEST_OUTPUT_MESSAGE";
//    std::string const EMPTY_STRING = "";
//
//    PipeProcess process;
//    ASSERT_TRUE(runTestProgram(&process, "out", TEST_STRING));
//
//    ASSERT_EQ(process.getExitStatus(), 0);
//    ASSERT_EQ(process.getTerminateSignal(), 0);
//    ASSERT_EQ(process.getStandardOutput(), TEST_STRING);
//    ASSERT_EQ(process.getStandardError(), EMPTY_STRING);
//}
//
//TEST(PipeProcessTest, StandardInput)
//{
//    std::string const  TEST_STRING = "TEST_INPUT";
//    std::string const INPUT_STRING = TEST_STRING + "\n";
//    std::string const EMPTY_STRING = "";
//
//    PipeProcess process;
//    process.setStandardInput(INPUT_STRING);
//    ASSERT_TRUE(runTestProgram(&process, "out", EMPTY_STRING));
//
//    ASSERT_EQ(process.getExitStatus(), 0);
//    ASSERT_EQ(process.getTerminateSignal(), 0);
//    ASSERT_EQ(process.getStandardOutput(), TEST_STRING);
//    ASSERT_EQ(process.getStandardError(), EMPTY_STRING);
//}
//
//TEST(PipeProcessTest, StandardError)
//{
//    std::string const  TEST_STRING = "TEST_ERROR_MESSAGE";
//    std::string const EMPTY_STRING = "";
//
//    PipeProcess process;
//    ASSERT_TRUE(runTestProgram(&process, "err", TEST_STRING));
//
//    ASSERT_EQ(process.getExitStatus(), 0);
//    ASSERT_EQ(process.getTerminateSignal(), 0);
//    ASSERT_EQ(process.getStandardOutput(), EMPTY_STRING);
//    ASSERT_EQ(process.getStandardError(),   TEST_STRING);
//}
//
//TEST(PipeProcessTest, FileOutput)
//{
//    filesystem::Path const TEST_FILE("__process_test_file.txt");
//    if (filesystem::common::existsFile(TEST_FILE)) {
//        filesystem::common::remove(TEST_FILE);
//    }
//    ASSERT_FALSE(filesystem::common::existsFile(TEST_FILE));
//
//    std::string const  TEST_STRING = "TEST";
//    std::string const EMPTY_STRING = "";
//
//    PipeProcess process;
//    ASSERT_TRUE(runTestProgram(&process, "file", TEST_STRING));
//
//    ASSERT_EQ(process.getExitStatus(), 0);
//    ASSERT_EQ(process.getTerminateSignal(), 0);
//    ASSERT_EQ(process.getStandardOutput(), EMPTY_STRING);
//    ASSERT_EQ(process.getStandardError(),  EMPTY_STRING);
//
//    ASSERT_TRUE(filesystem::common::existsFile(TEST_FILE));
//    ASSERT_TRUE(filesystem::common::isRegularFile(TEST_FILE));
//
//    std::ifstream file(TEST_FILE.getString());
//    std::string   file_body;
//    file >> file_body;
//    file.close();
//
//    ASSERT_EQ(file_body, TEST_STRING);
//}
//
//TEST(ProcessTest, exit_code_failure)
//{
//    Process process;
//    ASSERT_TRUE(runTestProgram(&process, "unknown", "unknown"));
//
//    ASSERT_EQ(process.getExitStatus(), 1);
//    ASSERT_EQ(process.getTerminateSignal(), 0);
//}

