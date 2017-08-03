/**
 * @file   Process.hpp
 * @brief  Process class prototype.
 * @author zer0
 * @date   2017-02-03
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVPP_PROCESS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVPP_PROCESS_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/uvpp/Handle.hpp>

#include <string>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {

// Forward declaration.
class Loop;
class Stream;

/**
 * Process class prototype.
 *
 * @author zer0
 * @date   2017-02-03
 *
 * @remarks
 *  Process handles will spawn a new process and allow the user to control it @n
 *  and establish communication channels with it using streams.
 */
class TBAG_API Process : public Handle
{
public:
    friend class Loop;

public:
    using Parent = Handle;

public:
    // The stdio field points to an array of uv_stdio_container_t structs
    // that describe the file descriptors that will be made available to the child process.
    // The convention is that stdio[0] points to stdin, fd 1 is used for stdout, and fd 2 is stderr.
    TBAG_CONSTEXPR static int const STANDARD_INPUT_FD  =  0; ///< @c stdin
    TBAG_CONSTEXPR static int const STANDARD_OUTPUT_FD =  1; ///< @c stdout
    TBAG_CONSTEXPR static int const STANDARD_ERROR_FD  =  2; ///< @c stderr
    TBAG_CONSTEXPR static int const STANDARD_IO_SIZE   =  3;
    TBAG_CONSTEXPR static int const UNKNOWN_PROCESS_ID = -1;

public:
    /**
     * Stdio container prototype.
     *
     * @author zer0
     * @date   2017-02-04
     *
     * @remarks
     *  Container for each stdio handle or fd passed to a child process.
     */
    struct TBAG_API StdioContainer
    {
        enum class Type
        {
            STDIO_CONTAINER_STREAM,
            STDIO_CONTAINER_FD,
        } type;

        Stream * stream;
        int fd;

        bool ignore;
        bool create_pipe;
        bool inherit_fd;
        bool inherit_stream;
        bool readable_pipe;
        bool writable_pipe;

        StdioContainer();
        explicit StdioContainer(Stream * s, bool inherit = false);
        explicit StdioContainer(int f, bool inherit = false);
        ~StdioContainer();

        StdioContainer & clear();

        StdioContainer & setStream(Stream * s);
        StdioContainer & setFd(int f);

        StdioContainer & setIgnore();
        StdioContainer & setCreatePipe(bool flag = true);
        StdioContainer & setInheritFd(bool flag = true);
        StdioContainer & setInheritStream(bool flag = true);
        StdioContainer & setReadablePipe(bool flag = true);
        StdioContainer & setWritablePipe(bool flag = true);
    };

    /**
     * Options for spawning the process.
     *
     * @author zer0
     * @date   2017-02-04
     */
    struct TBAG_API Options
    {
        using Strings = std::vector<std::string>;
        using Stdios  = std::vector<StdioContainer>;

        std::string file; ///< Executable file path.
        std::string cwd;  ///< Working directory.

        Strings args; ///< Arguments.
        Strings envs; ///< Environment variables (e.g. VAR=VALUE).

        Stdios stdios;

        // Changing the UID/GID is only supported on Unix.
        uuser  uid;
        ugroup gid;

        bool setuid; ///< Set the child process' user id.
        bool setgid; ///< Set the child process' group id.
        bool detached;
        bool verbatim_args;
        bool hide;

        Options();
        ~Options();

        Options & clear();

        Options & setFile(std::string const & path);
        Options & setWorking(std::string const & dir);
        Options & setCurrentWorking();

        Options & appendArgument(std::string const & arg);
        Options & appendEnvironment(std::string const & env);

        Options & appendStdio(StdioContainer const & io);

        Options & setUserId(uuser id, bool enable = true);
        Options & setGroupId(ugroup id, bool enable = true);

        Options & setDetached(bool flag = true);
        Options & setVerbatimArguments(bool flag = true);
        Options & setHide(bool flag = true);

        std::string getAllArguments() const;
    };

private:
    Options _options;

protected:
    Process(Loop & loop, Options const & options);

public:
    virtual ~Process();

public:
    /** The PID of the spawned process. It’s set after calling uv_spawn(). */
    int getPid() const;

public:
    /** Initializes the process handle and starts the process. */
    Err spawn(Loop & loop, Options const & options);

    /** Sends the specified signal to the given process handle. */
    Err processKill(int signum);

public:
    virtual void onExit(int64_t exit_status, int term_signal);

public:
    /** Disables inheritance for file descriptors / handles that this process inherited from its parent. */
    static void disableStdioInheritance();

    /** Sends the specified signal to the given PID. */
    static Err kill(int pid, int signum);
};

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_PROCESS_HPP__

