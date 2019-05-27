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

#include <cstdint>
#include <atomic>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {
namespace func {

/**
 * FunctionalProcess class prototype.
 *
 * @author zer0
 * @date   2017-05-31
 */
template <typename ProcessType>
struct FunctionalProcess : public FunctionalHandle<ProcessType>
{
    using Parent  = FunctionalHandle<ProcessType>;
    using Options = typename Parent::Options;
    using OnExit  = std::function<void(int64_t, int)>;

    STATIC_ASSERT_CHECK_IS_BASE_OF(libtbag::uvpp::Process, Parent);
    STATIC_ASSERT_CHECK_IS_SAME(libtbag::uvpp::Process::Options, Options);

    OnExit exit_cb;

    std::atomic<bool>    exit;
    std::atomic<int64_t> exit_status;
    std::atomic<int>     term_signal;

    template <typename ... Args>
    FunctionalProcess(Args && ... args) : Parent(std::forward<Args>(args) ...),
                                          exit(false), exit_status(0), term_signal(0)
    { /* EMPTY. */ }

    virtual ~FunctionalProcess()
    { /* EMPTY. */ }

    virtual void onExit(int64_t code, int signal) override
    {
        exit = true;
        exit_status = code;
        term_signal = signal;

        if (exit_cb) {
            exit_cb(code, signal);
        } else {
            Parent::onExit(code, signal);
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

