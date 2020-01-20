/**
 * @file   StdProcessTest.cpp
 * @brief  StdProcess class tester.
 * @author zer0
 * @date   2017-09-05
 */

#include <gtest/gtest.h>
#include <libtbag/process/StdProcess.hpp>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/uvpp/Loop.hpp>

using namespace libtbag;
using namespace libtbag::process;

TEST(StdProcessTest, InAndOut)
{
    std::string const EXE_NAME = "tbproc";
    std::string const TEST_OUT = "test_output_string";

    using Path = filesystem::Path;
    Path const PATH = Path::getExeDir() / filesystem::getExecutableName(EXE_NAME);

    std::string output;
    std::string error;

    int on_exit  = 0;
    int on_close = 0;

    StdProcess proc;
    proc.out_read_cb = [&](char const * buffer, std::size_t size){
        output.assign(buffer, buffer + size);
    };
    proc.err_read_cb = [&](char const * buffer, std::size_t size){
        error.assign(buffer, buffer + size);
    };
    proc.exit_cb = [&](int64_t exit_status, int term_signal){
        ++on_exit;
    };
    proc.close_cb = [&](){
        ++on_close;
    };

    uvpp::Loop loop;
    ASSERT_EQ(E_SUCCESS, proc.spawn(loop, PATH.toString(), {"out"}, {}, std::string(), TEST_OUT));
    ASSERT_EQ(E_SUCCESS, loop.run());

    ASSERT_EQ(TEST_OUT, output);
    ASSERT_TRUE(error.empty());
    ASSERT_EQ(1, on_exit);
    ASSERT_EQ(1, on_close);
}

TEST(StdProcessTest, SilentOut)
{
    std::string const EXE_NAME = "tbproc";
    std::string const TEST_OUT = "silent_message";

    using Path = filesystem::Path;
    Path const PATH = Path::getExeDir() / filesystem::getExecutableName(EXE_NAME);

    std::string output;

    int on_exit  = 0;
    int on_close = 0;

    StdProcess proc;
    proc.out_read_cb = [&](char const * buffer, std::size_t size){
        output.assign(buffer, buffer + size);
    };
    proc.exit_cb = [&](int64_t exit_status, int term_signal){
        ++on_exit;
    };
    proc.close_cb = [&](){
        ++on_close;
    };

    uvpp::Loop loop;
    ASSERT_EQ(E_SUCCESS, proc.spawn(loop, PATH.toString(), {"out"}, {}, std::string(), TEST_OUT, false));
    ASSERT_EQ(E_SUCCESS, loop.run());

    ASSERT_TRUE(output.empty());
    ASSERT_EQ(1, on_exit);
    ASSERT_EQ(1, on_close);
}

TEST(StdProcessTest, Subprocess)
{
    std::string const EXE_NAME = "tbproc";
    std::string const TEST_OUT = "test_output_subprocess";

    using Path = filesystem::Path;
    Path const PATH = Path::getExeDir() / filesystem::getExecutableName(EXE_NAME);

    std::string output;
    std::string error;

    int64_t exit = -1;
    int term = -1;

    auto const CODE = subprocess(PATH.toString(), {"out"}, {}, std::string(), TEST_OUT, &exit, &term, &output, &error);
    ASSERT_EQ(E_SUCCESS, CODE);
    ASSERT_EQ(0, exit);
    ASSERT_EQ(0, term);

    ASSERT_EQ(TEST_OUT, output);
    ASSERT_TRUE(error.empty());
}

