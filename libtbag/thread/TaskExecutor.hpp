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

#include <map>
#include <queue>
#include <memory>
#include <mutex>
#include <functional>
#include <condition_variable>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace thread {

/**
 * TaskExecutor class prototype.
 *
 * @author zer0
 * @date   2016-05-16
 *
 * @warning
 *  It has a potential bug.
 *
 * @deprecated
 */
class TaskExecutor : public Noncopyable
{
public:
    using Task       = std::function<void(void)>;
    using SharedTask = std::shared_ptr<Task>;
    using TaskQueue  = std::queue<SharedTask>;

public:
    using Mutex  = std::mutex;
    using Guard  = std::lock_guard<Mutex>;
    using Signal = std::condition_variable;

private:
    ThreadGroup _threads;

private:
    mutable Mutex _mutex;
    Signal    _condition;
    TaskQueue _queue;
    bool      _exit;

public:
    TaskExecutor();
    TaskExecutor(std::size_t size);
    ~TaskExecutor();

public:
    bool push(Task const & task);
    SharedTask pop();
    void clear();

public:
    void exit(bool flag = true);
    bool isExit() const;

public:
    inline std::size_t size() const
    { Guard g(_mutex); return _queue.size(); }
    inline bool empty() const
    { Guard g(_mutex); return _queue.empty(); }

private:
    void runner();

public:
    void runAsync(std::size_t size = 1U) throw (IllegalArgumentException);
    void reset();
    void join();

public:
    inline std::size_t getThreadCount() const noexcept
    { return _threads.size(); }
};

// ----------------
// EXTRA UTILITIES.
// ----------------

/**
 * Synchronized task process.
 *
 * @warning
 *  Don't use the @c std::condition_variable class.
 */
bool joinTask(TaskExecutor & executor, std::function<void(void)> const & task);

} // namespace thread

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_THREAD_TASKEXECUTOR_HPP__

