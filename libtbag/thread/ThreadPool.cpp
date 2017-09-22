/**
 * @file   ThreadPool.cpp
 * @brief  ThreadPool class implementation.
 * @author zer0
 * @date   2016-10-12
 */

#include <libtbag/thread/ThreadPool.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/uvpp/UvCommon.hpp>

#include <cassert>
#include <exception>
#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace thread {

/**
 * Pointer to implementation of @c uv_thread_t.
 *
 * @author zer0
 * @date   2016-10-12
 *
 * @remarks
 *  Use the libuv.
 */
struct ThreadPool::ThreadPimpl
{
    ThreadPool * parent;
    uv_thread_t  thread;

    ThreadPimpl(ThreadPool * p) : parent(p)
    {
        assert(parent != nullptr);
        int const ERROR_CODE = ::uv_thread_create(&thread, &ThreadPimpl::globalCallback, this);
        if (ERROR_CODE != 0) {
            tDLogE("ThreadPimpl::ThreadPimpl() error[{}] {}", ERROR_CODE, getUvErrorName(ERROR_CODE));
            throw std::bad_alloc();
        }
    }

    ~ThreadPimpl()
    {
        join();
    }

    void join()
    {
        ::uv_thread_join(&thread);
    }

private:
    static void globalCallback(void * arg)
    {
        ThreadPool::ThreadPimpl * thread = static_cast<ThreadPool::ThreadPimpl*>(arg);
        assert(thread != nullptr);
        assert(thread->parent != nullptr);
        thread->parent->setUp();
        thread->parent->runner();
        thread->parent->tearDown();
    }
};

// --------------------------
// ThreadPool implementation.
// --------------------------

ThreadPool::ThreadPool(std::size_t size) : _exit(false)
{
    if (createThreads(size) == false) {
        throw std::bad_alloc();
    }
}

ThreadPool::~ThreadPool()
{
    exit();
    clearThreads();
}

bool ThreadPool::createThreads(std::size_t size)
{
    bool result = true;

    _mutex.lock();
    if (size != 0U) {
        _threads.resize(size);

        for (std::size_t i = 0; i < size; ++i) {
            _threads[i] = SharedThread(new ThreadPimpl(this));
            if (static_cast<bool>(_threads[i]) == false) {
                result = false;
                break;
            }
        }

        if (result == false) {
            tDLogE("ThreadPool::createThreads({}) ThreadPimpl constructor error.", size);
            clearThreads();
            _exit = true;
        }
        _signal.broadcast();
    } else {
        tDLogE("ThreadPool::createThreads({}) IllegalArgumentException: pool size is 0.", size);
    }
    _mutex.unlock();

    return result;
}

void ThreadPool::clearThreads()
{
    _threads.clear();
}

void ThreadPool::runner()
{
    bool is_exit   = false;
    bool find_task = true;
    SharedTask current_task;

    while (is_exit == false) {
        // WAIT SIGNAL LOOP.
        _mutex.lock();
        while (_exit == false && _task.empty()) {
            _signal.wait(_mutex);
        }
        _mutex.unlock();

        // TASK LOOP.
        find_task = true;
        while (find_task) {
            // TASK POP.
            _mutex.lock();
            if (_task.empty()) {
                find_task = false;
            } else {
                find_task = true;
                current_task = _task.front();
                _task.pop();
            }
            _mutex.unlock();

            if (find_task) {
                try {
                    if (static_cast<bool>(current_task) && static_cast<bool>(*current_task.get())) {
                        (*current_task.get())();
                    }
                } catch (...) {
                    // EMPTY.
                }
            }
        }

        // CHECK THREAD END.
        _mutex.lock();
        is_exit = _exit;
        _mutex.unlock();
    }
}

void ThreadPool::clear()
{
    _mutex.lock();
    if (_task.empty() == false) {
        _task.pop();
    }
    _mutex.unlock();
}

void ThreadPool::exit()
{
    _mutex.lock();
    _exit = true;
    _signal.broadcast();
    _mutex.unlock();
}

void ThreadPool::join()
{
    for (auto & thread : _threads) {
        assert(static_cast<bool>(thread));
        thread->join();
    }
}

bool ThreadPool::isExit() const
{
    bool result;
    _mutex.lock();
    result = _exit;
    _mutex.unlock();
    return result;
}

bool ThreadPool::push(Task const & task)
{
    bool result = false;
    _mutex.lock();
    if (_exit == false) {
        _task.push(SharedTask(new Task(task)));
        result = true;
    }
    _signal.signal();
    _mutex.unlock();
    return result;
}

bool ThreadPool::isEmptyOfThreads() const
{
    return _threads.empty();
}

bool ThreadPool::isEmptyOfTasks() const
{
    bool is_empty = false;
    _mutex.lock();
    is_empty = _task.empty();
    _mutex.unlock();
    return is_empty;
}

std::size_t ThreadPool::sizeOfThreads() const
{
    return _threads.size();
}

std::size_t ThreadPool::sizeOfTasks() const
{
    std::size_t size = 0;
    _mutex.lock();
    size = _task.size();
    _mutex.unlock();
    return size;
}

bool ThreadPool::waitTask(ThreadPool & pool, Task const & task)
{
    Mutex  mutex;
    Signal signal;
    bool   result;
    bool   task_end = false;

    result = pool.push([&](){
        task(); // RUN TASK!!

        mutex.lock();
        task_end = true;
        signal.signal();
        mutex.unlock();
    });

    if (result) {
        mutex.lock();
        while (task_end == false) {
            signal.wait(mutex);
        }
        mutex.unlock();
    }

    return result;
}

} // namespace thread

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

