/**
 * @file   UnixService.cpp
 * @brief  UnixService class implementation.
 * @author zer0
 * @date   2017-05-27
 * @date   2019-11-05 (Move package: libtbag/app/details -> libtbag/service/details)
 */

#include <libtbag/service/details/UnixService.hpp>
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

namespace service {
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

    Impl(UnixService * p) : parent(p)
    {
#if defined(TBAG_PLATFORM_UNIX_LIKE)
        sid  = 0;
        pgid = 0;
        ppid = 0;
        pid  = 0;
        pid_file = 0;
#endif
    }

    ~Impl()
    { /* EMPTY. */ }

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
            tDLogE("UnixService::Impl::eliminateControlTerminals() fork() {} error", FORK_ERROR_CODE);
            return FORK_ERROR_CODE;
        }

        // This is child process.
        assert(pid == 0);

        sid = setsid(); // Be a session leader.
        if (sid == -1) {
            Err const SETSID_ERROR_CODE = getGlobalSystemError();
            tDLogE("UnixService::Impl::eliminateControlTerminals() setsid() {} error", SETSID_ERROR_CODE);
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
        //  It works with root privileges.

        tDLogI("UnixService::Impl::eliminateControlTerminals() "
               "Fork child process: SID({}) PGID({}) PPID({}) PID({})",
               (int)sid, (int)pgid, (int)ppid, (int)pid);

        return E_SUCCESS;
#else
        return E_ENOSYS;
#endif
    }

    Err changeRootDirectory()
    {
#if defined(TBAG_PLATFORM_UNIX_LIKE)
        if (chdir("/") != 0) {
            Err const CHDIR_ERROR_CODE = getGlobalSystemError();
            tDLogE("UnixService::Impl::changeRootDirectory() chdir() {} error", CHDIR_ERROR_CODE);
            return CHDIR_ERROR_CODE;
        }
        return E_SUCCESS;
#else
        return E_ENOSYS;
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

        return E_SUCCESS;
#else
        return E_ENOSYS;
#endif
    }

    Err reopenStdio()
    {
#if defined(TBAG_PLATFORM_UNIX_LIKE)
        stdin  = freopen("/dev/null", "r", stdin);
        stdout = freopen("/dev/null", "w", stdout);
        stderr = freopen("/dev/null", "w", stderr);
        return E_SUCCESS;
#else
        return E_ENOSYS;
#endif
    }

    Err zeroUmask()
    {
#if defined(TBAG_PLATFORM_UNIX_LIKE)
        umask(0);
        return E_SUCCESS;
#else
        return E_ENOSYS;
#endif
    }

    Err createPidFile(std::string const & path)
    {
#if defined(TBAG_PLATFORM_UNIX_LIKE)
        pid_file = open(path.c_str(), (O_RDWR | O_CREAT), 0600);
        if (pid_file == -1) {
            Err const OPEN_ERROR_CODE = getGlobalSystemError();
            tDLogE("UnixService::Impl::createPidFile() open() {} error", OPEN_ERROR_CODE);
            return OPEN_ERROR_CODE;
        }

        // Try to lock file.
        if (lockf(pid_file, F_TLOCK, 0) == -1) {
            close(pid_file);
            pid_file = 0;

            // Couldn't get lock on lock file.
            Err const LOCKF_ERROR_CODE = getGlobalSystemError();
            tDLogE("UnixService::Impl::createPidFile() lockf() {} error", LOCKF_ERROR_CODE);
            return LOCKF_ERROR_CODE;
        }

        std::string const PID_STRING = std::to_string(getpid());
        // Write pid to lock file.
        ssize_t const write_size = write(pid_file, PID_STRING.c_str(), PID_STRING.length());
        if (write_size == -1) {
            tDLogW("UnixService::Impl::createPidFile() Write PID error: {}", getGlobalSystemError());
        }
        return E_SUCCESS;
#else
        return E_ENOSYS;
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
        return E_SUCCESS;
#else
        return E_ENOSYS;
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
    // EMPTY.
}

Err UnixService::install()
{
#if defined(TBAG_PLATFORM_UNIX_LIKE)
    return E_ENOSYS;
#else
    return E_ENOSYS;
#endif
}

Err UnixService::uninstall()
{
#if defined(TBAG_PLATFORM_UNIX_LIKE)
    return E_ENOSYS;
#else
    return E_ENOSYS;
#endif
}

Err UnixService::start()
{
    Err code = E_UNKNOWN;
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

    return E_SUCCESS;
}

Err UnixService::stop()
{
    _impl->removePidFile();
    if (_pid_path.empty() == false) {
        filesystem::Path(_pid_path).remove();
    }
    return E_SUCCESS;
}

} // namespace details
} // namespace service

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

