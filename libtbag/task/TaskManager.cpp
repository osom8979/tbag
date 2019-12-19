/**
 * @file   TaskManager.cpp
 * @brief  TaskManager class implementation.
 * @author zer0
 * @date   2019-12-19
 */

#include <libtbag/task/TaskManager.hpp>
#include <libtbag/signal/SignalHandler.hpp>
#include <libtbag/log/Log.hpp>

#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace task {

using TaskId = TaskManager::TaskId;
using ErrTaskId = TaskManager::ErrTaskId;
using ErrTaskInfo = TaskManager::ErrTaskInfo;

TaskManager::TaskManager(TaskCallback * cb)
        : CALLBACK(cb), _task_number(BEGIN_TASK_ID)
{
    using namespace std::placeholders;
    _processes.out_read_cb = std::bind(&TaskManager::onProcessOut, this, _1, _2, _3);
    _processes.err_read_cb = std::bind(&TaskManager::onProcessErr, this, _1, _2, _3);
    _processes.exit_cb = std::bind(&TaskManager::onProcessExit, this, _1, _2, _3);
}

TaskManager::~TaskManager()
{
    // EMPTY.
}

std::size_t TaskManager::size() const
{
    ReadLockGuard const G(_tasks_lock);
    return _tasks.size();
}

bool TaskManager::empty() const
{
    ReadLockGuard const G(_tasks_lock);
    return _tasks.empty();
}

std::vector<TaskId> TaskManager::list() const
{
    std::vector<TaskId> result;
    ReadLockGuard const G(_tasks_lock);
    for (auto const & task : _tasks) {
        result.push_back(task.first);
    }
    return result;
}

TaskManager::TaskMap TaskManager::map() const
{
    ReadLockGuard const G(_tasks_lock);
    return _tasks;
}

bool TaskManager::exists(TaskId id) const
{
    ReadLockGuard const G(_tasks_lock);
    return _tasks.find(id) != _tasks.end();
}

ErrTaskInfo TaskManager::getTaskInfo(TaskId id) const
{
    ReadLockGuard const G(_tasks_lock);
    auto const itr = _tasks.find(id);
    if (itr == _tasks.end()) {
        return E_NFOUND;
    }
    return { E_SUCCESS, itr->second };
}

void TaskManager::onThreadExit(TaskId id)
{
    tDLogN("TaskManager::onThreadExit(task_id={}) function was called.", id);
    COMMENT("Tasks lock") {
        WriteLockGuard const G(_tasks_lock);
        auto itr = _tasks.find(id);
        assert(itr != _tasks.end());
        itr->second.done = true;
    }
    if (CALLBACK) {
        CALLBACK->onThreadExit(id);
    }
}

void TaskManager::onProcessOut(int pid, char const * buffer, std::size_t size)
{
    TaskId task_id;
    COMMENT("Tasks lock") {
        ReadLockGuard const G(_tasks_lock);
        auto itr0 = _pid2task.find(pid);
        assert(itr0 != _pid2task.end());
        task_id = itr0->second;
        auto itr = _tasks.find(task_id);
        assert(itr != _tasks.end());
    }
    if (CALLBACK) {
        CALLBACK->onProcessOut(task_id, buffer, size);
    }
}

void TaskManager::onProcessErr(int pid, char const * buffer, std::size_t size)
{
    TaskId task_id;
    COMMENT("Tasks lock") {
        ReadLockGuard const G(_tasks_lock);
        auto itr0 = _pid2task.find(pid);
        assert(itr0 != _pid2task.end());
        task_id = itr0->second;
        auto itr = _tasks.find(task_id);
        assert(itr != _tasks.end());
    }
    if (CALLBACK) {
        CALLBACK->onProcessErr(task_id, buffer, size);
    }
}

void TaskManager::onProcessExit(int pid, int64_t exit_status, int term_signal)
{
    TaskId task_id;
    COMMENT("Tasks lock") {
        WriteLockGuard const G(_tasks_lock);
        auto itr0 = _pid2task.find(pid);
        assert(itr0 != _pid2task.end());
        task_id = itr0->second;
        auto itr = _tasks.find(task_id);
        assert(itr != _tasks.end());
        itr->second.done = true;
        itr->second.exit_status = exit_status;
        itr->second.term_signal = term_signal;
    }
    tDLogN("TaskManager::onProcessExit(task_id={},pid={},exit={},signum={}) function was called.",
           task_id, pid, exit_status, term_signal);
    if (CALLBACK) {
        CALLBACK->onProcessExit(task_id, exit_status, term_signal);
    }
}

ErrTaskId TaskManager::runThread(ThreadParams const & params, void * opaque)
{
    auto const TASK_ID = _task_number++;

    // [IMPORTANT] Before adding a task, to prevent access from a new thread.
    WriteLockGuard const G(_tasks_lock);

    _threads_lock.writeLock();
    auto const thread_id = _threads.createThread([this, params, TASK_ID](){
        if (params.runner) {
            params.runner();
        }
        onThreadExit(TASK_ID);
    });
    _threads_lock.writeUnlock();

    if (thread_id == uthread()) {
        return E_CREATE;
    }

    TaskInfo info = {};
    info.type = TaskType::TT_THREAD;
    info.internal_id = createThreadTaskId(thread_id);
    info.done = false;
    info.exit_status = 0;
    info.term_signal = 0;
    info.opaque = opaque;
    if (!_tasks.emplace(TASK_ID, info).second) {
        return E_INSERT;
    }
    return { E_SUCCESS, TASK_ID };
}

ErrTaskId TaskManager::runProcess(ProcessParams const & params, void * opaque)
{
    auto const TASK_ID = _task_number++;

    // [IMPORTANT] Before adding a task, to prevent access from a new thread.
    WriteLockGuard const G(_tasks_lock);

    _processes_lock.writeLock();
    auto const pid = _processes.exec(params.file, params.args, params.envs, params.cwd, params.input);
    _processes_lock.writeUnlock();

    if (pid == 0) {
        return E_CREATE;
    }

    TaskInfo info = {};
    info.type = TaskType::TT_PROCESS;
    info.internal_id = createProcessTaskId(pid);
    info.done = false;
    info.exit_status = 0;
    info.term_signal = 0;
    info.opaque = opaque;
    if (!_tasks.emplace(TASK_ID, info).second) {
        return E_INSERT;
    }
    if (!_pid2task.emplace(pid, TASK_ID).second) {
        return E_INSERT;
    }
    return { E_SUCCESS, TASK_ID };
}

Err TaskManager::join(TaskId id)
{
    auto const err_task_info = getTaskInfo(id);
    if (!err_task_info) {
        return err_task_info.code;
    }
    auto const & task_info = err_task_info.value;

    // [IMPORTANT] Do not change the calling order.

    if (task_info.type == TaskType::TT_PROCESS) {
        auto const pid = task_info.internal_id.process;
        tDLogN("TaskManager::join(task_id={}) Process ID: {}", id, pid);
        WriteLockGuard const G2(_processes_lock);
        return _processes.join(pid);
    } else {
        assert(task_info.type == TaskType::TT_THREAD);
        auto const thread_id = task_info.internal_id.thread;
        tDLogN("TaskManager::join(task_id={}) Thread ID: {}", id, (void*)thread_id);
        WriteLockGuard const G2(_threads_lock);
        return _threads.join(thread_id, true);
    }
}

Err TaskManager::erase(TaskId id)
{
    auto const err_task_info = getTaskInfo(id);
    if (!err_task_info) {
        return err_task_info.code;
    }
    auto const & task_info = err_task_info.value;

    // [IMPORTANT] Do not change the calling order.

    if (task_info.type == TaskType::TT_PROCESS) {
        WriteLockGuard const G2(_processes_lock);
        auto const erase_result = _processes.erase(task_info.internal_id.process);
        assert(erase_result);
    } else {
        assert(task_info.type == TaskType::TT_THREAD);
        WriteLockGuard const G2(_threads_lock);
        auto const erase_result = _threads.erase(task_info.internal_id.thread);
        assert(erase_result);
    }

    // [IMPORTANT] Do not change the calling order.

    WriteLockGuard const G2(_tasks_lock);
    if (task_info.type == TaskType::TT_PROCESS) {
        auto const pid2task_erase_result = _pid2task.erase(task_info.internal_id.process);
        assert(pid2task_erase_result == 1u);
    }
    return _tasks.erase(id) == 1u ? E_SUCCESS : E_ERASE;
}

Err TaskManager::kill(TaskId id)
{
    auto const err_task_info = getTaskInfo(id);
    if (!err_task_info) {
        return err_task_info.code;
    }
    auto const & task_info = err_task_info.value;

    // [IMPORTANT] Do not change the calling order.

    if (task_info.type == TaskType::TT_PROCESS) {
        auto const pid = task_info.internal_id.process;
        tDLogN("TaskManager::kill(task_id={}) Process ID: {}", id, pid);
        WriteLockGuard const G2(_processes_lock);
        return _processes.kill(pid, libtbag::signal::TBAG_SIGNAL_KILL);
    } else {
        assert(task_info.type == TaskType::TT_THREAD);
        auto const thread_id = task_info.internal_id.thread;
        tDLogN("TaskManager::kill(task_id={}) Thread ID: {}", id, (void*)thread_id);
        WriteLockGuard const G2(_threads_lock);
        return _threads.kill(thread_id);
    }
}

} // namespace task

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

