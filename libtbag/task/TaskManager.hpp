/**
 * @file   TaskManager.hpp
 * @brief  TaskManager class prototype.
 * @author zer0
 * @date   2019-12-19
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TASK_TASKMANAGER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TASK_TASKMANAGER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/ErrPair.hpp>
#include <libtbag/lock/RwLock.hpp>
#include <libtbag/thread/ThreadGroup.hpp>
#include <libtbag/process/ProcessManager.hpp>
#include <libtbag/Type.hpp>

#include <string>
#include <vector>
#include <atomic>
#include <functional>
#include <unordered_map>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace task {

/**
 * TaskManager class prototype.
 *
 * @author zer0
 * @date   2019-12-19
 */
class TBAG_API TaskManager : private Noncopyable
{
public:
    using RwLock         = libtbag::lock::RwLock;
    using ReadLockGuard  = libtbag::lock::ReadLockGuard;
    using WriteLockGuard = libtbag::lock::WriteLockGuard;

    using ThreadGroup = libtbag::thread::ThreadGroup;
    using ProcessManager = libtbag::process::ProcessManager;

    using uthread = libtbag::uvpp::uthread;
    using TaskId = std::int32_t;
    using ErrTaskId = libtbag::ErrPair<TaskId>;

    /** ThreadID or ProcessID. */
    union InternalTaskId
    {
        uthread thread;
        int process;
    };

    static InternalTaskId createThreadTaskId(uthread id) TBAG_NOEXCEPT
    {
        InternalTaskId result = {};
        result.thread = id;
        return result;
    }

    static InternalTaskId createProcessTaskId(int id) TBAG_NOEXCEPT
    {
        InternalTaskId result = {};
        result.process = id;
        return result;
    }

public:
    TBAG_CONSTEXPR static TaskId const UNKNOWN_TASK_ID = 0;
    TBAG_CONSTEXPR static TaskId const BEGIN_TASK_ID = 1;

public:
    enum class TaskType
    {
        TT_THREAD,
        TT_PROCESS,
    };

    inline static char const * getTaskTypeName(TaskType t) TBAG_NOEXCEPT
    {
        switch (t) {
        case TaskType::TT_THREAD:
            return "thread";
        case TaskType::TT_PROCESS:
            return "process";
        default:
            return "unknown";
        }
    }

    struct ThreadParams
    {
        using Callback = std::function<int(void)>;
        Callback runner;
    };

    struct ProcessParams
    {
        std::string file;
        std::vector<std::string> args;
        std::vector<std::string> envs;
        std::string cwd;
        std::string input;
        bool enable_stdout = true;
        bool enable_stderr = true;
    };

    struct TaskInfo
    {
        /** thread or process? */
        TaskType type;

        /** Internal ID. */
        InternalTaskId internal_id;

        /** Initialize params. */
        std::shared_ptr<void> params;

        /** Task id done? */
        bool done = false;

        /** Task id killed? */
        bool killed = false;

        /** Exit status code. */
        int64_t exit_status = 0;

        /** Terminate signal number. */
        int term_signal = 0;

        /** User's data. */
        void * opaque = nullptr;

        std::weak_ptr<ThreadParams> getThreadParams() const
        {
            if (type == TaskType::TT_THREAD) {
                return std::static_pointer_cast<ThreadParams>(params);
            }
            return {};
        }

        std::weak_ptr<ProcessParams> getProcessParams() const
        {
            if (type == TaskType::TT_PROCESS) {
                return std::static_pointer_cast<ProcessParams>(params);
            }
            return {};
        }
    };

    using ErrTaskInfo = libtbag::ErrPair<TaskInfo>;
    using TaskMap = std::unordered_map<TaskId, TaskInfo>;
    using Pid2Task = std::unordered_map<int, TaskId>;
    using TaskInfoMap = std::unordered_map<TaskId, TaskInfo>;

    struct Callback
    {
        Callback() { /* EMPTY. */ }
        virtual ~Callback() { /* EMPTY. */ }

        virtual void onThreadExit(TaskId id, int64_t exit_status, int term_signal) { /* EMPTY. */ }
        virtual void onProcessOut(TaskId id, char const * buffer, std::size_t size) { /* EMPTY. */ }
        virtual void onProcessErr(TaskId id, char const * buffer, std::size_t size) { /* EMPTY. */ }
        virtual void onProcessExit(TaskId id, int64_t exit_status, int term_signal) { /* EMPTY. */ }
    };

private:
    /** @warning 'CALLBACK' are being used as macros in MSVC. */
    Callback * const TASK_CALLBACK;

    /** Call the 'clear' method by the destructor. */
    bool const AUTO_CLEAR;

private:
    RwLock mutable _threads_lock;
    ThreadGroup _threads;

private:
    RwLock mutable _processes_lock;
    ProcessManager _processes;

private:
    RwLock mutable _tasks_lock;
    TaskMap _tasks;
    Pid2Task _pid2task;

private:
    std::atomic<TaskId> _task_number;

public:
    TaskManager(Callback * cb = nullptr, bool auto_clear = true);
    virtual ~TaskManager();

public:
    void registerCallbacks();
    void clear();

private:
    TBAG_API friend void __task_interrupt_signal(int signum);

public:
    /**
     * Can only be called from a thread task.
     */
    static TaskId getCurrentTaskId();

public:
    TaskId nextTaskId();

public:
    std::size_t size() const;
    bool empty() const;

public:
    std::vector<TaskId> list() const;
    TaskMap map() const;

public:
    bool exists(TaskId id) const;
    ErrTaskInfo getTaskInfo(TaskId id) const;
    TaskInfoMap getTaskInfos() const;

private:
    void _on_thread_exit(TaskId id, int64_t exit_status, int term_signal);
    void _on_process_out(int pid, char const * buffer, std::size_t size);
    void _on_process_err(int pid, char const * buffer, std::size_t size);
    void _on_process_exit(int pid, int64_t exit_status, int term_signal);

public:
    ErrTaskId runThread(ThreadParams const & params, void * opaque = nullptr);
    ErrTaskId runProcess(ProcessParams const & params, void * opaque = nullptr);

public:
    Err join(TaskId id);
    Err erase(TaskId id);
    Err kill(TaskId id);
};

} // namespace task

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TASK_TASKMANAGER_HPP__

