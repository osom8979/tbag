/**
 * @file   ProcessManagerTest.cpp
 * @brief  ProcessManager class tester.
 * @author zer0
 * @date   2017-07-29
 */

#include <gtest/gtest.h>
#include <libtbag/log/Log.hpp>
#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/process/ProcessManager.hpp>
#include <libtbag/filesystem/Path.hpp>

using namespace libtbag;
using namespace libtbag::process;
using namespace libtbag::uvpp;

TEST(ProcessManagerTest, Default)
{
    log::SeverityGuard guard;

    std::string const EXE_NAME = "tbproc";
    std::string const TEST_OUT = "test_output_string";

    using Path = filesystem::Path;
    Path const PATH = Path::getExeDir() / filesystem::getExecutableName(EXE_NAME);

    int const TEST_COUNT = 10;
    int on_read  = 0;
    int on_error = 0;
    int on_exit  = 0;

    ProcessManager pm;
    pm.out_read_cb = [&](int pid, char const * buffer, std::size_t size){
        ++on_read;
    };
    pm.err_read_cb = [&](int pid, char const * buffer, std::size_t size){
        ++on_error;
    };
    pm.exit_cb = [&](int pid, int64_t exit_status, int term_signal){
        ++on_exit;
    };

    for (int i = 0; i < TEST_COUNT; ++i) {
        ASSERT_NE(0, pm.exec(PATH.toString(), {"out"}, {}, std::string(), TEST_OUT));
    }
    pm.join();

    ASSERT_EQ(TEST_COUNT, pm.size());
    ASSERT_FALSE(pm.empty());
    auto list = pm.list();
    ASSERT_EQ(TEST_COUNT, list.size());
    for (auto & proc : list) {
        auto shared = pm.get(proc.pid).lock();
        ASSERT_EQ(0, shared->getExitStatus());
        ASSERT_EQ(0, shared->getTermSignal());
    }
    ASSERT_EQ(TEST_COUNT, on_read);
    ASSERT_EQ(0, on_error);
    ASSERT_EQ(TEST_COUNT, on_exit);
}

