/**
 * @file   StdProcess.hpp
 * @brief  StdProcess class prototype.
 * @author zer0
 * @date   2017-09-05
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PROCESS_STDPROCESS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PROCESS_STDPROCESS_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/uvpp/func/FunctionalProcess.hpp>
#include <libtbag/uvpp/func/FunctionalPipe.hpp>
#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/uvpp/Request.hpp>

#include <cstdint>
#include <memory>
#include <atomic>
#include <vector>
#include <string>
#include <functional>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace process {

/**
 * StdProcess class prototype.
 *
 * @author zer0
 * @date   2017-09-05
 */
class TBAG_API StdProcess : private Noncopyable
{
public:
    using Loop        = uvpp::Loop;
    using binf        = uvpp::binf;
    using FuncProcess = uvpp::func::FuncProcess;
    using FuncPipe    = uvpp::func::FuncPipe;
    using WriteReq    = uvpp::WriteRequest;

    using SharedProc = std::shared_ptr<FuncProcess>;
    using SharedPipe = std::shared_ptr<FuncPipe>;

private:
    SharedProc _process;
    SharedPipe _in;
    SharedPipe _out;
    SharedPipe _err;

    WriteReq _write_req;
    std::vector<char> _out_buffer;
    std::vector<char> _err_buffer;

private:
    std::atomic_bool _is_running;

private:
    int     _pid;
    int64_t _exit_status;
    int     _term_signal;

public:
    StdProcess();
    virtual ~StdProcess();

public:
    // @formatter:off
    inline bool        isRunning() const TBAG_NOEXCEPT_SP_OP(_is_running.load()) { return _is_running.load(); }
    inline int            getPid() const TBAG_NOEXCEPT { return _pid; }
    inline int64_t getExitStatus() const TBAG_NOEXCEPT { return _exit_status; }
    inline int     getTermSignal() const TBAG_NOEXCEPT { return _term_signal; }
    // @formatter:on

public:
    // @formatter:off
    Err spawn(Loop & loop, std::string const & file);
    Err spawn(Loop & loop, std::string const & file, std::vector<std::string> const & args);
    Err spawn(Loop & loop, std::string const & file, std::vector<std::string> const & args, std::vector<std::string> const & envs);
    Err spawn(Loop & loop, std::string const & file, std::vector<std::string> const & args, std::vector<std::string> const & envs, std::string const & cwd);
    Err spawn(Loop & loop, std::string const & file, std::vector<std::string> const & args, std::vector<std::string> const & envs, std::string const & cwd, std::string const & input);
    // @formatter:on

public:
    Err kill(int signum);

public:
    virtual void onOutRead(char const * buffer, std::size_t size) { /* EMPTY. */ }
    virtual void onErrRead(char const * buffer, std::size_t size) { /* EMPTY. */ }
    virtual void onExit(int64_t exit_status, int term_signal) { /* EMPTY. */ }
    virtual void onClose() { /* EMPTY. */ }
};

/**
 * FunctionalStdProcess class prototype.
 *
 * @author zer0
 * @date   2017-09-05
 */
class FunctionalStdProcess : public StdProcess
{
public:
    using OnOutRead = std::function<void(char const *, std::size_t)>;
    using OnErrRead = std::function<void(char const *, std::size_t)>;
    using OnExit    = std::function<void(int64_t, int)>;
    using OnClose   = std::function<void()>;

private:
    OnOutRead  _out_read_cb;
    OnErrRead  _err_read_cb;
    OnExit     _exit_cb;
    OnClose    _close_cb;

public:
    FunctionalStdProcess()
    { /* EMPTY. */ }
    virtual ~FunctionalStdProcess()
    { /* EMPTY. */ }

public:
    void setOnOutRead(OnOutRead const & cb) { _out_read_cb = cb; }
    void setOnErrRead(OnErrRead const & cb) { _err_read_cb = cb; }
    void setOnExit   (OnExit    const & cb) {     _exit_cb = cb; }
    void setOnClose  (OnClose   const & cb) {    _close_cb = cb; }

public:
    virtual void onOutRead(char const * buffer, std::size_t size) override
    {
        if (static_cast<bool>(_out_read_cb)) {
            _out_read_cb(buffer, size);
        }
    }

    virtual void onErrRead(char const * buffer, std::size_t size) override
    {
        if (static_cast<bool>(_err_read_cb)) {
            _err_read_cb(buffer, size);
        }
    }

    virtual void onExit(int64_t exit_status, int term_signal) override
    {
        if (static_cast<bool>(_exit_cb)) {
            _exit_cb(exit_status, term_signal);
        }
    }

    virtual void onClose() override
    {
        if (static_cast<bool>(_close_cb)) {
            _close_cb();
        }
    }
};

} // namespace process

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PROCESS_STDPROCESS_HPP__

