/**
 * @file   ThreadPool.hpp
 * @brief  ThreadPool class prototype.
 * @author zer0
 * @date   2016-10-12
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_THREAD_THREADPOOL_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_THREAD_THREADPOOL_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/lock/UvLock.hpp>
#include <libtbag/lock/UvCondition.hpp>

#include <functional>
#include <memory>
#include <vector>
#include <queue>
#include <atomic>
#include <exception>
#include <stdexcept>
#include <thread>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace thread {

/**
 * ThreadPool class prototype.
 *
 * @author zer0
 * @date   2016-10-12
 */
class TBAG_API ThreadPool : private Noncopyable
{
public:
    struct ThreadPimpl;
    friend struct ThreadPimpl;

public:
    TBAG_CONSTEXPR static unsigned long const WAIT_INFINITE_TIMEOUT = 0;
    TBAG_CONSTEXPR static unsigned long const WAIT_TIMEOUT_MILLISEC = 16 * 1000;

public:
    using SharedThread = std::shared_ptr<ThreadPimpl>;
    using ThreadGroup  = std::vector<SharedThread>;

    using Task = std::function<void(void)>;
    using SharedTask = std::shared_ptr<Task>;
    using TaskQueue = std::queue<SharedTask>;

    using Mutex     = libtbag::lock::UvLock;
    using Condition = libtbag::lock::UvCondition;

private:
    mutable Mutex _mutex;

    bool        _exit;
    std::size_t _active;
    Condition   _condition;

    ThreadGroup _threads;
    TaskQueue   _task;

private:
    std::exception_ptr _exception;

public:
    std::atomic_bool handling_exceptions_in_destructors;

public:
    ThreadPool(std::size_t size = 1U,
               bool wait_active = true,
               bool signal_handing = true);
    ~ThreadPool();

private:
    bool createThreads(std::size_t size, bool wait_active, bool signal_handing);
    void runner(std::size_t index);

public:
    void clear();
    void exit();
    bool isExit() const;
    bool push(Task const & task);

public:
    void join(bool rethrow = true);
    void rethrowIfExists();

public:
    bool isEmptyOfThreads() const;
    bool isEmptyOfTasks() const;

    std::size_t sizeOfThreads() const;
    std::size_t sizeOfTasks() const;
    std::size_t sizeOfActiveTasks() const;

    std::thread::id getThreadId(int i) const;

protected:
    virtual void setUp   () { /* EMPTY. */ }
    virtual void tearDown() { /* EMPTY. */ }

public:
    bool waitPush(Task const & task)
    {
        return waitTask(*this, task);
    }

public:
    static bool waitTask(ThreadPool & pool, Task const & task);
};

} // namespace thread

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_THREAD_THREADPOOL_HPP__

