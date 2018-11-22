/**
 * @file   ThreadPool.cpp
 * @brief  ThreadPool class implementation.
 * @author zer0
 * @date   2016-10-12
 */

#include <libtbag/thread/ThreadPool.hpp>
#include <libtbag/signal/SignalHandler.hpp>
#include <libtbag/thread/ThreadLocalStorage.hpp>
#include <libtbag/uvpp/UvCommon.hpp>
#include <libtbag/debug/StackTrace.hpp>
#include <libtbag/log/Log.hpp>

#include <cassert>
#include <atomic>
#include <exception>
#include <chrono>
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
public:
    using Tls = libtbag::thread::ThreadLocalStorage;

public:
    static Tls pimpl_pointer;

public:
    std::size_t const INDEX;
    bool const SIGNAL_HANDING;

public:
    ThreadPool * parent;
    uv_thread_t  thread;

    std::atomic_bool  active;
    std::thread::id   id;

public:
    ThreadPimpl(ThreadPool * p, std::size_t i, bool s)
            : INDEX(i), SIGNAL_HANDING(s), parent(p), active(false)
    {
        assert(parent != nullptr);
        int const ERROR_CODE = ::uv_thread_create(&thread, &ThreadPimpl::globalCallback, this);
        if (ERROR_CODE != 0) {
            tDLogE("ThreadPimpl::ThreadPimpl({}) error[{}] {}", INDEX, ERROR_CODE, getUvErrorName(ERROR_CODE));
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
        ThreadPimpl * thread = static_cast<ThreadPimpl*>(arg);
        assert(thread != nullptr);
        assert(thread->parent != nullptr);

        if (thread->SIGNAL_HANDING) {
            using namespace libtbag::signal;
            std::signal(TBAG_SIGNAL_ILLEGAL_INSTRUCTION, signalDispatcher);
            std::signal(TBAG_SIGNAL_FLOATING_POINT_EXCEPTION, signalDispatcher);
            std::signal(TBAG_SIGNAL_SEGMENTATION_VIOLATION, signalDispatcher);
            std::signal(TBAG_SIGNAL_TERMINATION, signalDispatcher);
            std::signal(TBAG_SIGNAL_ABORT, signalDispatcher);
        }

        // Initialize.
        thread->pimpl_pointer.set(thread);
        thread->id = std::this_thread::get_id();
        thread->active = true;

        // Start.
        thread->parent->setUp();
        thread->parent->runner(thread->INDEX);
        thread->parent->tearDown();

        // Cleanup.
        thread->active = false;
    }

    static void signalDispatcher(int signum)
    {
        ThreadPimpl * thread = pimpl_pointer.cast<ThreadPimpl>();
        assert(thread != nullptr);
        assert(thread->parent != nullptr);
        thread->parent->signal(signum);
    }
};

ThreadPool::ThreadPimpl::Tls ThreadPool::ThreadPimpl::pimpl_pointer;

// --------------------------
// ThreadPool implementation.
// --------------------------

ThreadPool::ThreadPool(std::size_t size, bool wait_active, bool signal_handing)
        : _exit(false), _active(0)
{
    if (!createThreads(size, wait_active, signal_handing)) {
        throw std::bad_alloc();
    }
}

ThreadPool::~ThreadPool()
{
    exit();
    _threads.clear();
}

bool ThreadPool::createThreads(std::size_t size, bool wait_active, bool signal_handing)
{
    bool result = true;

    _mutex.lock();
    if (size != 0U) {
        _threads.resize(size);

        for (std::size_t i = 0; i < size; ++i) {
            _threads[i] = SharedThread(new (std::nothrow) ThreadPimpl(this, i, signal_handing));
            if (!_threads[i]) {
                result = false;
                break;
            }
        }

        if (!result) {
            tDLogE("ThreadPool::createThreads({}) ThreadPimpl constructor error.", size);
            _threads.clear();
            _exit = true;
        }
        _condition.broadcast();
    } else {
        tDLogE("ThreadPool::createThreads({}) IllegalArgumentException: pool size is 0.", size);
    }
    _mutex.unlock();

    if (result && wait_active) {
        std::size_t active_count = 0;

        auto const BEGIN   = std::chrono::system_clock::now();
        auto const TIMEOUT = std::chrono::milliseconds(static_cast<unsigned long>(WAIT_TIMEOUT_MILLISEC));

        // Wait for all threads to become active.
        while (active_count != size) {
            active_count = 0;
            _mutex.lock();
            for (std::size_t i = 0; i < size; ++i) {
                assert(static_cast<bool>(_threads[i]));
                if (_threads[i]->active) {
                    ++active_count;
                }
            }
            _mutex.unlock();

            if ((WAIT_TIMEOUT_MILLISEC > WAIT_INFINITE_TIMEOUT) && (std::chrono::system_clock::now() - BEGIN) >= TIMEOUT) {
                tDLogE("ThreadPool::createThreads({}) Active wait timeout.");
                return false;
            }
        }
    }

    return result;
}

void ThreadPool::runner(std::size_t index)
{
    bool is_exit   = false;
    bool find_task = true;
    SharedTask current_task;
    std::exception_ptr exception;

    while (!is_exit) {
        // WAIT SIGNAL LOOP.
        _mutex.lock();
        while (!_exit && _task.empty()) {
            _condition.wait(_mutex);
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
                ++_active;
            }
            _mutex.unlock();

            if (find_task) {
                try {
                    if (static_cast<bool>(current_task) && static_cast<bool>(*current_task.get())) {
                        (*current_task.get())();
                    }
                } catch (...) {
                    exception = std::current_exception();
                }
            }

            _mutex.lock();
            if (find_task) {
                assert(_active >= 1);
                --_active;
            }
            if (exception && !_exception) {
                _exception = exception;
                _exit = true;
            }
            if (_exception) {
                // Forcibly terminates the current loop !!
                find_task = false;
            }
            _mutex.unlock();
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
    if (!_task.empty()) {
        _task.pop();
    }
    _mutex.unlock();
}

void ThreadPool::exit()
{
    _mutex.lock();
    _exit = true;
    _condition.broadcast();
    _mutex.unlock();
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
    if (!_exit) {
        _task.push(SharedTask(new Task(task)));
        result = true;
    }
    _condition.signal();
    _mutex.unlock();
    return result;
}

void ThreadPool::join(bool rethrow)
{
    for (auto & thread : _threads) {
        assert(static_cast<bool>(thread));
        thread->join();
    }

    if (rethrow) {
        rethrowIfExists();
    }
}

void ThreadPool::rethrowIfExists()
{
    std::exception_ptr exception;

    _mutex.lock();
    if (_exception) {
        exception = _exception;
        _exception = std::exception_ptr();
    }
    _mutex.unlock();

    if (exception) {
        std::rethrow_exception(exception);
    }
}

bool ThreadPool::isEmptyOfThreads() const
{
    return _threads.empty();
}

bool ThreadPool::isEmptyOfTasks() const
{
    bool is_empty = false;
    _mutex.lock();
    if (_task.empty() && _active == 0) {
        is_empty = true;
    }
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
    size = _task.size() + _active;
    _mutex.unlock();
    return size;
}

std::size_t ThreadPool::sizeOfActiveTasks() const
{
    std::size_t size = 0;
    _mutex.lock();
    size = _active;
    _mutex.unlock();
    return size;
}

std::thread::id ThreadPool::getThreadId(int i) const
{
    std::thread::id id;
    _mutex.lock();
    if (0 <= COMPARE_AND(i) < _threads.size() && _threads[i]) {
        id = _threads[i]->id;
    }
    _mutex.unlock();
    return id;
}

void ThreadPool::signal(int signum)
{
    using namespace libtbag::signal;
    using namespace libtbag::debug;
    if (existSignalNumber(signum)) {
        tDLogA("ThreadPool::signal({}) Signal catch!\n{}", getSignalName(signum), getStackTraceString());
    } else {
        tDLogA("ThreadPool::signal({}) Signal catch!\n{}", signum, getStackTraceString());
    }
    exitForce(signum);
}

bool ThreadPool::waitTask(ThreadPool & pool, Task const & task)
{
    Mutex mutex;
    Condition signal;

    bool result   = false;
    bool task_end = false;

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

