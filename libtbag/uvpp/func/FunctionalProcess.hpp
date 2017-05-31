/**
 * @file   FunctionalProcess.hpp
 * @brief  FunctionalProcess class prototype.
 * @author zer0
 * @date   2017-05-31
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIONALPROCESS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIONALPROCESS_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Type.hpp>

#include <libtbag/uvpp/func/FunctionalHandle.hpp>
#include <libtbag/uvpp/Process.hpp>
#include <libtbag/lock/FakeLock.hpp>

#include <cstdint>
#include <functional>
#include <mutex>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {

// Forward declaration.
class Loop;

namespace func {

/**
 * FunctionalProcess class prototype.
 *
 * @author zer0
 * @date   2017-05-31
 */
template <typename ProcessType, typename MutexType = lock::FakeLock>
class FunctionalProcess : public ProcessType
{
public:
    using Parent = ProcessType;
    using Mutex  = MutexType;
    using Guard  = std::lock_guard<Mutex>;

    STATIC_ASSERT_CHECK_IS_BASE_OF(libtbag::uvpp::Process, Parent);
    TBAG_UVPP_FUNCTIONAL_HANDLE_DEFAULT(Guard, _mutex);

public:
    using Options = typename Parent::Options;
    using OnExit  = std::function<void(int64_t, int)>;

private:
    Mutex _mutex;
    OnExit _exit_cb;

public:
    FunctionalProcess(Loop & loop, Options const & options) : Parent(loop, options)
    { /* EMPTY. */ }
    virtual ~FunctionalProcess()
    { /* EMPTY. */ }

public:
    void setOnExit (OnExit const & cb)
    {
        Guard guard(_mutex);
        _exit_cb = cb;
    }

public:
    virtual void onExit(int64_t exit_status, int term_signal) override
    {
        Guard guard(_mutex);
        if (static_cast<bool>(_exit_cb)) {
            _exit_cb(exit_status, term_signal);
        }
    }
};

/**
 * FuncProcess typedef.
 *
 * @author zer0
 * @date   2017-05-29
 */
using FuncProcess = FunctionalProcess<libtbag::uvpp::Process>;

} // namespace func
} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIONALPROCESS_HPP__

