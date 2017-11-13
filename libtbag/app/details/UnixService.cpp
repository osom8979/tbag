/**
 * @file   UnixService.cpp
 * @brief  UnixService class implementation.
 * @author zer0
 * @date   2017-05-27
 */

#include <libtbag/app/details/UnixService.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/filesystem/Path.hpp>

#if defined(TBAG_PLATFORM_UNIX_LIKE)
#include <unistd.h>
#include <fcntl.h>
#endif

#include <cassert>
#include <csignal>
#include <cstdio>
#include <iostream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace app     {
namespace details {

struct UnixService::Impl : private Noncopyable
{
    UnixService * parent;

#if defined(TBAG_PLATFORM_UNIX_LIKE)
    pid_t  sid; ///< Session ID.
    pid_t pgid; ///< Process group ID.
    pid_t ppid; ///< Parent Process ID.
    pid_t  pid; ///< Process ID.

    pid_t pid_file;
#endif

    Impl(UnixService * p) : parent(p), sid(0), pgid(0), ppid(0), pid(0), pid_file(0)
    { /* EMPTY. */ }

    ~Impl()
    {
        removePidFile();
    }

    Err eliminateControlTerminals()
    {
#if defined(TBAG_PLATFORM_UNIX_LIKE)
        pid = fork();

        if (pid > 0) {
            // This is parent process.
            tDLogI("UnixService::Impl::eliminateControlTerminals() Exit Parent Process {}", (int)getpid());
            _exit(0);

        } else if (pid < 0) {
            Err const FORK_ERROR_CODE = getGlobalSystemError();
            tDLogE("UnixService::Impl::eliminateControlTerminals() fork() {} error", getErrName(FORK_ERROR_CODE));
            return FORK_ERROR_CODE;
        }

        // This is child process.
        assert(pid == 0);

        sid = setsid(); // Be a session leader.
        if (sid == -1) {
            Err const SETSID_ERROR_CODE = getGlobalSystemError();
            tDLogE("UnixService::Impl::eliminateControlTerminals() setsid() {} error", getErrName(SETSID_ERROR_CODE));
            return SETSID_ERROR_CODE;
        }

        pgid = getpgid(0);
        ppid = getppid();
         pid = getpid();

        assert(sid == getsid(0));
        assert(pid != 0);
        assert(pgid == pid);

        // Don't use this code:
        //  @code
        //   pid_t const INIT_PROCESS = 1;
        //   assert(ppid == INIT_PROCESS);
        //  @endcode
        // Reason:
        //  Not working in the OSX.

        tDLogI("UnixService::Impl::eliminateControlTerminals() Fork child process: SID({}) PGID({}) PPID({}) PID({})",
               (int)sid, (int)pgid, (int)ppid, (int)pid);

        return Err::E_SUCCESS;
#else
        return Err::E_ENOSYS;
#endif
    }

    Err changeRootDirectory()
    {
#if defined(TBAG_PLATFORM_UNIX_LIKE)
        if (chdir("/") != 0) {
            Err const CHDIR_ERROR_CODE = getGlobalSystemError();
            tDLogE("UnixService::Impl::changeRootDirectory() chdir() {} error", getErrName(CHDIR_ERROR_CODE));
            return CHDIR_ERROR_CODE;
        }
        return Err::E_SUCCESS;
#else
        return Err::E_ENOSYS;
#endif
    }

    Err ignoreSignals()
    {
#if defined(TBAG_PLATFORM_UNIX_LIKE)
        ::signal(SIGHUP, SIG_IGN);

        // Ignore TTY signals.
        ::signal(SIGTSTP, SIG_IGN);
        ::signal(SIGTTOU, SIG_IGN);
        ::signal(SIGTTIN, SIG_IGN);

        return Err::E_SUCCESS;
#else
        return Err::E_ENOSYS;
#endif
    }

    Err reopenStdio()
    {
#if defined(TBAG_PLATFORM_UNIX_LIKE)
        stdin  = freopen("/dev/null", "r", stdin);
        stdout = freopen("/dev/null", "w", stdout);
        stderr = freopen("/dev/null", "w", stderr);
        return Err::E_SUCCESS;
#else
        return Err::E_ENOSYS;
#endif
    }

    Err zeroUmask()
    {
#if defined(TBAG_PLATFORM_UNIX_LIKE)
        umask(0);
        return Err::E_SUCCESS;
#else
        return Err::E_ENOSYS;
#endif
    }

    Err createPidFile(std::string const & path)
    {
#if defined(TBAG_PLATFORM_UNIX_LIKE)
        pid_file = open(path.c_str(), (O_RDWR | O_CREAT), 0600);
        if (pid_file == -1) {
            Err const OPEN_ERROR_CODE = getGlobalSystemError();
            tDLogE("UnixService::Impl::createPidFile() open() {} error", getErrName(OPEN_ERROR_CODE));
            return OPEN_ERROR_CODE;
        }

        // Try to lock file.
        if (lockf(pid_file, F_TLOCK, 0) == -1) {
            close(pid_file);
            pid_file = 0;

            // Couldn't get lock on lock file.
            Err const LOCKF_ERROR_CODE = getGlobalSystemError();
            tDLogE("UnixService::Impl::createPidFile() lockf() {} error", getErrName(LOCKF_ERROR_CODE));
            return LOCKF_ERROR_CODE;
        }

        std::string const PID_STRING = std::to_string(getpid());
        // Write pid to lock file.
        write(pid_file, PID_STRING.c_str(), PID_STRING.length());

        return Err::E_SUCCESS;
#else
        return Err::E_ENOSYS;
#endif
    }

    Err removePidFile()
    {
#if defined(TBAG_PLATFORM_UNIX_LIKE)
        if (pid_file > 0) {
            lockf(pid_file, F_ULOCK, 0);
            close(pid_file);
            pid_file = 0;
        }
        return Err::E_SUCCESS;
#else
        return Err::E_ENOSYS;
#endif
    }
};

// ---------------------------
// UnixService implementation.
// ---------------------------

UnixService::UnixService(std::string const & name)
        : ServiceCommon(name), _impl(new Impl(this)), _pid_path(std::string("/var/run/") + name + ".pid")
{
    assert(_impl != nullptr);
}

UnixService::~UnixService()
{
    _impl.reset();
    if (_pid_path.empty() == false) {
        filesystem::Path(_pid_path).remove();
    }
}

Err UnixService::install()
{
#if defined(TBAG_PLATFORM_UNIX_LIKE)
    return Err::E_ENOSYS;
#else
    return Err::E_ENOSYS;
#endif
}

Err UnixService::uninstall()
{
#if defined(TBAG_PLATFORM_UNIX_LIKE)
    return Err::E_ENOSYS;
#else
    return Err::E_ENOSYS;
#endif
}

Err UnixService::start()
{
    Err code = Err::E_UNKNOWN;
    code = _impl->eliminateControlTerminals();
    if (isFailure(code)) {
        return code;
    }

    code = _impl->changeRootDirectory();
    if (isFailure(code)) {
        return code;
    }

    _impl->ignoreSignals();
    _impl->zeroUmask();
    _impl->createPidFile(_pid_path);
    _impl->reopenStdio();

    return Err::E_SUCCESS;
}

Err UnixService::stop()
{
#if defined(TBAG_PLATFORM_UNIX_LIKE)
    return Err::E_ENOSYS;
#else
    return Err::E_ENOSYS;
#endif
}

} // namespace details
} // namespace app

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

