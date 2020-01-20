/**
 * @file   StdProcess.cpp
 * @brief  StdProcess class implementation.
 * @author zer0
 * @date   2017-09-05
 */

#include <libtbag/process/StdProcess.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/filesystem/FindPath.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/uvpp/Loop.hpp>

#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace process {

StdProcess::StdProcess() : _is_running(false), _pid(0), _exit_status(0), _term_signal(0)
{
    // EMPTY.
}

StdProcess::~StdProcess()
{
    // EMPTY.
}

Err StdProcess::spawn(Loop & loop,
                      std::string const & file,
                      std::vector<std::string> const & args,
                      std::vector<std::string> const & envs,
                      std::string const & cwd,
                      std::string const & input,
                      bool enable_stdout,
                      bool enable_stderr)
{
    using namespace libtbag::filesystem;
    using namespace libtbag::uvpp;

    Path exe_file(file);

    if (exe_file.exists() == false) {
        exe_file = findFirstUtf8ExecuteFile("^" + file + "$");
        if (exe_file.empty() == false && exe_file.exists() == false) {
            tDLogE("StdProcess::spawn({}) No such file or directory", file);
            return E_ENOENT;
        }
    }

    if (exe_file.isExecutable() == false) {
        tDLogE("StdProcess::spawn({}) Permission denied", file);
        return E_EACCES;
    }

    Process::Options options;
    options.setFile(exe_file.toString());
    if (cwd.empty()) {
        options.setWorking(Path::getWorkDir().toString());
    } else {
        options.setWorking(cwd);
    }

    for (auto & arg : args) {
        options.appendArgument(arg);
    }
    for (auto & env : envs) {
        options.appendEnvironment(env);
    }

    if (input.empty()) {
        options.appendIgnoreStdio();
    } else {
        _in = loop.newHandle<FuncPipe>(loop, false);
        _in->write_cb = [&](WriteRequest & request, Err code){
            if (code != E_SUCCESS) {
                tDLogE("StdProcess::spawn() stdin write {} error", getErrName(code));
            }
            _in->close();
        };
        Process::StdioContainer in_container(_in.get());
        in_container.setCreatePipe();
        in_container.setReadablePipe();
        options.appendStdio(in_container);
    }

    if (enable_stdout) {
        _out = loop.newHandle<FuncPipe>(loop, false);
        _out->alloc_cb = [&](std::size_t suggested_size) -> binf {
            return uvpp::defaultOnAlloc(_out_buffer, suggested_size);
        };
        _out->read_cb = [&](Err code, char const * buffer, std::size_t size){
            if (code == E_SUCCESS) {
                onOutRead(buffer, size);
            } else if (code == E_EOF) {
                _out->close();
            } else {
                tDLogE("StdProcess::spawn() stdout read {} error", getErrName(code));
                _out->close();
            }
        };
        Process::StdioContainer out_container(_out.get());
        out_container.setCreatePipe();
        out_container.setWritablePipe();
        options.appendStdio(out_container);
    } else {
        options.appendIgnoreStdio();
    }

    if (enable_stderr) {
        _err = loop.newHandle<FuncPipe>(loop, false);
        _err->alloc_cb = [&](std::size_t suggested_size) -> binf {
            return uvpp::defaultOnAlloc(_err_buffer, suggested_size);
        };
        _err->read_cb = [&](Err code, char const * buffer, std::size_t size){
            if (code == E_SUCCESS) {
                onErrRead(buffer, size);
            } else if (code == E_EOF) {
                _err->close();
            } else {
                tDLogE("StdProcess::spawn() stderr read {} error", getErrName(code));
                _err->close();
            }
        };
        Process::StdioContainer err_container(_err.get());
        err_container.setCreatePipe();
        err_container.setWritablePipe();
        options.appendStdio(err_container);
    } else {
        options.appendIgnoreStdio();
    }

    try {
        _process = loop.newHandle<FuncProcess>(loop, options);
    } catch (std::bad_alloc & e) {
        return E_BADALLOC;
    } catch (...) {
        return E_UNKEXCP;
    }

    _process->exit_cb = [&](int64_t exit_status, int term_signal){
        _exit_status = exit_status;
        _term_signal = term_signal;
        onExit(exit_status, term_signal);
        _process->close();
    };
    _process->close_cb = [&](){
        _is_running = false;
        onClose();
    };

    if (!input.empty()) {
        auto const write_code = _in->write(_write_req, input.c_str(), input.size());
        if (write_code != E_SUCCESS) {
            tDLogE("StdProcess::spawn({}) stdin write {} error", file, write_code);
            return E_WRERR;
        }
    }

    if (enable_stdout) {
        auto const stdout_read_code = _out->startRead();
        if (stdout_read_code != E_SUCCESS) {
            tDLogE("StdProcess::spawn({}) stdout read {} error", file, stdout_read_code);
            return E_RDERR;
        }
    }

    if (enable_stderr) {
        auto const stderr_read_code = _err->startRead();
        if (stderr_read_code != E_SUCCESS) {
            tDLogE("StdProcess::spawn({}) stderr read {} error", file, stderr_read_code);
            return E_RDERR;
        }
    }

    _pid = _process->getPid();
    if (_pid == 0) {
        return E_UNKNOWN;
    }
    _is_running = true;
    return E_SUCCESS;
}

Err StdProcess::kill(int signum)
{
    if (static_cast<bool>(_process)) {
        return _process->processKill(signum);
    }
    return E_EXPIRED;
}

void StdProcess::onOutRead(char const * buffer, std::size_t size)
{
    if (out_read_cb) {
        out_read_cb(buffer, size);
    }
}

void StdProcess::onErrRead(char const * buffer, std::size_t size)
{
    if (err_read_cb) {
        err_read_cb(buffer, size);
    }
}

void StdProcess::onExit(int64_t exit_status, int term_signal)
{
    if (exit_cb) {
        exit_cb(exit_status, term_signal);
    }
}

void StdProcess::onClose()
{
    if (close_cb) {
        close_cb();
    }
}

// ------------------------
// Miscellaneous utilities.
// ------------------------

Err subprocess(std::string const & file,
               std::vector<std::string> const & args,
               std::vector<std::string> const & envs,
               std::string const & cwd,
               std::string const & input,
               int64_t * exit,
               int * term,
               std::string * output,
               std::string * error)
{
    std::string output_result;
    std::string error_result;
    int64_t exit_result = 0;
    int term_result = 0;
    bool on_close = false;

    StdProcess proc;
    proc.out_read_cb = [&](char const * buffer, std::size_t size){
        output_result.append(buffer, size);
    };
    proc.err_read_cb = [&](char const * buffer, std::size_t size){
        error_result.append(buffer, size);
    };
    proc.exit_cb = [&](int64_t exit_status, int term_signal){
        exit_result = exit_status;
        term_result = term_signal;
    };
    proc.close_cb = [&](){
        on_close = true;
    };

    libtbag::uvpp::Loop loop;
    auto const SPAWN_CODE = proc.spawn(loop, file, args, envs, cwd, input, output != nullptr, error != nullptr);
    if (isFailure(SPAWN_CODE)) {
        tDLogE("subprocess() Spawn error: {}", SPAWN_CODE);
        return SPAWN_CODE;
    }

    auto const RUN_CODE = loop.run();
    if (isFailure(RUN_CODE)) {
        tDLogE("subprocess() Loop run error: {}", RUN_CODE);
        return RUN_CODE;
    }

    assert(on_close);
    assert(isSuccess(SPAWN_CODE));
    assert(isSuccess(RUN_CODE));

    if (exit != nullptr) {
        *exit = exit_result;
    }
    if (term != nullptr) {
        *term = term_result;
    }
    if (output != nullptr) {
        *output = output_result;
    }
    if (error != nullptr) {
        *error = error_result;
    }
    return E_SUCCESS;
}

} // namespace process

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

