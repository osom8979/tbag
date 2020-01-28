/**
 * @file   ProcessManager.cpp
 * @brief  ProcessManager class implementation.
 * @author zer0
 * @date   2017-07-29
 */

#include <libtbag/process/ProcessManager.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/signal/SignalHandler.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace process {

// ------------------------------------
// ProcessManager::Proc implementation.
// ------------------------------------

ProcessManager::Proc::Proc(ProcessManager * parent) : _parent(parent)
{
    // EMPTY.
}

ProcessManager::Proc::~Proc()
{
    if (_thread.joinable()) {
        _thread.join();
    }
}

void ProcessManager::Proc::onOutRead(char const * buffer, std::size_t size)
{
    assert(_parent != nullptr);
    _parent->onOutRead(getPid(), buffer, size);
}

void ProcessManager::Proc::onErrRead(char const * buffer, std::size_t size)
{
    assert(_parent != nullptr);
    _parent->onErrRead(getPid(), buffer, size);
}

void ProcessManager::Proc::onExit(int64_t exit_status, int term_signal)
{
    assert(_parent != nullptr);
    _parent->onExit(getPid(), exit_status, term_signal);
}

void ProcessManager::Proc::runner()
{
    int const pid = getPid();
    if (pid == 0) {
        tDLogE("ProcessManager::Proc::runner() Unknown process id");
        return;
    }
    auto const code = _loop.run();
    if (isFailure(code)) {
        tDLogW("ProcessManager::Proc::runner() loop {} error (PID: {})", code, pid);
    }
}

Err ProcessManager::Proc::exec(std::string const & file,
                               std::vector<std::string> const & args,
                               std::vector<std::string> const & envs,
                               std::string const & cwd,
                               std::string const & input,
                               bool enable_stdout,
                               bool enable_stderr)
{
    Err const CODE = spawn(_loop, file, args, envs, cwd, input, enable_stdout, enable_stderr);
    if (isFailure(CODE)) {
        return CODE;
    }
    try {
        _thread = std::thread(&ProcessManager::Proc::runner, this);
    } catch (...) {
        return E_UNKEXCP;
    }
    return E_SUCCESS;
}

bool ProcessManager::Proc::joinable() const
{
    return _thread.joinable();
}

void ProcessManager::Proc::join()
{
    _thread.join();
}

// -----------------------------
// ProcessManager implementation
// -----------------------------

ProcessManager::ProcessManager()
{
    // EMPTY.
}

ProcessManager::ProcessManager(ProcessManager && obj) TBAG_NOEXCEPT
        : _procs(std::move(obj._procs)),
          out_read_cb(std::move(obj.out_read_cb)),
          err_read_cb(std::move(obj.err_read_cb)),
          exit_cb(std::move(obj.exit_cb))
{
    // EMPTY.
}

ProcessManager::~ProcessManager()
{
    // EMPTY.
}

ProcessManager & ProcessManager::operator =(ProcessManager && obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        Guard const __self_locker__(_mutex);
        Guard const __argument_locker__(obj._mutex);
        _procs = std::move(obj._procs);
        out_read_cb = std::move(obj.out_read_cb);
        err_read_cb = std::move(obj.err_read_cb);
        exit_cb = std::move(obj.exit_cb);
    }
    return *this;
}

void ProcessManager::swap(ProcessManager & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        Guard const __self_locker__(_mutex);
        Guard const __argument_locker__(obj._mutex);
        _procs.swap(obj._procs);
        out_read_cb.swap(obj.out_read_cb);
        err_read_cb.swap(obj.err_read_cb);
        exit_cb.swap(obj.exit_cb);
    }
}

bool ProcessManager::empty() const
{
    Guard const G(_mutex);
    return _procs.empty();
}

std::size_t ProcessManager::size() const
{
    Guard const G(_mutex);
    return _procs.size();
}

void ProcessManager::clear()
{
    Guard const G(_mutex);
    _procs.clear();
}

Err ProcessManager::join(int pid)
{
    Guard const G(_mutex);
    auto itr = _procs.find(pid);
    if (itr == _procs.end()) {
        return E_NFOUND;
    }
    if (!itr->second) {
        return E_EXPIRED;
    }
    if (!itr->second->joinable()) {
        return E_ILLSTATE;
    }
    itr->second->join();
    return E_SUCCESS;
}

void ProcessManager::joinAll()
{
    Guard const G(_mutex);
    for (auto & proc : _procs) {
        if (proc.second && proc.second->joinable()) {
            proc.second->join();
        }
    }
}

bool ProcessManager::exists(int pid) const
{
    Guard g(_mutex);
    return _procs.find(pid) != _procs.end();
}

bool ProcessManager::isActive(int pid) const
{
    Guard g(_mutex);
    auto itr = _procs.find(pid);
    if (itr != _procs.end() && itr->second) {
        return itr->second->isRunning();
    }
    return false;
}

std::vector<ProcessManager::ProcessInfo> ProcessManager::list() const
{
    std::vector<ProcessInfo> result;
    Guard g(_mutex);
    for (auto & proc : _procs) {
        if (static_cast<bool>(proc.second)) {
            ProcessInfo info;
            info.pid = proc.second->getPid();
            info.active = proc.second->isRunning();
            info.infos.emplace_back(util::PROCESS_INFO_KEY_FILE  , proc.second->getFile());
            info.infos.emplace_back(util::PROCESS_INFO_KEY_ARGS  , string::mergeTokens(proc.second->getArgs(), util::PROCESS_INFO_VAL_DELIMITER));
            info.infos.emplace_back(util::PROCESS_INFO_KEY_ENVS  , string::mergeTokens(proc.second->getEnvs(), util::PROCESS_INFO_VAL_DELIMITER));
            info.infos.emplace_back(util::PROCESS_INFO_KEY_CWD   , proc.second->getCwd());
            info.infos.emplace_back(util::PROCESS_INFO_KEY_EXIT  , std::to_string(proc.second->getExitStatus()));
            info.infos.emplace_back(util::PROCESS_INFO_KEY_SIGNUM, std::to_string(proc.second->getTermSignal()));
            result.push_back(std::move(info));
        }
    }
    return result;
}

ProcessManager::WeakProc ProcessManager::get(int pid)
{
    Guard g(_mutex);
    auto itr = _procs.find(pid);
    if (itr != _procs.end()) {
        return WeakProc(itr->second);
    }
    return WeakProc();
}

bool ProcessManager::erase(int pid)
{
    Guard g(_mutex);
    return _procs.erase(pid) == 1u;
}

int ProcessManager::exec(std::string const & file,
                         std::vector<std::string> const & args,
                         std::vector<std::string> const & envs,
                         std::string const & cwd,
                         std::string const & input,
                         bool enable_stdout,
                         bool enable_stderr)
{
    Guard g(_mutex);
    SharedProc proc(new Proc(this));
    Err const SPAWN_CODE = proc->exec(file, args, envs, cwd, input, enable_stdout, enable_stderr);
    if (isFailure(SPAWN_CODE)) {
        tDLogE("ProcessManager::spawn() process spawn error.", getErrName(SPAWN_CODE));
        return 0;
    }

    int const PID = proc->getPid();
    if (!_procs.insert(ProcPair(PID, proc)).second) {
        tDLogE("ProcessManager::spawn() Insert error.");
        return 0;
    }
    return PID;
}

Err ProcessManager::kill(int pid, int signum)
{
    Guard const g(_mutex);
    auto itr = _procs.find(pid);
    if (itr == _procs.end()) {
        return E_NFOUND;
    }
    if (!itr->second) {
        return E_EXPIRED;
    }
    if (!itr->second->isRunning()) {
        return E_ILLSTATE;
    }
    return itr->second->kill(signum);
}

void ProcessManager::killAll(int signum)
{
    Guard const G(_mutex);
    for (auto & proc : _procs) {
        if (proc.second && proc.second->isRunning()) {
            proc.second->kill(signum);
        }
    }
}

void ProcessManager::killAll()
{
    return killAll(libtbag::signal::TBAG_SIGNAL_TERMINATION);
}

void ProcessManager::onOutRead(int pid, char const * buffer, std::size_t size)
{
    if (out_read_cb) {
        out_read_cb(pid, buffer, size);
    }
}

void ProcessManager::onErrRead(int pid, char const * buffer, std::size_t size)
{
    if (err_read_cb) {
        err_read_cb(pid, buffer, size);
    }
}

void ProcessManager::onExit(int pid, int64_t exit_status, int term_signal)
{
    if (exit_cb) {
        exit_cb(pid, exit_status, term_signal);
    }
}

} // namespace process

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

