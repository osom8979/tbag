/**
 * @file   ProcessManager.cpp
 * @brief  ProcessManager class implementation.
 * @author zer0
 * @date   2017-07-29
 */

#include <libtbag/process/ProcessManager.hpp>
#include <libtbag/log/Log.hpp>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace process {

// ------------------------------
// ProcessManager implementation.
// ------------------------------

ProcessManager::ProcessManager() : ProcessManager(true)
{
    // EMPTY.
}

ProcessManager::ProcessManager(bool auto_erase) : _procs(), _auto_erase(auto_erase)
{
    // EMPTY.
}

ProcessManager::~ProcessManager()
{
    // EMPTY.
}

ProcessManager::WeakProcess ProcessManager::get(int pid)
{
    auto itr = _procs.find(pid);
    if (itr != _procs.end()) {
        return WeakProcess(itr->second);
    }
    return WeakProcess();
}

int ProcessManager::exec(Loop & loop, Options const & options)
{
    Err code = Err::E_UNKNOWN;
    int pid = 0;

    try {
        auto proc = loop.newHandle<FuncProcess>(loop, options);
        proc->setOnExit([proc](int64_t exit_status, int term_signal){
            proc->close();
        });
        proc->setOnClose([this, proc](){
            this->onExit(proc->getPid(), proc->getExitStatus(), proc->getTermSignal());
            this->_procs.erase(proc->getPid());
        });

        pid = proc->getPid();
        if (_procs.insert(ProcessPair(pid, proc)).second) {
            code = Err::E_SUCCESS;
        } else {
            code = Err::E_INSERT;
        }
    } catch (...) {
        code = Err::E_UNKEXCP;
        pid = 0;
    }

    this->onSpawn(code, pid);
    return pid;
}

Err ProcessManager::kill(int pid, int signum)
{
    return uvpp::Process::kill(pid, signum);
}

void ProcessManager::onSpawn(Err code, int pid)
{
    // EMPTY.
}

void ProcessManager::onExit(int pid, int64_t exit_status, int term_signal)
{
    // EMPTY.
}

} // namespace process

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

