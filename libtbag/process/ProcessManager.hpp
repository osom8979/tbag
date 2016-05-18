/**
 * @file   ProcessManager.hpp
 * @brief  ProcessManager class prototype.
 * @author zer0
 * @date   2016-05-17
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PROCESS_PROCESSMANAGER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PROCESS_PROCESSMANAGER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/pattern/Singleton.hpp>
#include <libtbag/process/Process.hpp>

#include <list>
#include <string>
#include <mutex>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace process {

/**
 * ProcessManager class prototype.
 *
 * @author zer0
 * @date   2016-05-17
 */
class ProcessManager : SINGLETON_INHERITANCE(ProcessManager)
{
public:
    SINGLETON_RESTRICT(ProcessManager);

public:
    using SharedProcess = std::shared_ptr<Process>;
    using ProcessList = std::list<SharedProcess>;

private:
    std::mutex _list_lock;
    ProcessList _list;

private:
    SharedProcess get(uv_handle_t * handle) {
        std::lock_guard<std::mutex> guard(_list_lock);
        for (ProcessList::iterator itr = _list.begin(), end = _list.end(); itr != end; ++itr) {
            if (static_cast<bool>(*itr) && (*itr)->isSameHandle(handle)) {
                return (*itr);
            }
        }
        return SharedProcess();
    }

    SharedProcess get(uv_process_t * process) {
        return get((uv_handle_t*)process);
    }

    SharedProcess createProcess() {
        std::lock_guard<std::mutex> guard(_list_lock);
        SharedProcess process = SharedProcess(new (std::nothrow) Process());
        this->_list.push_back(process);
        return process;
    }

    void removeProcess(SharedProcess process) {
        std::lock_guard<std::mutex> guard(_list_lock);
        for (ProcessList::iterator itr = _list.begin(), end = _list.end(); itr != end; ++itr) {
            if (static_cast<bool>(*itr) && (*itr)->isSameHandle(&process->_process)) {
                this->_list.erase(itr);
                return;
            }
        }
    }

private:
    static void onClose(uv_handle_t * handle) {
        SharedProcess process = ProcessManager::getInstance()->get(handle);
        if (static_cast<bool>(process)) {
            process->onClose();
        }
    }

    static void onExit(uv_process_t * handle, int64_t exit_status, int signal) {
        SharedProcess process = ProcessManager::getInstance()->get(handle);
        if (static_cast<bool>(process)) {
            process->onExit(exit_status, signal);
        }
        uv_close((uv_handle_t*)handle, &ProcessManager::onClose);
    }

public:
    ProcessResult run(std::string              const & exe
                    , std::vector<std::string> const & args
                    , std::string              const & work) {

        SharedProcess process = createProcess();
        process->setExecuteFile(exe);
        process->setArguments(args);
        process->setWorkingDirectory(work);

        process->init();
        process->spawn(&ProcessManager::onExit);
        process->run();
        process->close();

        removeProcess(process);
        return process->getResult();
    }
};

} // namespace process

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PROCESS_PROCESSMANAGER_HPP__

