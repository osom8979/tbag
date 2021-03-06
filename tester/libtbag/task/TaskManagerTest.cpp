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

    TaskManager::TaskId id1;
    TaskManager::TaskId id2;

    ThreadParams params1;
    params1.runner = [&]() -> int {
        id1 = TaskManager::getCurrentTaskId();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        test1 = true;
        return EXIT_SUCCESS;
    };
    ThreadParams params2;
    params2.runner = [&]() -> int {
        id2 = TaskManager::getCurrentTaskId();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        test2 = true;
        return EXIT_SUCCESS;
    };

    auto const result1 = mgr.runThread(params1);
    ASSERT_EQ(E_SUCCESS, result1);
    auto const result2 = mgr.runThread(params2);
    ASSERT_EQ(E_SUCCESS, result2);

    ASSERT_FALSE(mgr.empty());
    ASSERT_EQ(2u, mgr.size());

    auto const task_list = mgr.list();
    ASSERT_EQ(2u, task_list.size());

    auto const join_result1 = mgr.join(task_list[0]);
    ASSERT_EQ(E_SUCCESS, join_result1);
    auto const join_result2 = mgr.join(task_list[1]);
    ASSERT_EQ(E_SUCCESS, join_result2);

    ASSERT_EQ(id1, result1.val);
    ASSERT_EQ(id2, result2.val);

    ASSERT_TRUE(test1);
    ASSERT_TRUE(test2);

    ASSERT_FALSE(mgr.empty());
    ASSERT_EQ(2u, mgr.size());

    auto const task_info1 = mgr.getTaskInfo(task_list[0]);
    ASSERT_EQ(E_SUCCESS, task_info1);
    auto const task_info2 = mgr.getTaskInfo(task_list[1]);
    ASSERT_EQ(E_SUCCESS, task_info2);

    auto const infos = mgr.getTaskInfos();
    ASSERT_EQ(2, infos.size());

    ASSERT_EQ(TaskManager::TaskType::TT_THREAD, task_info1.val.type);
    ASSERT_EQ(TaskManager::TaskType::TT_THREAD, task_info2.val.type);
    ASSERT_TRUE(task_info1.val.done);
    ASSERT_TRUE(task_info2.val.done);
    ASSERT_FALSE(task_info1.val.killed);
    ASSERT_FALSE(task_info2.val.killed);

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
    params.runner = [&]() -> int {
        throw ErrException(E_UNKNOWN);
        return EXIT_FAILURE;
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
    params.runner = [&]() -> int {
        std::this_thread::sleep_for(std::chrono::minutes(1));
        return EXIT_FAILURE;
    };

    TaskManager mgr;
    auto const result = mgr.runThread(params);
    ASSERT_EQ(1u, mgr.size());
    ASSERT_EQ(E_SUCCESS, result);

    auto const task_id = result.val;
    ASSERT_EQ(E_SUCCESS, mgr.kill(task_id));
    ASSERT_EQ(E_SUCCESS, mgr.join(task_id));

    auto const err_task_info = mgr.getTaskInfo(task_id);
    ASSERT_EQ(E_SUCCESS, err_task_info);
    auto const & task_info = err_task_info.val;
    ASSERT_EQ(TaskManager::TaskType::TT_THREAD, task_info.type);
    ASSERT_TRUE(task_info.done);
    ASSERT_TRUE(task_info.killed);
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
    ASSERT_EQ(E_SUCCESS, result);

    auto const task_id = result.val;
    ASSERT_EQ(E_SUCCESS, mgr.join(task_id));

    auto const err_task_info = mgr.getTaskInfo(task_id);
    ASSERT_EQ(E_SUCCESS, err_task_info);
    auto const & task_info = err_task_info.val;
    ASSERT_EQ(TaskManager::TaskType::TT_PROCESS, task_info.type);
    ASSERT_TRUE(task_info.done);
    ASSERT_FALSE(task_info.killed);
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
    ASSERT_EQ(E_SUCCESS, result);

    auto const task_id = result.val;
    ASSERT_EQ(E_SUCCESS, mgr.kill(task_id));
    ASSERT_EQ(E_SUCCESS, mgr.join(task_id));

    auto const err_task_info = mgr.getTaskInfo(task_id);
    ASSERT_EQ(E_SUCCESS, err_task_info);
    auto const & task_info = err_task_info.val;
    ASSERT_EQ(TaskManager::TaskType::TT_PROCESS, task_info.type);
    ASSERT_TRUE(task_info.done);
    ASSERT_TRUE(task_info.killed);
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

