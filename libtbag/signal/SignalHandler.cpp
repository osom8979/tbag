/**
 * @file   SignalHandler.cpp
 * @brief  SignalHandler class implementation.
 * @author zer0
 * @date   2016-12-24
 */

#include <libtbag/signal/SignalHandler.hpp>
#include <libtbag/pattern/Singleton2.hpp>
#include <libtbag/debug/StackTrace.hpp>
#include <libtbag/log/Log.hpp>

#include <cassert>
#include <cstdlib>

#include <iostream>
#include <mutex>
#include <memory>
#include <map>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace signal  {

// ===============
namespace __impl {

// #define SIGHUP     1  // hangup
// #define SIGINT     2  // interrupt
// #define SIGQUIT    3  // quit
// #define SIGILL     4  // illegal instruction (not reset when caught)
// #define SIGTRAP    5  // trace trap (not reset when caught)
// #define SIGABRT    6  // abort()
// #define SIGFPE     8  // floating point exception
// #define SIGKILL    9  // kill (cannot be caught or ignored)
// #define SIGBUS    10  // bus error
// #define SIGSEGV   11  // segmentation violation
// #define SIGSYS    12  // bad argument to system call
// #define SIGPIPE   13  // write on a pipe with no one to read it
// #define SIGALRM   14  // alarm clock
// #define SIGTERM   15  // software termination signal from kill
// #define SIGURG    16  // urgent condition on IO channel
// #define SIGSTOP   17  // sendable stop signal not from tty
// #define SIGTSTP   18  // stop signal from tty
// #define SIGCONT   19  // continue a stopped process
// #define SIGCHLD   20  // to parent on child stop or exit
// #define SIGTTIN   21  // to readers pgrp upon background tty read
// #define SIGTTOU   22  // like TTIN for output if (tp->t_local&LTOSTOP)
// #define SIGXCPU   24  // exceeded CPU time limit
// #define SIGXFSZ   25  // exceeded file size limit
// #define SIGVTALRM 26  // virtual time alarm
// #define SIGPROF   27  // profiling time alarm
// #define SIGUSR1   30  // user defined signal 1
// #define SIGUSR2   31  // user defined signal 2
//
// #define SIG_DFL // The signal handler is set to default signal handler.
// #define SIG_IGN // The signal is ignored.

/**
 * Signal observable.
 *
 * @author zer0
 * @date   2016-12-24
 */
class /*NO EXPORT API*/ SignalObservable : public pattern::Singleton2<SignalObservable>
{
public:
    SINGLETON2_PROTOTYPE(SignalObservable);

public:
    using SharedHandler = std::shared_ptr<SignalHandler>;

    using Order  = int;
    using Signal = int;
    using OrderMultiMap = std::multimap<Signal, SharedHandler>;
    using SignalMap     = std::map<Signal, OrderMultiMap>;

private:
    using Mutex = std::mutex;
    using Guard = std::lock_guard<Mutex>;

private:
    SignalMap _handlers;

private:
    mutable Mutex _mutex;

public:
    bool insert(int signal, int order, SignalHandler * handler)
    {
        Guard guard(_mutex);
        if (_handlers.size() + 1 >= _handlers.max_size()) {
            return false;
        }

        auto find_itr = _handlers.find(signal);
        if (find_itr == _handlers.end()) {
            find_itr = _handlers.insert(SignalMap::value_type(signal, OrderMultiMap())).first;
        }
        find_itr->second.insert(OrderMultiMap::value_type(order, SharedHandler(handler)));

        return true;
    }

    inline SignalMap getSignalMap() const
    {
        Guard guard(_mutex);
        return _handlers;
    }

private:
    Mutex _notify;

private:
    void notifyImplement(int signal)
    {
        Guard guard(_notify);
        auto handlers = getSignalMap();
        auto find_orders = handlers.find(signal);
        if (find_orders == handlers.end()) {
            __tbag_debug("Not found [{}] signal handlers.", signal);
            return;
        }

        assert(find_orders->first == signal);
        for (auto & cursor : find_orders->second) {
            if (static_cast<bool>(cursor.second)) {
                cursor.second->run(signal);
            }
        }
    }

public:
    void notify(int signal)
    {
        notifyImplement(signal);
    }
};

/**
 * Default signal handler.
 *
 * @author zer0
 * @date   2016-12-25
 */
struct DefaultSignalHandler : public SignalHandler
{
    virtual void run(int signal) override
    {
        __tbag_debug("Signal {}:\n{}", getSignalName(signal), debug::getStackTrace());
        //std::abort(); // Don't use this abort.
        std::exit(EXIT_FAILURE);
    }
};

/**
 * Default terminate handler.
 *
 * @author zer0
 * @date   2016-12-25
 */
struct DefaultTerminateHandler : public SignalHandler
{
    virtual void run(int signal) override
    {
        __tbag_debug("Terminate signal:\n{}", debug::getStackTrace());
        //std::abort(); // Don't use this abort.
        std::exit(EXIT_FAILURE);
    }
};

} // namespace __impl
// ==================

static void __signal_dispatcher__(int signal)
{
    using Observable = __impl::SignalObservable;
    Observable * instance = Observable::getInstance();
    if (instance != nullptr) {
        instance->notify(signal);
    }
}

static void __std_terminate_dispatcher__()
{
    using Observable = __impl::SignalObservable;
    Observable * instance = Observable::getInstance();
    if (instance != nullptr) {
        instance->notify(SIGNAL_STD_TERMINATE);
    }
}

// -------------
// Main methods.
// -------------

std::string getSignalName(int signal_number)
{
    switch (signal_number) {
#define _TBAG_XX(name, signal, message) case name: return #signal;
    TBAG_SIGNAL_MAP(_TBAG_XX)
#undef _TBAG_XX
    default: return std::to_string(signal_number);
    }
}

void registerStdTerminateHandler(SignalHandler * handler, int order)
{
    using Observable = __impl::SignalObservable;
    Observable * instance = Observable::getInstance();
    if (instance != nullptr) {
        instance->insert(SIGNAL_STD_TERMINATE, order, handler);
    }
    std::set_terminate(__std_terminate_dispatcher__);
}

void registerHandler(int signal, SignalHandler * handler, int order)
{
    using Observable = __impl::SignalObservable;
    Observable * instance = Observable::getInstance();
    if (instance != nullptr) {
        instance->insert(signal, order, handler);
    }
    std::signal(signal, __signal_dispatcher__);
}

void raise(int signal)
{
    std::raise(signal);
}

void registerDefaultStdTerminateHandler()
{
    registerStdTerminateHandler(new __impl::DefaultTerminateHandler, LAST_ORDER);
}

void registerDefaultHandler()
{
    registerHandler(SIGNAL_ABORT                 , new __impl::DefaultSignalHandler, LAST_ORDER);
    registerHandler(SIGNAL_SEGMENTATION_VIOLATION, new __impl::DefaultSignalHandler, LAST_ORDER);
    registerHandler(SIGNAL_TERMINATION           , new __impl::DefaultSignalHandler, LAST_ORDER);
}

} // namespace signal

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

