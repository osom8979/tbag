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
#include <libtbag/string/Environments.hpp>
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

Err subprocessCallbacks(std::string const & file,
                        std::vector<std::string> const & args,
                        std::vector<std::string> const & envs,
                        std::string const & cwd,
                        std::string const & input,
                        int64_t * exit,
                        int * term,
                        StdProcess::OnOutRead const & out_cb,
                        StdProcess::OnErrRead const & err_cb)
{
    if (file.empty()) {
        return E_ILLARGS;
    }

    auto const file_path = libtbag::filesystem::Path(file).getCanonical();
    if (!file_path.isRegularFile()) {
        return E_ENOENT;
    }
    if (!file_path.isExecutable()) {
        return E_EACCES;
    }

    int64_t exit_result = 0;
    int term_result = 0;
    bool on_close = false;

    StdProcess proc;
    proc.out_read_cb = out_cb;
    proc.err_read_cb = err_cb;
    proc.exit_cb = [&](int64_t exit_status, int term_signal){
      exit_result = exit_status;
      term_result = term_signal;
    };
    proc.close_cb = [&](){
      on_close = true;
    };

    libtbag::uvpp::Loop loop;
    auto const spawn_code = proc.spawn(loop, file, args, envs, cwd, input,
                                       static_cast<bool>(out_cb),
                                       static_cast<bool>(err_cb));
    if (isFailure(spawn_code)) {
        tDLogE("subprocess() Spawn error: {}", spawn_code);
        return spawn_code;
    }

    auto const run_code = loop.run();
    if (isFailure(run_code)) {
        tDLogE("subprocess() Loop run error: {}", run_code);
        return run_code;
    }

    assert(on_close);
    assert(isSuccess(spawn_code));
    assert(isSuccess(run_code));

    if (exit != nullptr) {
        *exit = exit_result;
    }
    if (term != nullptr) {
        *term = term_result;
    }
    return E_SUCCESS;
}

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

    auto const out_read_cb = [&](char const * buffer, std::size_t size){
        output_result.append(buffer, size);
    };
    auto const err_read_cb = [&](char const * buffer, std::size_t size){
        error_result.append(buffer, size);
    };

    auto const code = subprocessCallbacks(file, args, envs, cwd, input,
                                          exit, term,
                                          out_read_cb, err_read_cb);
    if (isFailure(code)) {
        return code;
    }

    if (output != nullptr) {
        *output = output_result;
    }
    if (error != nullptr) {
        *error = error_result;
    }
    return E_SUCCESS;
}

ErrSpawnResult subprocessSafe(std::string const & file,
                              std::vector<std::string> const & args,
                              std::vector<std::string> const & envs,
                              std::string const & cwd,
                              std::string const & input)
{
    std::vector<std::string> updated_envs;
    if (envs.empty()) {
        updated_envs = libtbag::string::Environments::createDefaultEnvironments().toStrings();
    } else {
        updated_envs = envs;
    }

    std::string updated_cwd;
    if (cwd.empty()) {
        updated_cwd = libtbag::filesystem::Path::getWorkDir();
    } else {
        updated_cwd = cwd;
    }

    SpawnResult result;
    auto const code = subprocess(file, args, updated_envs, updated_cwd, input,
                                 &result.exit,
                                 &result.term,
                                 &result.output,
                                 &result.error);
    if (isFailure(code)) {
        return code;
    }
    return { E_SUCCESS, result };
}

} // namespace process

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

