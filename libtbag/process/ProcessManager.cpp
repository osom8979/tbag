/**
 * @file   ProcessManager.cpp
 * @brief  ProcessManager class implementation.
 * @author zer0
 * @date   2017-07-29
 */

#include <libtbag/process/ProcessManager.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/signal/SignalHandler.hpp>
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

void ProcessManager::Proc::runner()
{
    int const PID = getPid();
    if (PID == 0) {
        tDLogE("ProcessManager::Proc::runner() Unknown process id");
        return;
    }

    Err const CODE = _loop.run();
    if (TBAG_ERR_FAILURE(CODE)) {
        tDLogW("ProcessManager::Proc::runner() loop {} error (PID: {})", getErrName(CODE), PID);
    }

    assert(_parent != nullptr);
    _parent->onExit(PID, getExitStatus(), getTermSignal());
}

Err ProcessManager::Proc::exec(std::string const & file,
                               std::vector<std::string> const & args,
                               std::vector<std::string> const & envs,
                               std::string const & cwd,
                               std::string const & input)
{
    Err const CODE = spawn(_loop, file, args, envs, cwd, input);
    if (TBAG_ERR_FAILURE(CODE)) {
        return CODE;
    }
    try {
        _thread = std::thread(&ProcessManager::Proc::runner, this);
    } catch (...) {
        return Err::E_UNKEXCP;
    }
    return Err::E_SUCCESS;
}

bool ProcessManager::Proc::joinable() const
{
    return _thread.joinable();
}

void ProcessManager::Proc::join()
{
    _thread.join();
}

// ------------------------------
// ProcessManager implementation.
// ------------------------------

ProcessManager::ProcessManager()
{
    // EMPTY.
}

ProcessManager::~ProcessManager()
{
    Guard g(_mutex);
    for (auto & proc : _procs) {
        if (static_cast<bool>(proc.second) && proc.second->isRunning()) {
            proc.second->kill(signal::TBAG_SIGNAL_TERMINATION);
        }
    }
    _procs.clear();
}

void ProcessManager::join()
{
    tDLogD("ProcessManager::join() BEGIN");
    Guard g(_mutex);
    for (auto & proc : _procs) {
        if (static_cast<bool>(proc.second) && proc.second->joinable()) {
            proc.second->join();
        }
    }
    tDLogD("ProcessManager::join() END");
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
    if (itr != _procs.end() && static_cast<bool>(itr->second)) {
        return itr->second->isRunning();
    }
    return false;
}

std::vector<int> ProcessManager::list() const
{
    std::vector<int> result;
    Guard g(_mutex);
    for (auto & proc : _procs) {
        result.push_back(proc.first);
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
    return _procs.erase(pid) == 1U;
}

int ProcessManager::exec(std::string const & file,
                         std::vector<std::string> const & args,
                         std::vector<std::string> const & envs,
                         std::string const & cwd,
                         std::string const & input)
{
    Guard g(_mutex);
    SharedProc proc(new Proc(this));
    Err const SPAWN_CODE = proc->exec(file, args, envs, cwd, input);
    if (TBAG_ERR_FAILURE(SPAWN_CODE)) {
        tDLogE("ProcessManager::spawn() process spawn error.", getErrName(SPAWN_CODE));
        return 0;
    }

    int const PID = proc->getPid();
    if (_procs.insert(ProcPair(PID, proc)).second == false) {
        tDLogE("ProcessManager::spawn() Insert error.");
        return 0;
    }
    return PID;
}

Err ProcessManager::kill(int pid, int signum)
{
    Guard g(_mutex);
    auto itr = _procs.find(pid);
    if (itr != _procs.end()) {
        if (static_cast<bool>(itr->second)) {
            if (itr->second->isRunning()) {
                return itr->second->kill(signum);
            } else {
                return Err::E_ILLSTATE;
            }
        } else {
            return Err::E_EXPIRED;
        }
    }
    return Err::E_NOTFOUND;
}

} // namespace process

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

