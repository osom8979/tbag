/**
 * @file   TaskExecutor.cpp
 * @brief  TaskExecutor class implementation.
 * @author zer0
 * @date   2016-07-27
 */

#include <libtbag/thread/TaskExecutor.hpp>
#include <libtbag/log/Log.hpp>

#include <cassert>
#include <chrono>

//#define ENABLE_TASKEXECUTOR_DEBUG

#if defined(ENABLE_TASKEXECUTOR_DEBUG)
# define TASKEXECUTOR_DEBUG_PREFIX         "[TaskExecutor] "
# define TASKEXECUTOR_DEBUG(m)             tDLogD(TASKEXECUTOR_DEBUG_PREFIX m)
# define TASKEXECUTOR_DEBUG_FORMAT(m, ...) tDLogDF(TASKEXECUTOR_DEBUG_PREFIX m, __VA_ARGS__)
#else
# define TASKEXECUTOR_DEBUG(m)
# define TASKEXECUTOR_DEBUG_FORMAT(m, ...)
#endif

#define ENTER_TASKEXECUTOR_LOCK(mutex) TASKEXECUTOR_DEBUG("LOCK!!"); mutex.lock();
#define LEAVE_TASKEXECUTOR_LOCK(mutex) TASKEXECUTOR_DEBUG("UNLOCK"); mutex.unlock();

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
    TASKEXECUTOR_DEBUG("TaskExecutor()");
}

TaskExecutor::TaskExecutor(std::size_t size) : TaskExecutor()
{
    runAsync(size);
}

TaskExecutor::~TaskExecutor()
{
    exit();
    clear();
    join();
    TASKEXECUTOR_DEBUG("~TaskExecutor()");
}

bool TaskExecutor::push(Task const & task)
{
    SharedTask new_task;
    try {
        new_task = SharedTask(new SharedTask::element_type(task));
    } catch (...) {
        return false;
    }

    bool result = false;

    ENTER_TASKEXECUTOR_LOCK(_mutex);
    if (_exit == false) {
        _queue.push(new_task);
        result = true;
    }
    LEAVE_TASKEXECUTOR_LOCK(_mutex);

    _condition.notify_one();
    return result;
}

TaskExecutor::SharedTask TaskExecutor::pop()
{
    SharedTask result;

    ENTER_TASKEXECUTOR_LOCK(_mutex);
    if (_queue.empty() == false) {
        result = _queue.front();
        _queue.pop();
    }
    LEAVE_TASKEXECUTOR_LOCK(_mutex);

    _condition.notify_one();
    return result;
}

void TaskExecutor::clear()
{
    ENTER_TASKEXECUTOR_LOCK(_mutex);
    while (_queue.empty() == false) {
        _queue.pop();
    }
    LEAVE_TASKEXECUTOR_LOCK(_mutex);
    _condition.notify_all();
}

void TaskExecutor::exit(bool flag)
{
    ENTER_TASKEXECUTOR_LOCK(_mutex);
    _exit = flag;
    LEAVE_TASKEXECUTOR_LOCK(_mutex);
    _condition.notify_all();
}

bool TaskExecutor::isExit() const
{
    Guard guard(_mutex);
    return _exit && _queue.empty();
}

void TaskExecutor::runner()
{
    TASKEXECUTOR_DEBUG("runner() START");

    do {
        TASKEXECUTOR_DEBUG("runner() WAIT-LOCK");
        {
            std::unique_lock<std::mutex> locker(_mutex);
            while (_exit == false && _queue.empty()) {
                _condition.wait(locker);
            }
        }
        TASKEXECUTOR_DEBUG("runner() WAIT-UNLOCK");

        SharedTask current_task = pop();
        while (current_task.get() != nullptr) {
            if (static_cast<bool>(*current_task)) {
                (*current_task)();
            }
            current_task = pop();
        }
    } while (isExit() == false);

    TASKEXECUTOR_DEBUG("runner() END");
}

void TaskExecutor::runAsync(std::size_t size) throw (IllegalArgumentException)
{
    if (size == 0) {
        throw (IllegalArgumentException());
    }

    TASKEXECUTOR_DEBUG_FORMAT("runAsync({})", size);

    for (std::size_t cursor = 0; cursor < size; ++cursor) {
        std::thread * new_thread = _threads.createThread(&TaskExecutor::runner, this);
        assert(new_thread != nullptr);
    }

    // [BUG FIX]
    // Kernel release: 4.2.0-27-generic
    // Kernel version: #32~14.04.1-Ubuntu SMP Fri Jan 22 15:32:26 UTC 2016
    // Machine: x86_64
    // 이 함수가 끝나면 exit 플래그가 true로 변경되는 버그가 발견되었다.
    //{
    exit(false);
    //}
}

void TaskExecutor::reset()
{
    exit(false);
    clear();

    _threads.joinAll();
    _threads.clear();
}

void TaskExecutor::join()
{
    TASKEXECUTOR_DEBUG("join() START");
    _threads.joinAll();
    TASKEXECUTOR_DEBUG("join() END");
}

// ----------------
// EXTRA UTILITIES.
// ----------------

bool joinTask(TaskExecutor & executor, std::function<void(void)> const & task)
{
    std::mutex mutex;
    mutex.lock();

    std::thread thread;
    try {
        thread = std::thread([&](){
#if 0 // TEST CODE
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
#endif
            mutex.lock(); // FORCE WAITING.
        });
    } catch (...) {
        mutex.unlock();
        return false;
    }

    // CURRENT LOCKING COUNT: 2(Working thread) OR 1(Not working thread)

    auto const THREAD_LOCKER_UNLOCK_FUNCTOR = [&](){
        // Thread-locker it is not locked.
        while (mutex.try_lock() == true) {
            mutex.unlock();
        }
        mutex.unlock();
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
    mutex.unlock();

    return is_pushed;
}

} // namespace thread

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

