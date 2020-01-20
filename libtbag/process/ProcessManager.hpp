/**
 * @file   ProcessManager.hpp
 * @brief  ProcessManager class prototype.
 * @author zer0
 * @date   2017-07-29
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PROCESS_PROCESSMANAGER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PROCESS_PROCESSMANAGER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/Err.hpp>

#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/process/StdProcess.hpp>
#include <libtbag/signal/SignalHandler.hpp>
#include <libtbag/util/Structures.hpp>

#include <unordered_map>
#include <vector>
#include <memory>
#include <mutex>
#include <thread>
#include <functional>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace process {

/**
 * ProcessManager class prototype.
 *
 * @author zer0
 * @date   2017-07-29
 */
class TBAG_API ProcessManager : private Noncopyable
{
public:
    using Loop = uvpp::Loop;

    /**
     * Proc class prototype.
     *
     * @author zer0
     * @date   2017-09-06
     */
    class TBAG_API Proc : public StdProcess
    {
    private:
        ProcessManager * _parent;

    private:
        std::thread _thread;
        Loop _loop;

    public:
        Proc(ProcessManager * parent);
        virtual ~Proc();

    protected:
        void onOutRead(char const * buffer, std::size_t size) override;
        void onErrRead(char const * buffer, std::size_t size) override;
        void onExit(int64_t exit_status, int term_signal) override;

    private:
        void runner();

    public:
        Err exec(std::string const & file,
                 std::vector<std::string> const & args = std::vector<std::string>(),
                 std::vector<std::string> const & envs = std::vector<std::string>(),
                 std::string const & cwd = std::string(),
                 std::string const & input = std::string(),
                 bool enable_stdout = true,
                 bool enable_stderr = true);

    public:
        bool joinable() const;
        void join();
    };

    friend class Proc;

public:
    using SharedProc = std::shared_ptr<Proc>;
    using WeakProc   = std::weak_ptr<Proc>;

    using ProcMap  = std::unordered_map<int, SharedProc>;
    using ProcPair = ProcMap::value_type;

    using Mutex = std::mutex;
    using Guard = std::lock_guard<Mutex>;

    using ProcessInfo = util::ProcessInfo;

    using OnOutRead = std::function<void(int, char const *, std::size_t)>;
    using OnErrRead = std::function<void(int, char const *, std::size_t)>;
    using OnExit    = std::function<void(int, int64_t, int)>;

private:
    Mutex mutable _mutex;
    ProcMap _procs;

public:
    OnOutRead out_read_cb;
    OnErrRead err_read_cb;
    OnExit exit_cb;

public:
    ProcessManager();
    virtual ~ProcessManager();

public:
    bool empty() const;
    std::size_t size() const;

public:
    void clear();
    Err join(int pid);
    void joinAll();

public:
    bool exists(int pid) const;
    bool isActive(int pid) const;
    std::vector<ProcessInfo> list() const;

public:
    WeakProc get(int pid);
    bool erase(int pid);

public:
    int exec(std::string const & file,
             std::vector<std::string> const & args = std::vector<std::string>(),
             std::vector<std::string> const & envs = std::vector<std::string>(),
             std::string const & cwd = std::string(),
             std::string const & input = std::string(),
             bool enable_stdout = true,
             bool enable_stderr = true);
    Err kill(int pid, int signum);
    void killAll(int signum);
    void killAll();

public:
    virtual void onOutRead(int pid, char const * buffer, std::size_t size);
    virtual void onErrRead(int pid, char const * buffer, std::size_t size);
    virtual void onExit(int pid, int64_t exit_status, int term_signal);
};

} // namespace process

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PROCESS_PROCESSMANAGER_HPP__

