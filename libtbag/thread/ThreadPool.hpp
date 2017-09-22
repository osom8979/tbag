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
#include <map>

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

public:
    using SharedThread = std::shared_ptr<ThreadPimpl>;
    using ThreadGroup  = std::vector<SharedThread>;

    using Task = std::function<void(void)>;
    using SharedTask = std::shared_ptr<Task>;
    using TaskQueue = std::queue<SharedTask>;

    using Mutex  = libtbag::lock::UvLock;
    using Signal = libtbag::lock::UvCondition;

private:
    mutable Mutex _mutex;

    bool   _exit;
    Signal _signal;

    ThreadGroup _threads;
    TaskQueue   _task;

public:
    ThreadPool(std::size_t size);
    ~ThreadPool();

private:
    bool createThreads(std::size_t size);
    void clearThreads();
    void runner();

public:
    void clear();
    void exit();
    void join();
    bool isExit() const;
    bool push(Task const & task);

public:
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

