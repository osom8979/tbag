/**
 * @file   FunctionalStdProcess.hpp
 * @brief  FunctionalStdProcess class prototype.
 * @author zer0
 * @date   2017-09-15
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PROCESS_FUNCTIONALSTDPROCESS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PROCESS_FUNCTIONALSTDPROCESS_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/process/StdProcess.hpp>

#include <functional>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace process {

/**
 * FunctionalStdProcess class prototype.
 *
 * @author zer0
 * @date   2017-09-05
 */
class FunctionalStdProcess : public StdProcess
{
public:
    using OnOutRead = std::function<void(char const *, std::size_t)>;
    using OnErrRead = std::function<void(char const *, std::size_t)>;
    using OnExit    = std::function<void(int64_t, int)>;
    using OnClose   = std::function<void()>;

private:
    OnOutRead  _out_read_cb;
    OnErrRead  _err_read_cb;
    OnExit     _exit_cb;
    OnClose    _close_cb;

public:
    FunctionalStdProcess()
    { /* EMPTY. */ }
    virtual ~FunctionalStdProcess()
    { /* EMPTY. */ }

public:
    void setOnOutRead(OnOutRead const & cb) { _out_read_cb = cb; }
    void setOnErrRead(OnErrRead const & cb) { _err_read_cb = cb; }
    void setOnExit   (OnExit    const & cb) {     _exit_cb = cb; }
    void setOnClose  (OnClose   const & cb) {    _close_cb = cb; }

public:
    virtual void onOutRead(char const * buffer, std::size_t size) override
    {
        if (static_cast<bool>(_out_read_cb)) {
            _out_read_cb(buffer, size);
        }
    }

    virtual void onErrRead(char const * buffer, std::size_t size) override
    {
        if (static_cast<bool>(_err_read_cb)) {
            _err_read_cb(buffer, size);
        }
    }

    virtual void onExit(int64_t exit_status, int term_signal) override
    {
        if (static_cast<bool>(_exit_cb)) {
            _exit_cb(exit_status, term_signal);
        }
    }

    virtual void onClose() override
    {
        if (static_cast<bool>(_close_cb)) {
            _close_cb();
        }
    }
};

using FuncStdProcess = FunctionalStdProcess;

} // namespace process

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PROCESS_FUNCTIONALSTDPROCESS_HPP__

