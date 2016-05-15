/**
 * @file   Task.hpp
 * @brief  Task class prototype.
 * @author zer0
 * @date   2016-05-15
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_THREAD_TASK_TASK_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_THREAD_TASK_TASK_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/Noncopyable.hpp>

#include <mutex>
#include <atomic>
#include <condition_variable>
#include <functional>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace thread {
namespace task   {

/**
 * Task class prototype.
 *
 * @author zer0
 * @date   2016-05-15
 */
class Task : public Noncopyable
{
public:
    enum Status
    {
        STATUS_READY   = 0,
        STATUS_RUNNING = 1,
    };

public:
    using Callback = std::function<void(void)>;

private:
    std::atomic_bool _exit;
    std::atomic_int _status;
    Callback _callback;

private:
    std::mutex _condition_mutex;
    std::unique_lock<std::mutex> _condition_locker;
    std::condition_variable _condition;

public:
    /**
     * @param callback  [in] functional object.
     * @param call_once [in] call once?
     */
    Task(Callback const & callback, bool call_once)
            : _exit(call_once), _status(STATUS_READY), _callback(callback)
            , _condition_mutex(), _condition_locker(_condition_mutex) {
        __EMPTY_BLOCK__
    }

    explicit Task(Callback const & callback) : Task(callback, false) {
        __EMPTY_BLOCK__
    }

    explicit Task() : Task(Callback(), false) {
        __EMPTY_BLOCK__
    }

    ~Task() {
        __EMPTY_BLOCK__
    }

public:
    void setCallback(Callback const & callback) {
        this->_callback = callback;
    }

    void setCallback(Callback && callback) {
        this->_callback.swap(callback);
    }

    inline void setExit(bool flag = true) noexcept {
        this->_exit.store(flag);
    }

    inline void exit() noexcept {
        this->setExit(true);
    }

public:
    void notify() {
        this->_condition.notify_all();
    }

public:
    void run() {
        do {
            if (static_cast<bool>(this->_callback)) {
                this->_status.store(STATUS_RUNNING);
                this->_callback();
                this->_status.store(STATUS_READY);
            }

            if (this->_exit == false) {
                this->_condition.wait(this->_condition_locker);
            }
        } while (this->_exit == false);
    }

public:
    inline bool isRunning() const noexcept {
        return (this->_status.load() == STATUS_RUNNING ? true : false);
    }

    inline bool isReady() const noexcept {
        return (this->_status.load() == STATUS_READY ? true : false);
    }
};

} // namespace task
} // namespace thread

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_THREAD_TASK_TASK_HPP__

