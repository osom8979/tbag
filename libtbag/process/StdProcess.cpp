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
                      std::string const & input)
{
    using namespace filesystem;
    using namespace uvpp;

    Path exe_file(file);

    if (exe_file.exists() == false) {
        exe_file = findFirstUtf8ExecuteFile("^" + file + "$");
        if (exe_file.empty() == false && exe_file.exists() == false) {
            tDLogE("StdProcess::spawn({}) No such file or directory", file);
            return Err::E_ENOENT;
        }
    }

    if (exe_file.isExecutable() == false) {
        tDLogE("StdProcess::spawn({}) Permission denied", file);
        return Err::E_EACCES;
    }

    Process::Options options;
    options.clear();
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
            if (code != Err::E_SUCCESS) {
                tDLogE("StdProcess::spawn() stdin write {} error", getErrName(code));
            }
            _in->close();
        };
        Process::StdioContainer in_container(_in.get());
        in_container.setCreatePipe();
        in_container.setReadablePipe();
        options.appendStdio(in_container);
    }

    _out = loop.newHandle<FuncPipe>(loop, false);
    _out->alloc_cb = [&](std::size_t suggested_size) -> binf {
        return uvpp::defaultOnAlloc(_out_buffer, suggested_size);
    };
    _out->read_cb = [&](Err code, char const * buffer, std::size_t size){
        if (code == Err::E_SUCCESS) {
            onOutRead(buffer, size);
        } else if (code == Err::E_EOF) {
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

    _err = loop.newHandle<FuncPipe>(loop, false);
    _err->alloc_cb = [&](std::size_t suggested_size) -> binf {
        return uvpp::defaultOnAlloc(_err_buffer, suggested_size);
    };
    _err->read_cb = [&](Err code, char const * buffer, std::size_t size){
        if (code == Err::E_SUCCESS) {
            onErrRead(buffer, size);
        } else if (code == Err::E_EOF) {
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

    try {
        _process = loop.newHandle<FuncProcess>(loop, options);
    } catch (std::bad_alloc & e) {
        return Err::E_BADALLOC;
    } catch (...) {
        return Err::E_UNKEXCP;
    }

    _process->exit_cb = [&](int64_t exit_status, int term_signal){
        onExit(exit_status, term_signal);
        _process->close();
    };
    _process->close_cb = [&](){
        _is_running = false;
        onClose();
    };

    if (input.empty() == false) {
        Err const WRITE_CODE = _in->write(_write_req, input.c_str(), input.size());
        if (WRITE_CODE != Err::E_SUCCESS) {
            tDLogE("StdProcess::spawn({}) stdin write {} error", file, getErrName(WRITE_CODE));
            return Err::E_WRERR;
        }
    }

    Err const STDOUT_READ_CODE = _out->startRead();
    if (STDOUT_READ_CODE != Err::E_SUCCESS) {
        tDLogE("StdProcess::spawn({}) stdout read {} error", file, getErrName(STDOUT_READ_CODE));
        return Err::E_RDERR;
    }

    Err const STDERR_READ_CODE = _err->startRead();
    if (STDERR_READ_CODE != Err::E_SUCCESS) {
        tDLogE("StdProcess::spawn({}) stderr read {} error", file, getErrName(STDERR_READ_CODE));
        return Err::E_RDERR;
    }

    _pid = _process->getPid();
    if (_pid == 0) {
        return Err::E_UNKNOWN;
    }
    _is_running = true;
    return Err::E_SUCCESS;
}

Err StdProcess::kill(int signum)
{
    if (static_cast<bool>(_process)) {
        return _process->processKill(signum);
    }
    return Err::E_EXPIRED;
}

} // namespace process

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

