/**
 * @file   TaskExecutor.hpp
 * @brief  TaskExecutor class prototype.
 * @author zer0
 * @date   2016-05-16
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_THREAD_TASKEXECUTOR_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_THREAD_TASKEXECUTOR_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/thread/ThreadGroup.hpp>
#include <libtbag/Exception.hpp>
#include <libtbag/lock/SpinLock.hpp>
#include <libtbag/log/Log.hpp>

#include <queue>
#include <memory>
#include <chrono>
#include <mutex>
#include <functional>
#include <condition_variable>

//#define ENABLE_TASKEXECUTOR_DEBUG

#if defined(ENABLE_TASKEXECUTOR_DEBUG)
# define __TASKEXECUTOR_DEBUG_PREFIX         "[TaskExecutor] "
# define __TASKEXECUTOR_DEBUG(m)             tDLogD(__TASKEXECUTOR_DEBUG_PREFIX m)
# define __TASKEXECUTOR_DEBUG_FORMAT(m, ...) tDLogDF(__TASKEXECUTOR_DEBUG_PREFIX m, __VA_ARGS__)
#else
# define __TASKEXECUTOR_DEBUG(m)
# define __TASKEXECUTOR_DEBUG_FORMAT(m, ...)
#endif

#define __ENTER_TASKEXECUTOR_LOCK(mutex) __TASKEXECUTOR_DEBUG("LOCK!!"); mutex.lock();
#define __LEAVE_TASKEXECUTOR_LOCK(mutex) __TASKEXECUTOR_DEBUG("UNLOCK"); mutex.unlock();

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace thread {

/**
 * TaskExecutor class prototype.
 *
 * @author zer0
 * @date   2016-05-16
 */
class TaskExecutor : public Noncopyable
{
public:
    using Task       = std::function<void(void)>;
    using SharedTask = std::shared_ptr<Task>;
    using TaskQueue  = std::queue<SharedTask>;

public:
    using Mutex  = std::mutex;
    using Signal = std::condition_variable;

private:
    ThreadGroup _threads;

private:
    mutable std::mutex _locker;
    Signal    _condition;
    TaskQueue _queue;
    bool      _exit;

public:
    TaskExecutor() : _exit(false)
    {
#if defined(ENABLE_TASKEXECUTOR_DEBUG)
        if (log::getDefaultLogger() == nullptr) {
            log::createDefaultColorConsoleLogger();
            log::setDefaultLevel(log::LogLevel::LEVEL_DEBUG);
        }
#endif
        __TASKEXECUTOR_DEBUG("TaskExecutor()");
    }

    ~TaskExecutor()
    {
        this->exit();
        this->clear();
        this->join();
        __TASKEXECUTOR_DEBUG("~TaskExecutor()");
    }

public:
    bool push(Task const & task)
    {
        SharedTask new_task;
        try {
            new_task = SharedTask(new Task(task));
        } catch (...) {
            return false;
        }

        bool result = false;

        __ENTER_TASKEXECUTOR_LOCK(_locker);
        if (_exit == false) {
            _queue.push(new_task);
            result = true;
        }
        __LEAVE_TASKEXECUTOR_LOCK(_locker);

        _condition.notify_one();
        return result;
    }

    SharedTask pop()
    {
        SharedTask result;

        __ENTER_TASKEXECUTOR_LOCK(_locker);
        if (_queue.empty() == false) {
            result = this->_queue.front();
            _queue.pop();
        }
        __LEAVE_TASKEXECUTOR_LOCK(_locker);

        _condition.notify_one();
        return result;
    }

    void clear()
    {
        __ENTER_TASKEXECUTOR_LOCK(_locker);
        while (_queue.empty() == false) {
            _queue.pop();
        }
        __LEAVE_TASKEXECUTOR_LOCK(_locker);
        _condition.notify_all();
    }

    void exit(bool flag = true)
    {
        __ENTER_TASKEXECUTOR_LOCK(_locker);
        _exit = flag;
        __LEAVE_TASKEXECUTOR_LOCK(_locker);
        _condition.notify_all();
    }

    bool isExit() const
    {
        std::lock_guard<std::mutex> guard(_locker);
        return _exit;
    }

    std::size_t sizeOfQueue() const
    {
        std::lock_guard<std::mutex> guard(_locker);
        return _queue.size();
    }

    bool emptyOfQueue() const
    {
        std::lock_guard<std::mutex> guard(_locker);
        return _queue.empty();
    }

private:
    void runner()
    {
        __TASKEXECUTOR_DEBUG("runner() START");

        do {
            __TASKEXECUTOR_DEBUG("runner() LOCK");
            {
                std::unique_lock<std::mutex> locker(_locker);
                _condition.wait(locker, [&]() -> bool {
                    return (_exit || _queue.empty() == false);
                });
            }
            __TASKEXECUTOR_DEBUG("runner() UNLOCK");

            SharedTask current_task = this->pop();
            while (current_task.get() != nullptr) {
                if (static_cast<bool>(*current_task)) {
                    (*current_task)();
                }
                current_task = this->pop();
            }
        } while (isExit() == false);

        __TASKEXECUTOR_DEBUG("runner() END");
    }

public:
    void runAsync(std::size_t size = 1U) throw (IllegalArgumentException) {
        __TASKEXECUTOR_DEBUG_FORMAT("runAsync({})", size);
        if (size == 0) {
            throw (IllegalArgumentException());
        }

        for (std::size_t cursor = 0; cursor < size; ++cursor) {
            _threads.createThread(&TaskExecutor::runner, this);
        }
    }

    void reset()
    {
        this->clear();
        this->exit(false);

        _threads.joinAll();
        _threads.clear();
    }

    void join()
    {
        __TASKEXECUTOR_DEBUG("join() START");
        _threads.joinAll();
        __TASKEXECUTOR_DEBUG("join() END");
    }

    std::size_t getThreadCount() const noexcept
    {
        return _threads.size();
    }
};

} // namespace thread

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_THREAD_TASKEXECUTOR_HPP__

