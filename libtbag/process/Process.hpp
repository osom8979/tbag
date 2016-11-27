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

#include <cstdint>
#include <string>
#include <limits>

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
    inline static TBAG_CONSTEXPR int64_t getUnknownExitCode() TBAG_NOEXCEPT
    { return std::numeric_limits<int64_t>::min(); }
    inline static TBAG_CONSTEXPR int getUnknownTerminateSignal() TBAG_NOEXCEPT
    { return std::numeric_limits<int>::min(); }

private:
    int _terminate_signal;
    int64_t _exit_status;

public:
    Process();
    ~Process();

public:
    bool exe(std::string const & exe_path, std::string const & work_dir);
    bool exe(std::string const & exe_path);

public:
    virtual int     getTerminateSignal() const;
    virtual int64_t getExitStatus()      const;

public:
    void onExit(void * handle, int64_t exit_status, int term_signal);
};

} // namespace process

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PROCESS_PROCESS_HPP__

