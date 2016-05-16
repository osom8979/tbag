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

#include <queue>
#include <memory>
#include <chrono>
#include <atomic>
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
 */
class TaskExecutor : public Noncopyable
{
public:
    using Task       = std::function<bool(void)>;
    using SharedTask = std::shared_ptr<Task>;
    using TaskQueue  = std::queue<SharedTask>;

public:
    using Mutex  = std::mutex;
    using Signal = std::condition_variable;

private:
    std::atomic_bool _exit;
    std::atomic_int _wait_count;
    ThreadGroup _threads;

private:
    std::mutex _thread_mutex;
    Signal _thread_condition;

public:
    TaskExecutor() : _exit(false), _wait_count(0) {
        __EMPTY_BLOCK__
    }

    ~TaskExecutor() {
        this->clear();
        this->exit();
        this->join();
    }

private:
    mutable std::mutex _queue_lock;
    TaskQueue _queue;

public:
    bool push(Task const & task) {
        if (_exit.load()) {
            return false;
        }

        std::lock_guard<std::mutex> guard(this->_queue_lock);
        this->_queue.push(SharedTask(new (std::nothrow) Task(task)));
        this->_thread_condition.notify_all();
        return true;
    }

    SharedTask pop() {
        std::lock_guard<std::mutex> guard(this->_queue_lock);
        if (this->_queue.empty()) {
            return SharedTask();
        }

        SharedTask result = this->_queue.front();
        this->_queue.pop();
        this->_thread_condition.notify_all();
        return result;
    }

    void clear() {
        std::lock_guard<std::mutex> guard(this->_queue_lock);
        while (this->_queue.empty() == false) {
            this->_queue.pop();
        }
        this->_thread_condition.notify_all();
    }

    std::size_t sizeOfQueue() const {
        std::lock_guard<std::mutex> guard(this->_queue_lock);
        return this->_queue.size();
    }

private:
    void runner() {
        std::unique_lock<std::mutex> locker(this->_thread_mutex);
        SharedTask current_task;

        do {
            // IMPORTANT: Don't change condition check order.
            if (this->sizeOfQueue() == 0 && this->_exit.load() == false) {
                this->_thread_condition.wait_for(locker, std::chrono::milliseconds(1));
            }

            current_task = this->pop();
            while (current_task.get() != nullptr) {
                if (static_cast<bool>(*current_task) == true) {
                    if ((*current_task)() == false) {
                        // FORCE EXIT.
                        return;
                    }
                }
                current_task = this->pop();
            }
        } while (this->_exit.load() == false);
    }

public:
    void runAsync(std::size_t size = 1U) throw (IllegalArgumentException) {
        if (size == 0) {
            throw (IllegalArgumentException());
        }

        for (std::size_t cursor = 0; cursor < size; ++cursor) {
            this->_threads.createThread(&TaskExecutor::runner, this);
        }
    }

    void reset() {
        this->clear();
        this->setExit(false);

        this->_threads.joinAll();
        this->_threads.clear();
    }

    void setExit(bool flag = true) {
        this->_exit.store(flag);
        this->_thread_condition.notify_all();
    }

    void exit() {
        this->setExit(true);
    }

    void join() {
        this->_threads.joinAll();
    }

    std::size_t getThreadCount() const noexcept {
        return this->_threads.size();
    }
};

} // namespace thread

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_THREAD_TASKEXECUTOR_HPP__

