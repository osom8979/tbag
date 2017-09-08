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
#include <libtbag/util/ProcInfo.hpp>

#include <unordered_map>
#include <vector>
#include <memory>
#include <mutex>
#include <thread>

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
        virtual void onOutRead(char const * buffer, std::size_t size) override;
        virtual void onErrRead(char const * buffer, std::size_t size) override;

    private:
        void runner();

    public:
        Err exec(std::string const & file,
                 std::vector<std::string> const & args = std::vector<std::string>(),
                 std::vector<std::string> const & envs = std::vector<std::string>(),
                 std::string const & cwd = std::string(),
                 std::string const & input = std::string());

    public:
        bool joinable() const;
        void join();
    };

    friend class Proc;

public:
    using SharedProc = std::shared_ptr<Proc>;
    using   WeakProc =   std::weak_ptr<Proc>;

    using ProcMap  = std::unordered_map<int, SharedProc>;
    using ProcPair = ProcMap::value_type;

    using Mutex = std::mutex;
    using Guard = std::lock_guard<Mutex>;

    using ProcInfo = util::ProcInfo;

private:
    mutable Mutex _mutex;
    ProcMap _procs;

public:
    ProcessManager();
    virtual ~ProcessManager();

public:
    inline bool        empty() const TBAG_NOEXCEPT_SP_OP(_procs.empty()) { Guard g(_mutex); return _procs.empty(); }
    inline std::size_t  size() const TBAG_NOEXCEPT_SP_OP(_procs.size())  { Guard g(_mutex); return _procs.size();  }

public:
    void join();

public:
    bool exists(int pid) const;
    bool isActive(int pid) const;
    std::vector<ProcInfo> list() const;

public:
    WeakProc get(int pid);
    bool erase(int pid);

public:
    int exec(std::string const & file,
             std::vector<std::string> const & args = std::vector<std::string>(),
             std::vector<std::string> const & envs = std::vector<std::string>(),
             std::string const & cwd = std::string(),
             std::string const & input = std::string());
    Err kill(int pid, int signum);

public:
    virtual void onOutRead(int pid, char const * buffer, std::size_t size) { /* EMPTY. */ }
    virtual void onErrRead(int pid, char const * buffer, std::size_t size) { /* EMPTY. */ }
    virtual void onExit(int pid, int64_t exit_status, int term_signal) { /* EMPTY. */ }
};

/**
 * FunctionalProcessManager class prototype.
 *
 * @author zer0
 * @date   2017-09-06
 */
class FunctionalProcessManager : public ProcessManager
{
public:
    using OnOutRead = std::function<void(int, char const *, std::size_t)>;
    using OnErrRead = std::function<void(int, char const *, std::size_t)>;
    using OnExit    = std::function<void(int, int64_t, int)>;

private:
    OnOutRead  _out_read_cb;
    OnErrRead  _err_read_cb;
    OnExit     _exit_cb;

public:
    FunctionalProcessManager()
    { /* EMPTY. */ }
    virtual ~FunctionalProcessManager()
    { /* EMPTY. */ }

public:
    void setOnOutRead(OnOutRead const & cb) { _out_read_cb = cb; }
    void setOnErrRead(OnErrRead const & cb) { _err_read_cb = cb; }
    void setOnExit   (OnExit    const & cb) {     _exit_cb = cb; }

public:
    virtual void onOutRead(int pid, char const * buffer, std::size_t size) override
    {
        if (static_cast<bool>(_out_read_cb)) {
            _out_read_cb(pid, buffer, size);
        }
    }

    virtual void onErrRead(int pid, char const * buffer, std::size_t size) override
    {
        if (static_cast<bool>(_err_read_cb)) {
            _err_read_cb(pid, buffer, size);
        }
    }

    virtual void onExit(int pid, int64_t exit_status, int term_signal) override
    {
        if (static_cast<bool>(_exit_cb)) {
            _exit_cb(pid, exit_status, term_signal);
        }
    }
};

} // namespace process

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PROCESS_PROCESSMANAGER_HPP__

