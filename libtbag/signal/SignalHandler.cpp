/**
 * @file   SignalHandler.cpp
 * @brief  SignalHandler class implementation.
 * @author zer0
 * @date   2016-12-24
 */

#include <libtbag/signal/SignalHandler.hpp>
#include <libtbag/pattern/Singleton2.hpp>
#include <libtbag/log/Log.hpp>

#include <cassert>
#include <csignal>

#include <mutex>
#include <memory>
#include <map>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace signal  {

// ================
namespace details {

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

static int const SIGNAL_ABORT                    = SIGABRT; // abort()
static int const SIGNAL_FLOATING_POINT_EXCEPTION = SIGFPE;  // floating point exception
static int const SIGNAL_ILLEGAL_INSTRUCTION      = SIGILL;  // illegal instruction (not reset when caught)
static int const SIGNAL_INTERRUPT                = SIGINT;  // interrupt
static int const SIGNAL_SEGMENTATION_VIOLATION   = SIGSEGV; // segmentation violation
static int const SIGNAL_TERMINATION              = SIGTERM; // software termination signal from kill

/** Signal observable. */
class SignalObservable
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

public:
    void notify(int signal)
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
};

SINGLETON2_IMPLEMENT(SignalObservable);

} // namespace details
// ===================

static void __signalDispatcher(int signal)
{
    details::SignalObservable * instance = details::SignalObservable::getInstance();
    if (instance == nullptr) {
        __tbag_error("Not found SignalObservable instance.");
        return;
    }

    instance->notify(signal);
}

void registerHandler(int signal, int order, SignalHandler * handler)
{
    details::SignalObservable * instance = details::SignalObservable::getInstance();
    if (instance != nullptr) {
        instance->insert(signal, order, handler);
    }
    std::signal(signal, __signalDispatcher);
}

} // namespace signal

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

