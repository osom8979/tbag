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
#include <libtbag/stream/UvInStream.hpp>
#include <libtbag/stream/UvOutStream.hpp>
#include <libtbag/filesystem/Path.hpp>

#include <cstdint>
#include <string>
#include <vector>
#include <limits>
#include <memory>
#include <future>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace process {

// Platform-specific definitions for uv_spawn support.
#if defined(__PLATFORM_WINDOWS__)
typedef unsigned char UserIdType;
typedef unsigned char GroupIdType;
#else
typedef uint32_t UserIdType;
typedef uint32_t GroupIdType;
#endif

// The stdio field points to an array of uv_stdio_container_t structs
// that describe the file descriptors that will be made available to the child process.
// The convention is that stdio[0] points to stdin, fd 1 is used for stdout, and fd 2 is stderr.
static int const STANDARD_INPUT_FD  = 0; ///< @c stdin
static int const STANDARD_OUTPUT_FD = 1; ///< @c stdout
static int const STANDARD_ERROR_FD  = 2; ///< @c stderr
static int const STANDARD_IO_SIZE   = 3;

static int const UNKNOWN_PROCESS_ID = -1;

TBAG_API std::string getExecutableSuffix();
TBAG_API std::string getExecutableName(std::string const & name);

/**
 * Process class prototype.
 *
 * @author zer0
 * @date   2016-11-27
 * @date   2016-12-17 (Merge with PipeProcess class)
 */
class TBAG_API Process : public Noncopyable
{
public:
    struct ProcPimpl;
    friend struct ProcPimpl;

public:
    using String    = std::string;
    using Strings   = std::vector<String>;
    using Buffer    = std::vector<char>;
    using EventLoop = loop::UvEventLoop;
    using InStream  = stream::UvInStream;
    using OutStream = stream::UvOutStream;
    using Path      = filesystem::Path;

public:
    /**
     * Process parameters.
     *
     * @author zer0
     * @date   2016-11-27
     * @date   2016-12-17 (Merge with PipeProcess class)
     */
    struct Param
    {
        // @formatter:off
        enum class IoFlag : int { IGNORE_IO, INHERIT, PIPE, };
        // @formatter:on

        struct IoOption
        {
            IoFlag flag = IoFlag::IGNORE_IO;
            int    fd   = 0;

            inline void set(IoFlag f, int d)
            { flag = f; fd = d; }
        };

        String exe_path; ///< Executable file path.
        String work_dir; ///< Working directory.

        Strings arguments;    ///< Arguments.
        Strings environments; ///< Environment variables (e.g. VAR=VALUE).

        // Changing the UID/GID is only supported on Unix.
        UserIdType  uid = 0;
        GroupIdType gid = 0;

        bool verbatim_arg = false;
        bool detached     = false;

        IoOption  in;
        IoOption out;
        IoOption err;

        Buffer in_buffer;

        // @formatter:off
        inline Param & setExePath     (Path const & path)    { exe_path = path.getCanonicalString(); return *this; }
        inline Param & setWorkingDir  (Path const & dir)     { work_dir = dir.getCanonicalString();  return *this; }
        inline Param & pushArgument   (String const & arg)   { arguments.push_back(arg);    return *this; }
        inline Param & pushEnvironment(String const & env)   { environments.push_back(env); return *this; }
        inline Param & setArguments   (Strings const & args) { arguments    = args; return *this; }
        inline Param & setEnvironments(Strings const & envs) { environments = envs; return *this; }
        inline Param & setUserId      (UserIdType  id)       { uid = id; return *this; }
        inline Param & setGroupId     (GroupIdType id)       { gid = id; return *this; }
        inline Param & setVerbatimArgs(bool flag = true)     { verbatim_arg = flag; return *this; }
        inline Param & setDetached    (bool flag = true)     { detached     = flag; return *this; }
        // @formatter:on

        // @formatter:off
        inline Param & setStdinIgnore ()       {  in.set(IoFlag::IGNORE_IO,  0); return *this; }
        inline Param & setStdoutIgnore()       { out.set(IoFlag::IGNORE_IO,  0); return *this; }
        inline Param & setStderrIgnore()       { err.set(IoFlag::IGNORE_IO,  0); return *this; }
        inline Param & setStdinPipe   ()       {  in.set(IoFlag::PIPE     ,  0); return *this; }
        inline Param & setStdoutPipe  ()       { out.set(IoFlag::PIPE     ,  0); return *this; }
        inline Param & setStderrPipe  ()       { err.set(IoFlag::PIPE     ,  0); return *this; }
        inline Param & setStdinFd     (int fd = STANDARD_INPUT_FD ) {  in.set(IoFlag::INHERIT, fd); return *this; }
        inline Param & setStdoutFd    (int fd = STANDARD_OUTPUT_FD) { out.set(IoFlag::INHERIT, fd); return *this; }
        inline Param & setStderrFd    (int fd = STANDARD_ERROR_FD ) { err.set(IoFlag::INHERIT, fd); return *this; }
        // @formatter:on

        // @formatter:off
        inline Param & clearInBuffer()
        { in_buffer.clear(); return *this; }
        inline Param & setInBuffer(char const * buffer, std::size_t size)
        { in_buffer.assign(buffer, buffer + size); return *this; }
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
    InStream  _in;  ///< 0, stdin.
    OutStream _out; ///< 1, stdout.
    OutStream _err; ///< 2, stderr.

private:
    int64_t _exit_status;
    int _terminate_signal;

public:
    Process();
    ~Process();

public:
    inline void setParam(Process::Param const & param)
    { _param = param; }

    inline void setOnStdinCallback(InStream::OnWriteCallback * callback) TBAG_NOEXCEPT
    { _in.setOnWriteCallback(callback); }
    inline void setOnStdoutCallback(OutStream::OnReadCallback * callback) TBAG_NOEXCEPT
    { _out.setOnReadCallback(callback); }
    inline void setOnStderrCallback(OutStream::OnReadCallback * callback) TBAG_NOEXCEPT
    { _err.setOnReadCallback(callback); }

    inline int64_t getExitStatus() const TBAG_NOEXCEPT
    { return _exit_status; }
    inline int getTerminateSignal() const TBAG_NOEXCEPT
    { return _terminate_signal; }

private:
    void clear();
    void update();
    void updateWithFlags();
    void updateWithStdios();
    ErrorCode spawn();

public:
    bool exe();
    void close();

private:
    void close(util::UvHandle & handle);

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
    void onClose(void * handle);

public:
    inline static std::future<bool> asyncExe(Process & process, Param const & param)
    { return std::async(std::launch::async, [&process, &param]() -> bool { return process.exe(param); }); }
};

} // namespace process

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PROCESS_PROCESS_HPP__

