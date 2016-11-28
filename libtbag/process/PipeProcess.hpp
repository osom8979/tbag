/**
 * @file   PipeProcess.hpp
 * @brief  PipeProcess class prototype.
 * @author zer0
 * @date   2016-05-17
 * @date   2016-11-27 (Rename: Process -> PipeProcess)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PROCESS_PIPEPROCESS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PROCESS_PIPEPROCESS_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/loop/event/UvHandler.hpp>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/process/OutStream.hpp>
#include <libtbag/process/InStream.hpp>
#include <libtbag/process/Process.hpp>

#include <cstdlib>

#include <string>
#include <vector>
#include <limits>
#include <memory>
#include <functional>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

// Forward declaration.
namespace loop { class UvEventLoop; }

namespace process {

/**
 * PipeProcess class prototype.
 *
 * @author zer0
 * @date   2016-05-17
 */
class TBAG_API PipeProcess : public Process
{
public:
    struct ProcPimpl;
    friend struct ProcPimpl;

public:
    using String    = std::basic_string<char>;
    using Strings   = std::vector<String>;
    using EventLoop = loop::UvEventLoop;
    using Path      = filesystem::Path;

public:
    struct Param
    {
        String exe_path; ///< Executable file path.
        String work_dir; ///< Working directory.

        Strings args; ///< Arguments.
        Strings envs; ///< Environment variables.

        std::vector<char> input_buffer;

        OutStream::OnReadCallback out_callback;
        OutStream::OnReadCallback err_callback;
        InStream::OnWriteCallback in_callback;

        unsigned int flags = 0;

        inline Param & setExePath(Path const & path)
        { exe_path = path.getCanonicalString(); return *this; }
        inline Param & setWorkingDir(Path const & dir)
        { work_dir = dir.getCanonicalString(); return *this; }

        inline Param & pushArggument(String const & arg)
        { args.push_back(arg); return *this; }
        inline Param & pushEnvironment(String const & env)
        { envs.push_back(env); return *this; }

        inline Param & setFlags(unsigned int flag)
        { flags = flag; return *this; }

        inline Param & setInput(std::vector<char> const & buffer)
        { input_buffer = buffer; return *this; }
        inline Param & setInput(char const * buffer, std::size_t length)
        { input_buffer.assign(buffer, buffer + length); return *this; }

        inline Param & setOutReadCallback(OutStream::OnReadCallback const & callback)
        { out_callback = callback; return *this; }
        inline Param & setErrReadCallback(OutStream::OnReadCallback const & callback)
        { err_callback = callback; return *this; }
        inline Param & setInWriteCallback(InStream::OnWriteCallback const & callback)
        { in_callback = callback; return *this; }
    };

public:
    using UniqueEventLoop = std::unique_ptr<EventLoop>;
    using UniqueProcPimpl = std::unique_ptr<ProcPimpl>;

public:
    inline TBAG_CONSTEXPR static int64_t getUnknownExitCode() TBAG_NOEXCEPT
    { return std::numeric_limits<int64_t>::min(); }
    inline TBAG_CONSTEXPR static int getUnknownTerminateSignal() TBAG_NOEXCEPT
    { return std::numeric_limits<int>::min(); }

protected:
    UniqueEventLoop _loop;
    UniqueProcPimpl _process;

public:
    PipeProcess();
    ~PipeProcess();

public:
    bool exe(Param const & param);
    bool exe(Path const & exe_path, Path const & work_dir);
    bool exe(Path const & exe_path);

public:
    bool write(char const * buffer, std::size_t length);

public:
    int64_t getExitStatus() const;
    int getTerminateSignal() const;
};

} // namespace process

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PROCESS_PIPEPROCESS_HPP__

