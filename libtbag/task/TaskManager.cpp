/**
 * @file   TaskManager.cpp
 * @brief  TaskManager class implementation.
 * @author zer0
 * @date   2019-12-19
 */

#include <libtbag/task/TaskManager.hpp>
#include <libtbag/signal/SignalHandler.hpp>
#include <libtbag/thread/ThreadKill.hpp>
#include <libtbag/thread/ThreadLocalStorage.hpp>
#include <libtbag/container/Global.hpp>
#include <libtbag/lock/UvLock.hpp>
#include <libtbag/lock/UvCondition.hpp>
#include <libtbag/log/Log.hpp>

#include <cassert>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace task {

using TaskId = TaskManager::TaskId;
using ErrTaskId = TaskManager::ErrTaskId;
using ErrTaskInfo = TaskManager::ErrTaskInfo;
using TaskInfoMap = TaskManager::TaskInfoMap;

TBAG_CONSTEXPR static char const * const GLOBAL_MANAGER_REGISTER_KEY =
        "libtbag::task::_manager_register";
TBAG_CONSTEXPR static int const THREAD_TASK_KILL_SIGNAL =
        libtbag::signal::TBAG_SIGNAL_INTERRUPT;

struct _manager_register TBAG_FINAL : private Noncopyable
{
    using Tls = libtbag::thread::ThreadLocalStorage;

    Tls mgr;
    Tls id;

    _manager_register() { /* EMPTY. */ }
    ~_manager_register() { /* EMPTY. */ }
};

static std::shared_ptr<_manager_register> __get_global_manager_register()
{
    using namespace libtbag::container;
    auto weak = findGlobalObject<_manager_register>(GLOBAL_MANAGER_REGISTER_KEY);
    if (auto shared = weak.lock()) {
        return shared;
    } else {
        return newGlobalObject<_manager_register>(GLOBAL_MANAGER_REGISTER_KEY);
    }
}

static TaskId __get_current_task_id()
{
    return (TaskId)(std::intptr_t)__get_global_manager_register()->id.get();
}

void __task_interrupt_signal(int signum)
{
    auto const shared = __get_global_manager_register();
    TaskManager * mgr = shared->mgr.cast<TaskManager>();
    TaskId id = (TaskId)(std::intptr_t)shared->id.get();
    mgr->_on_thread_exit(id, EXIT_FAILURE, signum);
    libtbag::thread::exitThread(EXIT_FAILURE);
}

static void __init_task_thread(TaskManager * mgr, TaskId id)
{
    auto const shared = __get_global_manager_register();
    assert(static_cast<bool>(shared));
    shared->mgr.set(mgr);
    shared->id.set((void*)(std::intptr_t)id);

#if !defined(TBAG_PLATFORM_WINDOWS)
    std::signal(THREAD_TASK_KILL_SIGNAL, &__task_interrupt_signal);
#endif
}

// --------------------------
// TaskManager implementation
// --------------------------

TaskManager::TaskManager(Callback * cb, bool auto_clear)
        : TASK_CALLBACK(cb), AUTO_CLEAR(auto_clear), _task_number(BEGIN_TASK_ID)
{
    auto const shared = __get_global_manager_register();
    assert(static_cast<bool>(shared));
    using namespace std::placeholders;
    _processes.out_read_cb = std::bind(&TaskManager::_on_process_out, this, _1, _2, _3);
    _processes.err_read_cb = std::bind(&TaskManager::_on_process_err, this, _1, _2, _3);
    _processes.exit_cb = std::bind(&TaskManager::_on_process_exit, this, _1, _2, _3);
}

TaskManager::~TaskManager()
{
    if (AUTO_CLEAR) {
        clear();
    }
}

void TaskManager::clearUnsafe()
{
    _tasks.clear();
    _pid2task.clear();
    _threads.clear();
    _processes.clear();
}

void TaskManager::clear()
{
    WriteLockGuard const __task_lock__(_tasks_lock);
    WriteLockGuard const __thread_lock__(_threads_lock);
    WriteLockGuard const __process_lock__(_processes_lock);

    for (auto const & task : _tasks) {
        auto const & task_info = task.second;
        if (!task_info.done && !task_info.killed) {
            if (task_info.type == TaskType::TT_PROCESS) {
                _processes.kill(task_info.internal_id.process, libtbag::signal::TBAG_SIGNAL_KILL);
            } else {
                assert(task_info.type == TaskType::TT_THREAD);
                if (isWindowsPlatform()) {
                    _threads.cancel(task_info.internal_id.thread);
                } else {
                    _threads.kill(task_info.internal_id.thread, THREAD_TASK_KILL_SIGNAL);
                }
            }
        }
        if (task_info.type == TaskType::TT_PROCESS) {
            _processes.join(task_info.internal_id.process);
        } else {
            assert(task_info.type == TaskType::TT_THREAD);
            _threads.join(task_info.internal_id.thread, false);
        }
    }

    clearUnsafe();
}

TaskId TaskManager::getCurrentTaskId()
{
    return __get_current_task_id();
}

TaskId TaskManager::nextTaskId()
{
    while (true) {
        auto const id = _task_number++;
        if (!exists(id)) {
            return id;
        }
    }
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

TaskInfoMap TaskManager::getTaskInfos() const
{
    TaskInfoMap result;
    ReadLockGuard const G(_tasks_lock);
    for (auto const & itr : _tasks) {
        result.emplace(itr.first, itr.second);
    }
    return result;
}

void TaskManager::_on_thread_exit(TaskId id, int64_t exit_status, int term_signal)
{
    tDLogN("TaskManager::_on_thread_exit(task_id={},exit={},signum={}) function was called.",
           id, exit_status, term_signal);
    COMMENT("Tasks lock") {
        WriteLockGuard const G(_tasks_lock);
        auto itr = _tasks.find(id);
        assert(itr != _tasks.end());
        itr->second.done = true;
        itr->second.exit_status = exit_status;
        itr->second.term_signal = term_signal;
    }
    if (TASK_CALLBACK) {
        TASK_CALLBACK->onThreadExit(id, exit_status, term_signal);
    }
}

void TaskManager::_on_process_out(int pid, char const * buffer, std::size_t size)
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
    if (TASK_CALLBACK) {
        TASK_CALLBACK->onProcessOut(task_id, buffer, size);
    }
}

void TaskManager::_on_process_err(int pid, char const * buffer, std::size_t size)
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
    if (TASK_CALLBACK) {
        TASK_CALLBACK->onProcessErr(task_id, buffer, size);
    }
}

void TaskManager::_on_process_exit(int pid, int64_t exit_status, int term_signal)
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
    tDLogN("TaskManager::_on_process_exit(task_id={},pid={},exit={},signum={}) function was called.",
           task_id, pid, exit_status, term_signal);
    if (TASK_CALLBACK) {
        TASK_CALLBACK->onProcessExit(task_id, exit_status, term_signal);
    }
}

ErrTaskId TaskManager::runThread(ThreadParams const & params, void * opaque)
{
    auto const task_id = nextTaskId();

    libtbag::lock::UvLock lock;
    libtbag::lock::UvCondition signal;
    bool init_task_end = false;

    // [IMPORTANT] Before adding a task, to prevent access from a new thread.
    WriteLockGuard const G(_tasks_lock);

    _threads_lock.writeLock();
    auto const thread_id = _threads.createThread([this, params, opaque, task_id, &lock, &signal, &init_task_end](){
        __init_task_thread(this, task_id);

        lock.lock();
        init_task_end = true;
        signal.signal();
        lock.unlock();

        if (params.runner) {
            params.runner();
        }

        _on_thread_exit(task_id, EXIT_SUCCESS, 0);
    });
    _threads_lock.writeUnlock();

    if (thread_id == uthread()) {
        return E_CREATE;
    }

    TaskInfo info = {};
    info.type = TaskType::TT_THREAD;
    info.internal_id = createThreadTaskId(thread_id);
    info.done = false;
    info.killed = false;
    info.exit_status = 0;
    info.term_signal = 0;
    info.opaque = opaque;
    auto const task_insert_result = _tasks.emplace(task_id, info).second;
    assert(task_insert_result);

    // Do not unlock until the signal is registered.
    lock.lock();
    while (!init_task_end) {
        signal.wait(lock);
    }
    lock.unlock();

    return { E_SUCCESS, task_id };
}

ErrTaskId TaskManager::runProcess(ProcessParams const & params, void * opaque)
{
    auto const task_id = nextTaskId();

    // [IMPORTANT] Before adding a task, to prevent access from a new thread.
    WriteLockGuard const G(_tasks_lock);

    _processes_lock.writeLock();
    auto const pid = _processes.exec(params.file, params.args, params.envs, params.cwd, params.input,
                                     params.enable_stdout, params.enable_stderr);
    _processes_lock.writeUnlock();

    if (pid == 0) {
        return E_CREATE;
    }

    TaskInfo info = {};
    info.type = TaskType::TT_PROCESS;
    info.internal_id = createProcessTaskId(pid);
    info.done = false;
    info.killed = false;
    info.exit_status = 0;
    info.term_signal = 0;
    info.opaque = opaque;
    auto const task_insert_result = _tasks.emplace(task_id, info).second;
    assert(task_insert_result);
    auto const pid_insert_result = _pid2task.emplace(pid, task_id).second;
    assert(pid_insert_result);
    return { E_SUCCESS, task_id };
}

Err TaskManager::join(TaskId id)
{
    auto const err_task_info = getTaskInfo(id);
    if (!err_task_info) {
        return err_task_info.code;
    }
    auto const & task_info = err_task_info.value;

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
    WriteLockGuard const __task_guard__(_tasks_lock);
    auto const itr = _tasks.find(id);
    if (itr == _tasks.end()) {
        return E_NFOUND;
    }
    auto & task_info = itr->second;

    if (task_info.type == TaskType::TT_PROCESS) {
        WriteLockGuard const __process_guard__(_processes_lock);
        auto const erase_result = _processes.erase(task_info.internal_id.process);
        assert(erase_result);
    } else {
        assert(task_info.type == TaskType::TT_THREAD);
        WriteLockGuard const __thread_guard__(_threads_lock);
        auto const erase_result = _threads.erase(task_info.internal_id.thread);
        assert(erase_result);
    }

    if (task_info.type == TaskType::TT_PROCESS) {
        auto const pid2task_erase_result = _pid2task.erase(task_info.internal_id.process);
        assert(pid2task_erase_result == 1u);
    }
    return _tasks.erase(id) == 1u ? E_SUCCESS : E_ERASE;
}

Err TaskManager::kill(TaskId id)
{
    WriteLockGuard const __task_guard__(_tasks_lock);
    auto const itr = _tasks.find(id);
    if (itr == _tasks.end()) {
        return E_NFOUND;
    }

    auto & task_info = itr->second;
    if (task_info.done) {
        return E_ALREADY;
    }
    if (task_info.killed) {
        return E_ILLSTATE;
    }

    Err kill_result = E_UNKNOWN;

    if (task_info.type == TaskType::TT_PROCESS) {
        auto const pid = task_info.internal_id.process;
        tDLogN("TaskManager::kill(task_id={}) Process ID: {}", id, pid);
        WriteLockGuard const __process_guard__(_processes_lock);
        kill_result = _processes.kill(pid, libtbag::signal::TBAG_SIGNAL_KILL);
    } else {
        assert(task_info.type == TaskType::TT_THREAD);
        auto const thread_id = task_info.internal_id.thread;
        tDLogN("TaskManager::kill(task_id={}) Thread ID: {}", id, (void*)thread_id);

        WriteLockGuard const __thread_guard__(_threads_lock);
        if (isWindowsPlatform()) {
            auto const cancel_result = _threads.cancel(thread_id);

            // [IMPORTANT]
            // On Windows platforms, the thread terminates immediately.
            // Therefore, you must manipulate the attribute yourself.
            if (isSuccess(cancel_result)) {
                itr->second.done = true;
                itr->second.exit_status = EXIT_FAILURE;
                itr->second.term_signal = THREAD_TASK_KILL_SIGNAL;
            }
            if (TASK_CALLBACK) {
                TASK_CALLBACK->onThreadExit(id, EXIT_FAILURE, THREAD_TASK_KILL_SIGNAL);
            }
            kill_result = cancel_result;
        } else {
            kill_result = _threads.kill(thread_id, THREAD_TASK_KILL_SIGNAL);
        }
    }

    if (isSuccess(kill_result)) {
        task_info.killed = true;
    }
    return kill_result;
}

} // namespace task

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

