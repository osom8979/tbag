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
#include <libtbag/ErrPair.hpp>
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

    using OnOutRead = std::function<void(char const *, std::size_t)>;
    using OnErrRead = std::function<void(char const *, std::size_t)>;
    using OnExit    = std::function<void(int64_t, int)>;
    using OnClose   = std::function<void()>;

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
    OnOutRead  out_read_cb;
    OnErrRead  err_read_cb;
    OnExit     exit_cb;
    OnClose    close_cb;

public:
    StdProcess();
    virtual ~StdProcess();

public:
    // clang-format off
    inline bool        isRunning() const TBAG_NOEXCEPT_SP_OP(_is_running.load()) { return _is_running.load(); }
    inline int            getPid() const TBAG_NOEXCEPT { return _pid; }
    inline int64_t getExitStatus() const TBAG_NOEXCEPT { return _exit_status; }
    inline int     getTermSignal() const TBAG_NOEXCEPT { return _term_signal; }
    // clang-format on

public:
    Err spawn(Loop & loop,
              std::string const & file,
              std::vector<std::string> const & args = std::vector<std::string>(),
              std::vector<std::string> const & envs = std::vector<std::string>(),
              std::string const & cwd = std::string(),
              std::string const & input = std::string(),
              bool enable_stdout = true,
              bool enable_stderr = true);

public:
    // clang-format off
    std::string              getFile() const { return _process->atOptions().file; }
    std::vector<std::string> getArgs() const { return _process->atOptions().args; }
    std::vector<std::string> getEnvs() const { return _process->atOptions().envs; }
    std::string              getCwd () const { return _process->atOptions().cwd;  }
    // clang-format on

public:
    Err kill(int signum);

public:
    virtual void onOutRead(char const * buffer, std::size_t size);
    virtual void onErrRead(char const * buffer, std::size_t size);
    virtual void onExit(int64_t exit_status, int term_signal);
    virtual void onClose();
};

// ------------------------
// Miscellaneous utilities.
// ------------------------

TBAG_API Err subprocessCallbacks(std::string const & file = {},
                                 std::vector<std::string> const & args = {},
                                 std::vector<std::string> const & envs = {},
                                 std::string const & cwd = {},
                                 std::string const & input = {},
                                 int64_t * exit = nullptr,
                                 int * term = nullptr,
                                 StdProcess::OnOutRead const & out_cb = {},
                                 StdProcess::OnErrRead const & err_cb = {});

TBAG_API Err subprocess(std::string const & file,
                        std::vector<std::string> const & args = {},
                        std::vector<std::string> const & envs = {},
                        std::string const & cwd = {},
                        std::string const & input = {},
                        int64_t * exit = nullptr,
                        int * term = nullptr,
                        std::string * output = nullptr,
                        std::string * error = nullptr);

struct SpawnResult
{
    int64_t exit;
    int term;
    std::string output;
    std::string error;
};

using ErrSpawnResult = libtbag::ErrPair<SpawnResult>;

TBAG_API ErrSpawnResult subprocessSafe(std::string const & file,
                                       std::vector<std::string> const & args = {},
                                       std::vector<std::string> const & envs = {},
                                       std::string const & cwd = {},
                                       std::string const & input = {});

} // namespace process

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PROCESS_STDPROCESS_HPP__

