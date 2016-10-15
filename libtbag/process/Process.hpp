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
#include <libtbag/Noncopyable.hpp>
#include <libtbag/Exception.hpp>
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

/**
 * Process class prototype.
 *
 * @author zer0
 * @date   2016-05-17
 */
class Process : public Noncopyable
{
public:
    struct ProcPimpl;
    struct ProcOptionPimpl;

public:
    using Value     = char;
    using String    = std::basic_string<Value>;
    using Strings   = std::vector<String>;
    using EventLoop = loop::UvEventLoop;
    using Path      = filesystem::Path;

public:
    struct Param
    {
        String exe_path;
        String work_dir;

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
    using UniqueEventLoop       = std::unique_ptr<EventLoop>;
    using UniqueProcPimpl       = std::unique_ptr<ProcPimpl>;
    using UniqueProcOptionPimpl = std::unique_ptr<ProcOptionPimpl>;

public:
    constexpr static int64_t const UNKNOWN_EXIT_CODE        = std::numeric_limits<int64_t>::min();
    constexpr static int     const UNKNOWN_TERMINATE_SIGNAL = std::numeric_limits<int    >::min();

protected:
    UniqueEventLoop        _loop;
    UniqueProcPimpl        _process;
    UniqueProcOptionPimpl  _options;

public:
    Process() throw(InitializeException);
    virtual ~Process();

private:
    bool spawn();

public:
    bool exe(Param const & param);
    bool exe(Path const & exe_path, Path const & work_dir);
    bool exe(Path const & exe_path);

public:
    int64_t getExitStatus() const noexcept;
    int getTerminateSignal() const noexcept;
};

} // namespace process

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PROCESS_PROCESS_HPP__

