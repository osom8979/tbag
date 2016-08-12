/**
 * @file   TaskExecutor.cpp
 * @brief  TaskExecutor class implementation.
 * @author zer0
 * @date   2016-07-27
 */

#include <libtbag/thread/TaskExecutor.hpp>
#include <libtbag/log/Log.hpp>
#include <chrono>

//#define ENABLE_TASKEXECUTOR_DEBUG

#if defined(ENABLE_TASKEXECUTOR_DEBUG)
# define __TASKEXECUTOR_DEBUG_PREFIX         "[TaskExecutor] "
# define __TASKEXECUTOR_DEBUG(m)             tDLogD(__TASKEXECUTOR_DEBUG_PREFIX m)
# define __TASKEXECUTOR_DEBUG_FORMAT(m, ...) tDLogDF(__TASKEXECUTOR_DEBUG_PREFIX m, __VA_ARGS__)
#else
# define __TASKEXECUTOR_DEBUG(m)
# define __TASKEXECUTOR_DEBUG_FORMAT(m, ...)
#endif

#define ENTER_TASKEXECUTOR_LOCK(mutex) __TASKEXECUTOR_DEBUG("LOCK!!"); mutex.lock();
#define LEAVE_TASKEXECUTOR_LOCK(mutex) __TASKEXECUTOR_DEBUG("UNLOCK"); mutex.unlock();

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace thread {

TaskExecutor::TaskExecutor() : _exit(false)
{
#if defined(ENABLE_TASKEXECUTOR_DEBUG)
    if (log::getDefaultLogger() == nullptr) {
        log::createDefaultColorConsoleLogger();
        log::setDefaultLevel(log::LogLevel::LEVEL_DEBUG);
    }
#endif
    __TASKEXECUTOR_DEBUG("TaskExecutor()");
}

TaskExecutor::TaskExecutor(std::size_t size) : TaskExecutor()
{
    this->runAsync(size);
}

TaskExecutor::~TaskExecutor()
{
    this->exit();
    this->clear();
    this->join();
    __TASKEXECUTOR_DEBUG("~TaskExecutor()");
}

void TaskExecutor::setWaitCallback(WaitCallback const & callback)
{
    std::lock_guard<std::mutex> guard(_locker);
    _wait_callback = callback;
}

bool TaskExecutor::push(Task const & task)
{
    SharedTask new_task;
    try {
        new_task = SharedTask(new Task(task));
    } catch (...) {
        return false;
    }

    bool result = false;

    ENTER_TASKEXECUTOR_LOCK(_locker);
    if (_exit == false) {
        _queue.push(new_task);
        result = true;
    }
    LEAVE_TASKEXECUTOR_LOCK(_locker);

    _condition.notify_one();
    return result;
}

TaskExecutor::SharedTask TaskExecutor::pop()
{
    SharedTask result;

    ENTER_TASKEXECUTOR_LOCK(_locker);
    if (_queue.empty() == false) {
        result = this->_queue.front();
        _queue.pop();
    }
    LEAVE_TASKEXECUTOR_LOCK(_locker);

    _condition.notify_one();
    return result;
}

void TaskExecutor::clear()
{
    ENTER_TASKEXECUTOR_LOCK(_locker);
    while (_queue.empty() == false) {
        _queue.pop();
    }
    LEAVE_TASKEXECUTOR_LOCK(_locker);
    _condition.notify_all();
}

void TaskExecutor::exit(bool flag)
{
    ENTER_TASKEXECUTOR_LOCK(_locker);
    _exit = flag;
    LEAVE_TASKEXECUTOR_LOCK(_locker);
    _condition.notify_all();
}

bool TaskExecutor::isExit() const
{
    std::lock_guard<std::mutex> guard(_locker);
    return _exit && _queue.empty();
}

std::size_t TaskExecutor::sizeOfQueue() const
{
    std::lock_guard<std::mutex> guard(_locker);
    return _queue.size();
}

bool TaskExecutor::emptyOfQueue() const
{
    std::lock_guard<std::mutex> guard(_locker);
    return _queue.empty();
}

void TaskExecutor::runner()
{
    _locker.lock();
    _state.insert(std::make_pair(std::this_thread::get_id(), false));
    _locker.unlock();
    __TASKEXECUTOR_DEBUG_FORMAT("runner() @{} START", std::this_thread::get_id());

    do {
        __TASKEXECUTOR_DEBUG_FORMAT("runner() @{} LOCK", std::this_thread::get_id());
        {
            std::unique_lock<std::mutex> locker(_locker);
            while (_exit == false && _queue.empty()) {
                _state.find(std::this_thread::get_id())->second = true;
                if (static_cast<bool>(_wait_callback)) {
                    _wait_callback(_state);
                }
                _condition.wait(locker);
                _state.find(std::this_thread::get_id())->second = false;
            }
        }
        __TASKEXECUTOR_DEBUG_FORMAT("runner() @{} UNLOCK", std::this_thread::get_id());

        SharedTask current_task = this->pop();
        while (current_task.get() != nullptr) {
            if (static_cast<bool>(*current_task)) {
                (*current_task)();
            }
            current_task = this->pop();
        }
    } while (isExit() == false);

    __TASKEXECUTOR_DEBUG_FORMAT("runner() @{} END", std::this_thread::get_id());
    _locker.lock();
    _state.erase(std::this_thread::get_id());
    _locker.unlock();
}

void TaskExecutor::runAsync(std::size_t size) throw (IllegalArgumentException)
{
    __TASKEXECUTOR_DEBUG_FORMAT("runAsync({})", size);
    if (size == 0) {
        throw (IllegalArgumentException());
    }

    for (std::size_t cursor = 0; cursor < size; ++cursor) {
        _threads.createThread(&TaskExecutor::runner, this);
    }
}

void TaskExecutor::reset()
{
    this->exit(false);
    this->clear();

    _threads.joinAll();
    _threads.clear();
}

void TaskExecutor::join()
{
    __TASKEXECUTOR_DEBUG("join() START");
    _threads.joinAll();
    __TASKEXECUTOR_DEBUG("join() END");
}

std::size_t TaskExecutor::getThreadCount() const noexcept
{
    return _threads.size();
}

// ----------------
// EXTRA UTILITIES.
// ----------------

bool joinTask(TaskExecutor & executor, std::function<void(void)> const & task)
{
    std::mutex locker;
    locker.lock();

    std::thread thread;
    try {
        thread = std::thread([&](){
#if 0 // TEST CODE
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
#endif
            locker.lock(); // FORCE WAITING.
        });
    } catch (...) {
        locker.unlock();
        return false;
    }

    // CURRENT LOCKING COUNT: 2(Working thread) OR 1(Not working thread)

    auto const THREAD_LOCKER_UNLOCK_FUNCTOR = [&](){
        // Thread-locker it is not locked.
        while (locker.try_lock() == true) {
            locker.unlock();
        }
        locker.unlock();
    };

    bool is_pushed = executor.push([&](){
        task();
        THREAD_LOCKER_UNLOCK_FUNCTOR(); // If the push is successful.
    });
    if (is_pushed == false) {
        THREAD_LOCKER_UNLOCK_FUNCTOR(); // If the push is failed.
    }

    assert(thread.joinable());
    thread.join();
    locker.unlock();

    return is_pushed;
}

} // namespace thread

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

