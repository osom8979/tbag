/**
 * @file   ThreadPool.cpp
 * @brief  ThreadPool class implementation.
 * @author zer0
 * @date   2016-10-12
 */

#include <libtbag/thread/ThreadPool.hpp>
#include <libtbag/signal/SignalHandler.hpp>
#include <libtbag/uvpp/UvCommon.hpp>
#include <libtbag/debug/StackTrace.hpp>
#include <libtbag/log/Log.hpp>

#include <cassert>
#include <iostream>
#include <string>
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
            tDLogE("ThreadPimpl::ThreadPimpl({}) error[{}] {}",
                   INDEX, ERROR_CODE, ERROR_CODE);
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
            std::signal(TBAG_SIGNAL_ILLEGAL_INSTRUCTION/*......*/, __signal_dispatcher_cb);
            std::signal(TBAG_SIGNAL_FLOATING_POINT_EXCEPTION/*.*/, __signal_dispatcher_cb);
            std::signal(TBAG_SIGNAL_SEGMENTATION_VIOLATION/*...*/, __signal_dispatcher_cb);
            std::signal(TBAG_SIGNAL_TERMINATION/*..............*/, __signal_dispatcher_cb);
            std::signal(TBAG_SIGNAL_ABORT/*....................*/, __signal_dispatcher_cb);
            std::signal(TBAG_SIGNAL_INTERRUPT/*................*/, __signal_dispatcher_cb);
        }

        // Initialize.
        thread->id = std::this_thread::get_id();
        thread->active = true;

        // Start.
        thread->parent->setUp();
        thread->parent->runner(thread->INDEX);
        thread->parent->tearDown();

        // Cleanup.
        thread->active = false;
    }

    static void __signal_dispatcher_cb(int signum)
    {
        using namespace libtbag::signal;
        using namespace libtbag::debug;
        using namespace libtbag::log;

        std::string signal_name;
        if (existSignalNumber(signum)) {
            signal_name = getSignalName(signum);
        } else {
            signal_name = std::to_string(signum);
        }

        if (existsLogger(TBAG_DEFAULT_LOGGER_NAME) && getSeverity(TBAG_DEFAULT_LOGGER_NAME) >= ALERT_SEVERITY) {
            tDLogA("__signal_dispatcher_cb({}) Signal catch!\n{}",
                   signal_name, getStackTraceString());
        } else {
            std::cerr << "__signal_dispatcher_cb(" << signal_name << ") Signal catch!\n"
                      << getStackTraceString();
        }

        exitForce(signum);
    }
};

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
            tDLogE("ThreadPool::createThreads({}) "
                   "ThreadPimpl constructor error.", size);
            _threads.clear();
            _exit = true;
        }
        _condition.broadcast();
    } else {
        tDLogE("ThreadPool::createThreads({}) "
               "IllegalArgumentException: pool size is 0.", size);
    }
    _mutex.unlock();

    if (result && wait_active) {
        std::size_t active_count = 0;

        using namespace std::chrono;
        auto const BEGIN = system_clock::now();
        // Casting to resolve 'undefined symbol errors' in constexpr.
        auto const TIMEOUT = milliseconds(static_cast<milliseconds::rep>(WAIT_TIMEOUT_MILLISEC));

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

            if (system_clock::now() - BEGIN >= TIMEOUT) {
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
                    if (current_task && *current_task.get()) {
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

