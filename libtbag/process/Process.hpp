/**
 * @file   Process.hpp
 * @brief  Process class prototype.
 * @author zer0
 * @date   2016-11-27
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PROCESS_PROCESS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PROCESS_PROCESS_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/debug/ErrorCode.hpp>
#include <libtbag/loop/UvEventLoop.hpp>
#include <libtbag/filesystem/Path.hpp>

#include <cstdint>
#include <string>
#include <vector>
#include <limits>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace process {

TBAG_API std::string getExecutableSuffix();
TBAG_API std::string getExecutableName(std::string const & name);

/**
 * Process class prototype.
 *
 * @author zer0
 * @date   2016-11-27
 */
class TBAG_API Process : public Noncopyable
{
public:
    struct ProcPimpl;
    friend struct ProcPimpl;

public:
    using String    = std::string;
    using Strings   = std::vector<String>;
    using EventLoop = loop::UvEventLoop;
    using Path      = filesystem::Path;

public:
    struct Param
    {
        String exe_path; ///< Executable file path.
        String work_dir; ///< Working directory.

        Strings arguments;    ///< Arguments.
        Strings environments; ///< Environment variables (e.g. VAR=VALUE).

        // Changing the UID/GID is only supported on Unix.
        uint32_t uid = 0;
        uint32_t gid = 0;

        bool verbatim_arg = false;
        bool detached     = false;

        // @formatter:off
        inline Param & setExePath     (Path const & path)    { exe_path = path.getCanonicalString(); return *this; }
        inline Param & setWorkingDir  (Path const & dir)     { work_dir = dir.getCanonicalString();  return *this; }
        inline Param & pushArgument   (String const & arg)   { arguments.push_back(arg);    return *this; }
        inline Param & pushEnvironment(String const & env)   { environments.push_back(env); return *this; }
        inline Param & setArguments   (Strings const & args) { arguments    = args; return *this; }
        inline Param & setEnvironments(Strings const & envs) { environments = envs; return *this; }
        inline Param & setUserId      (uint32_t id)          { uid = id; return *this; }
        inline Param & setGroupId     (uint32_t id)          { gid = id; return *this; }
        inline Param & setVerbatimArgs(bool flag = true)     { verbatim_arg = flag; return *this; }
        inline Param & setDetached    (bool flag = true)     { detached     = flag; return *this; }
        // @formatter:on
    };

public:
    using UniqueProcPimpl = std::unique_ptr<ProcPimpl>;

public:
    inline static TBAG_CONSTEXPR int64_t getUnknownExitCode() TBAG_NOEXCEPT
    { return std::numeric_limits<int64_t>::min(); }
    inline static TBAG_CONSTEXPR int getUnknownTerminateSignal() TBAG_NOEXCEPT
    { return std::numeric_limits<int>::min(); }

protected:
    EventLoop _loop;
    UniqueProcPimpl _process;

protected:
    Process::Param     _param;
    std::vector<char*> _args_ptr;
    std::vector<char*> _envs_ptr;

private:
    int64_t _exit_status;
    int _terminate_signal;

public:
    Process();
    virtual ~Process();

public:
    inline void setParam(Process::Param const & param)
    { _param = param; }

    inline int64_t getExitStatus() const TBAG_NOEXCEPT
    { return _exit_status; }
    inline int getTerminateSignal() const TBAG_NOEXCEPT
    { return _terminate_signal; }

private:
    void clear();
    void update();
    ErrorCode spawn();

public:
    virtual bool exe();

public:
    bool exe(Param const & param);
    bool exe(Path const & exe_path, Path const & work_dir, Strings const & args, Strings const & envs);
    bool exe(Path const & exe_path, Path const & work_dir);
    bool exe(Path const & exe_path);

public:
    int getProcessId() const;
    ErrorCode kill(int signal_number);

public:
    void onExit(void * handle, int64_t exit_status, int term_signal);
};

} // namespace process

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PROCESS_PROCESS_HPP__

