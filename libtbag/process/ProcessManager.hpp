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
#include <libtbag/uvpp/func/FunctionalProcess.hpp>

#include <unordered_map>
#include <memory>

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
    using Options = uvpp::Process::Options;

    using FuncProcess = uvpp::func::FuncProcess;

    using SharedProcess = std::shared_ptr<FuncProcess>;
    using   WeakProcess =   std::weak_ptr<FuncProcess>;

    using ProcessMap  = std::unordered_map<int, SharedProcess>;
    using ProcessPair = ProcessMap::value_type;

private:
    ProcessMap _procs;
    bool _auto_erase;

public:
    ProcessManager();
    ProcessManager(bool auto_erase);
    virtual ~ProcessManager();

public:
    inline bool        empty() const TBAG_NOEXCEPT_SP_OP(_procs.empty()) { return _procs.empty(); }
    inline std::size_t  size() const TBAG_NOEXCEPT_SP_OP(_procs.size())  { return _procs.size();  }

public:
    WeakProcess get(int pid);

public:
    int exec(Loop & loop, Options const & options);
    Err kill(int pid, int signum);

public:
    virtual void onSpawn(Err code, int pid);
    virtual void onExit(int pid, int64_t exit_status, int term_signal);
};

} // namespace process

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PROCESS_PROCESSMANAGER_HPP__

