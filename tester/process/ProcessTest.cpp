/**
 * @file   ProcessTest.cpp
 * @brief  Process & PipeProcess class tester.
 * @author zer0
 * @date   2016-05-19
 */

#include <gtest/gtest.h>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/process/Process.hpp>

using namespace libtbag;
using namespace libtbag::process;

static char const * const EXE_NAME = "tbproc";
using Path = libtbag::filesystem::Path;

TEST(ProcessTest, ExitCodeFailure)
{
    Path const EXE_PATH = Path::getExeDir() / getExecutableName(EXE_NAME);

    Process process;
    ASSERT_TRUE(process.exe(EXE_PATH, Path::getExeDir()));
    ASSERT_EQ(1, process.getExitStatus());
    ASSERT_EQ(0, process.getTerminateSignal());
}

TEST(ProcessTest, StandardOutput)
{
    Path const EXE_PATH = Path::getExeDir() / getExecutableName(EXE_NAME);

    std::string const  TEST_STRING = "TEST_OUTPUT_MESSAGE";
    std::string const EMPTY_STRING = "";

    std::string out_result;
    std::string err_result;

    Process process;
    Process::Param param;
    param.setExePath(EXE_PATH);
    param.setWorkingDir(Path::getExeDir());
    param.pushArggument("out");
    param.pushArggument(TEST_STRING);
    param.setOutReadCallback([&](OutStream::ErrorCode code, char * buffer, std::size_t length){
        if (code == OutStream::ErrorCode::SUCCESS) {
            out_result.assign(buffer, buffer + length);
        }
    });
    param.setErrReadCallback([&](OutStream::ErrorCode code, char * buffer, std::size_t length){
        if (code == OutStream::ErrorCode::SUCCESS) {
            err_result.assign(buffer, buffer + length);
        }
    });

    ASSERT_TRUE(process.exe(param));

    ASSERT_EQ(0, process.getExitStatus());
    ASSERT_EQ(0, process.getTerminateSignal());
    ASSERT_EQ(TEST_STRING, out_result);
    ASSERT_EQ(EMPTY_STRING, err_result);
}

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

