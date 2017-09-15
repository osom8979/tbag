/**
 * @file   FunctionalProcessManager.hpp
 * @brief  FunctionalProcessManager class prototype.
 * @author zer0
 * @date   2017-09-15
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PROCESS_FUNCTIONALPROCESSMANAGER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PROCESS_FUNCTIONALPROCESSMANAGER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/process/ProcessManager.hpp>

#include <functional>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace process {

/**
 * FunctionalProcessManager class prototype.
 *
 * @author zer0
 * @date   2017-09-06
 */
class FunctionalProcessManager : public ProcessManager
{
public:
    using OnOutRead = std::function<void(int, char const *, std::size_t)>;
    using OnErrRead = std::function<void(int, char const *, std::size_t)>;
    using OnExit    = std::function<void(int, int64_t, int)>;

private:
    OnOutRead  _out_read_cb;
    OnErrRead  _err_read_cb;
    OnExit     _exit_cb;

public:
    FunctionalProcessManager()
    { /* EMPTY. */ }
    virtual ~FunctionalProcessManager()
    { /* EMPTY. */ }

public:
    void setOnOutRead(OnOutRead const & cb) { _out_read_cb = cb; }
    void setOnErrRead(OnErrRead const & cb) { _err_read_cb = cb; }
    void setOnExit   (OnExit    const & cb) {     _exit_cb = cb; }

public:
    virtual void onOutRead(int pid, char const * buffer, std::size_t size) override
    {
        if (static_cast<bool>(_out_read_cb)) {
            _out_read_cb(pid, buffer, size);
        }
    }

    virtual void onErrRead(int pid, char const * buffer, std::size_t size) override
    {
        if (static_cast<bool>(_err_read_cb)) {
            _err_read_cb(pid, buffer, size);
        }
    }

    virtual void onExit(int pid, int64_t exit_status, int term_signal) override
    {
        if (static_cast<bool>(_exit_cb)) {
            _exit_cb(pid, exit_status, term_signal);
        }
    }
};

using FuncProcessManager = FunctionalProcessManager;

} // namespace process

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PROCESS_FUNCTIONALPROCESSMANAGER_HPP__

