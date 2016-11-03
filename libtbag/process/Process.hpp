/**
 * @file   Process.hpp
 * @brief  Process class prototype.
 * @author zer0
 * @date   2016-05-17
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
#include <libtbag/loop/event/UvHandler.hpp>
#include <libtbag/filesystem/Path.hpp>

#include <cstdlib>

#include <string>
#include <vector>
#include <limits>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

// Forward declaration.
namespace loop { class UvEventLoop; }

namespace process {

TBAG_API std::string getExecutableSuffix();
TBAG_API std::string getExecutableName(std::string const & name);

/**
 * Process class prototype.
 *
 * @author zer0
 * @date   2016-05-17
 */
class TBAG_API Process : public libtbag::Noncopyable
{
public:
    struct ProcPimpl;

public:
    using Value     = char;
    using String    = std::basic_string<Value>;
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
    Process();
    virtual ~Process();

private:
    bool spawn();

public:
    bool exe(Param const & param);
    bool exe(Path const & exe_path, Path const & work_dir);
    bool exe(Path const & exe_path);

public:
    int64_t getExitStatus();
    int getTerminateSignal();
};

} // namespace process

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PROCESS_PROCESS_HPP__

