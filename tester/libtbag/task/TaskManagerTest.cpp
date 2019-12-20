/**
 * @file   TaskManagerTest.cpp
 * @brief  TaskManager class tester.
 * @author zer0
 * @date   2019-12-19
 */

#include <gtest/gtest.h>
#include <libtbag/task/TaskManager.hpp>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/signal/SignalHandler.hpp>
#include <libtbag/string/Environments.hpp>

#include <thread>
#include <chrono>

using namespace libtbag;
using namespace libtbag::task;

using ThreadParams = TaskManager::ThreadParams;
using ProcessParams = TaskManager::ProcessParams;

TEST(TaskManagerTest, Default)
{
    TaskManager mgr;
    ASSERT_TRUE(mgr.empty());
    ASSERT_EQ(0u, mgr.size());
    ASSERT_TRUE(mgr.list().empty());
    ASSERT_TRUE(mgr.map().empty());
}

TEST(TaskManagerTest, CreateThread)
{
    TaskManager mgr;
    ASSERT_TRUE(mgr.empty());

    bool test1 = false;
    bool test2 = false;

    ThreadParams params1;
    params1.runner = [&](){
        std::this_thread::sleep_for(std::chrono::seconds(1));
        test1 = true;
    };
    ThreadParams params2;
    params2.runner = [&](){
        std::this_thread::sleep_for(std::chrono::seconds(1));
        test2 = true;
    };

    auto const result1 = mgr.runThread(params1);
    ASSERT_EQ(E_SUCCESS, result1.code);
    auto const result2 = mgr.runThread(params2);
    ASSERT_EQ(E_SUCCESS, result2.code);

    ASSERT_FALSE(mgr.empty());
    ASSERT_EQ(2u, mgr.size());

    auto const task_list = mgr.list();
    ASSERT_EQ(2u, task_list.size());

    auto const join_result1 = mgr.join(task_list[0]);
    ASSERT_EQ(E_SUCCESS, join_result1);
    auto const join_result2 = mgr.join(task_list[1]);
    ASSERT_EQ(E_SUCCESS, join_result2);

    ASSERT_TRUE(test1);
    ASSERT_TRUE(test2);

    ASSERT_FALSE(mgr.empty());
    ASSERT_EQ(2u, mgr.size());

    auto const task_info1 = mgr.getTaskInfo(task_list[0]);
    ASSERT_EQ(E_SUCCESS, task_info1.code);
    auto const task_info2 = mgr.getTaskInfo(task_list[1]);
    ASSERT_EQ(E_SUCCESS, task_info2.code);

    ASSERT_EQ(TaskManager::TaskType::TT_THREAD, task_info1.value.type);
    ASSERT_EQ(TaskManager::TaskType::TT_THREAD, task_info2.value.type);
    ASSERT_TRUE(task_info1.value.done);
    ASSERT_TRUE(task_info2.value.done);

    auto const erase_result1 = mgr.erase(task_list[0]);
    ASSERT_EQ(E_SUCCESS, erase_result1);
    auto const erase_result2 = mgr.erase(task_list[1]);
    ASSERT_EQ(E_SUCCESS, erase_result2);

    ASSERT_TRUE(mgr.empty());
    ASSERT_EQ(0u, mgr.size());
}

TEST(TaskManagerTest, ThreadException)
{
    ThreadParams params;
    params.runner = [&](){
        throw ErrException(E_UNKNOWN);
    };

    TaskManager mgr;
    auto const result = mgr.runThread(params);
    ASSERT_EQ(1u, mgr.size());
    ASSERT_EQ(E_SUCCESS, result.code);

    auto const task_list = mgr.list();
    ASSERT_EQ(1u, task_list.size());
    ASSERT_THROW(mgr.join(task_list[0]), ErrException);
    auto const erase_result = mgr.erase(task_list[0]);
    ASSERT_EQ(E_SUCCESS, erase_result);
    ASSERT_TRUE(mgr.empty());
}

TEST(TaskManagerTest, KillThread)
{
    ThreadParams params;
    params.runner = [&](){
        std::this_thread::sleep_for(std::chrono::minutes(1));
    };

    TaskManager mgr;
    auto const result = mgr.runThread(params);
    ASSERT_EQ(1u, mgr.size());
    ASSERT_EQ(E_SUCCESS, result.code);

    auto const task_id = result.value;
    ASSERT_EQ(E_SUCCESS, mgr.kill(task_id));
    ASSERT_EQ(E_SUCCESS, mgr.join(task_id));

    auto const err_task_info = mgr.getTaskInfo(task_id);
    ASSERT_EQ(E_SUCCESS, err_task_info.code);
    auto const & task_info = err_task_info.value;
    ASSERT_EQ(TaskManager::TaskType::TT_THREAD, task_info.type);
    ASSERT_TRUE(task_info.done);
    ASSERT_EQ(EXIT_FAILURE, task_info.exit_status);
    ASSERT_EQ(libtbag::signal::TBAG_SIGNAL_INTERRUPT, task_info.term_signal);

    auto const erase_result = mgr.erase(task_id);
    ASSERT_EQ(E_SUCCESS, erase_result);

    ASSERT_TRUE(mgr.empty());
    ASSERT_EQ(0u, mgr.size());
}

TEST(TaskManagerTest, CreateProcess)
{
    using namespace libtbag::filesystem;
    auto const tbproc_exe = Path::getExeDir() / getExecutableName("tbproc");

    ProcessParams params;
    params.file = tbproc_exe;
    params.args = { "out", "temp" };
    params.envs = libtbag::string::Environments::createDefaultEnvironments(true).toStrings();
    params.cwd = libtbag::filesystem::Path::getWorkDir();
    params.input = {};

    TaskManager mgr;
    auto const result = mgr.runProcess(params);
    ASSERT_EQ(1u, mgr.size());
    ASSERT_EQ(E_SUCCESS, result.code);

    auto const task_id = result.value;
    ASSERT_EQ(E_SUCCESS, mgr.join(task_id));

    auto const err_task_info = mgr.getTaskInfo(task_id);
    ASSERT_EQ(E_SUCCESS, err_task_info.code);
    auto const & task_info = err_task_info.value;
    ASSERT_EQ(TaskManager::TaskType::TT_PROCESS, task_info.type);
    ASSERT_TRUE(task_info.done);
    ASSERT_EQ(0, task_info.exit_status);
    ASSERT_EQ(0, task_info.term_signal);

    auto const erase_result = mgr.erase(task_id);
    ASSERT_EQ(E_SUCCESS, erase_result);

    ASSERT_TRUE(mgr.empty());
    ASSERT_EQ(0u, mgr.size());
}

TEST(TaskManagerTest, KillProcess)
{
    using namespace libtbag::filesystem;
    auto const tbproc_exe = Path::getExeDir() / getExecutableName("tbproc");

    ProcessParams params;
    params.file = tbproc_exe;
    params.args = { "sleep" };
    params.envs = libtbag::string::Environments::createDefaultEnvironments(true).toStrings();
    params.cwd = libtbag::filesystem::Path::getWorkDir();
    params.input = {};

    TaskManager mgr;
    auto const result = mgr.runProcess(params);
    ASSERT_EQ(1u, mgr.size());
    ASSERT_EQ(E_SUCCESS, result.code);

    auto const task_id = result.value;
    ASSERT_EQ(E_SUCCESS, mgr.kill(task_id));
    ASSERT_EQ(E_SUCCESS, mgr.join(task_id));

    auto const err_task_info = mgr.getTaskInfo(task_id);
    ASSERT_EQ(E_SUCCESS, err_task_info.code);
    auto const & task_info = err_task_info.value;
    ASSERT_EQ(TaskManager::TaskType::TT_PROCESS, task_info.type);
    ASSERT_TRUE(task_info.done);
    if (isWindowsPlatform()) {
        ASSERT_EQ(1, task_info.exit_status);
    } else {
        ASSERT_EQ(0, task_info.exit_status);
    }
    ASSERT_EQ(libtbag::signal::TBAG_SIGNAL_KILL, task_info.term_signal);

    auto const erase_result = mgr.erase(task_id);
    ASSERT_EQ(E_SUCCESS, erase_result);

    ASSERT_TRUE(mgr.empty());
    ASSERT_EQ(0u, mgr.size());
}

