/**
 * @file   AddressToLine.cpp
 * @brief  AddressToLine class implementation.
 * @author zer0
 * @date   2017-06-06
 */

#include <libtbag/debug/st/unix/AddressToLine.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/uvpp/Process.hpp>

#include <cerrno>
#include <csignal> // kill(2)

#if defined(TBAG_PLATFORM_UNIX_LIKE)
# include <sys/types.h>
# include <sys/wait.h> // waitpid(2)
# include <execinfo.h>
# include <unistd.h> // pipe(2), fork(2), dup2(2), execvp(3), _exit(2)
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace debug {
namespace st    {
namespace unix  {

TBAG_CONSTEXPR static char const * const ADDR2LINE_PROGRAM_NAME = "addr2line";

struct AddressToLine::Impl : private Noncopyable
{
    FILE * output_pipe;

#if defined(TBAG_PLATFORM_UNIX_LIKE)
    pid_t pid;
#else
    int pid;
#endif

    Impl(const char * flag, const char * exec_path, const char * addr)
            : output_pipe(0), pid(0)
    {
#if defined(TBAG_PLATFORM_UNIX_LIKE)
        int pdes[2] = {0,};
        char * argp[] = {
                const_cast<char*>(ADDR2LINE_PROGRAM_NAME),
                const_cast<char*>(flag),
                const_cast<char*>(exec_path),
                const_cast<char*>(addr),
                nullptr
        };

        if (pipe(pdes) < 0) {
            tDLogE("AddressToLine::Impl::Impl() pipe {} error", getErrName(getGlobalSystemError()));
            return;
        }

        pid = fork();
        if (pid == -1) {
            tDLogE("AddressToLine::Impl::Impl() fork {} error", getErrName(getGlobalSystemError()));
            close(pdes[0]);
            close(pdes[1]);
            return;
        } else if (pid == 0) {
            // This is child process section.
            close(STDERR_FILENO);
            close(pdes[0]);
            if (pdes[1] != STDOUT_FILENO) {
                dup2(pdes[1], STDOUT_FILENO);
            }
            execvp(ADDR2LINE_PROGRAM_NAME, argp);
            _exit(127);
        }
        output_pipe = fdopen(pdes[0], "r");
        close(pdes[1]);
#endif
    }

    ~Impl()
    {
#if defined(TBAG_PLATFORM_UNIX_LIKE)
        if (output_pipe != nullptr) {
            fclose(output_pipe);
            kill(pid, SIGKILL);
            int pstat = 0;
            waitpid(pid, &pstat, 0);
        }
#endif
    }

    inline operator FILE *() const TBAG_NOEXCEPT
    {
        return output_pipe;
    }
};

AddressToLine::AddressToLine(const char * flag, const char * exec_path, const char * addr)
        : _impl(new Impl(flag, exec_path, addr))
{
    // EMPTY.
}

AddressToLine::~AddressToLine()
{
    // EMPTY.
}

FILE * AddressToLine::getFilePointer() const TBAG_NOEXCEPT
{
    return _impl->output_pipe;
}

} // namespace unix
} // namespace st
} // namespace debug

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

