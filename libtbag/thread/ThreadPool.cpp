/**
 * @file   ThreadPool.cpp
 * @brief  ThreadPool class implementation.
 * @author zer0
 * @date   2016-10-12
 */

#include <libtbag/thread/ThreadPool.hpp>
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
    using Callback = std::function<void(void)>;

    uv_thread_t thread;
    Callback  callback;

    ThreadPimpl(Callback const & c) : callback(c)
    {
        int code = ::uv_thread_create(&thread, &ThreadPimpl::globalCallback, this);
        if (code != 0) {
            throw InitializeException(code);
        }
    }

    ~ThreadPimpl()
    {
        ::uv_thread_join(&thread);
    }

private:
    static void globalCallback(void * arg)
    {
        static_cast<ThreadPool::ThreadPimpl*>(arg)->callback();
    }
};

// --------------------------
// ThreadPool implementation.
// --------------------------

ThreadPool::ThreadPool(std::size_t size) throw(IllegalArgumentException, InitializeException) : _exit(false)
{
    if (size == 0U) {
        throw IllegalArgumentException();
    }

    while (size > 0) {
        _pool.push(SharedThread(new ThreadPimpl([this](){ this->runner(); })));
        --size;
    }
}

ThreadPool::~ThreadPool()
{
    exit();
    while (_pool.empty() == false) {
        _pool.pop();
    }
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
                    if (static_cast<bool>(current_task) && static_cast<bool>(current_task.get())) {
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

