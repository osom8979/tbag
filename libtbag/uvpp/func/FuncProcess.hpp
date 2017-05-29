/**
 * @file   FuncProcess.hpp
 * @brief  FuncProcess class prototype.
 * @author zer0
 * @date   2017-05-29
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCPROCESS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCPROCESS_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Type.hpp>
#include <libtbag/uvpp/Process.hpp>

#include <functional>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {

// Forward declaration.
class Loop;

namespace func {

/**
 * FuncProcess class prototype.
 *
 * @author zer0
 * @date   2017-05-29
 */
template <typename ProcessType>
class FuncProcess : public ProcessType
{
public:
    using Parent = ProcessType;
    STATIC_ASSERT_CHECK_IS_BASE_OF(libtbag::uvpp::Process, Parent);

public:
    using Options = typename Parent::Options;

    using OnClose = std::function<void(void)>;
    using OnWalk  = std::function<void(void*)>;
    using OnExit  = std::function<void(int64_t, int)>;

private:
    OnClose _close_cb;
    OnExit   _exit_cb;

public:
    FuncProcess(Loop & loop, Options const & options) : Parent(loop, options)
    { /* EMPTY. */ }
    virtual ~FuncProcess()
    { /* EMPTY. */ }

public:
    // @formatter:off
    inline void setOnClose(OnClose const & cb) { _close_cb = cb; }
    inline void setOnExit (OnExit  const & cb) {  _exit_cb = cb; }
    // @formatter:on

public:
    virtual void onClose() override
    {
        if (static_cast<bool>(_close_cb)) {
            _close_cb();
        }
    }

    virtual void onExit(int64_t exit_status, int term_signal) override
    {
        if (static_cast<bool>(_exit_cb)) {
            _exit_cb(exit_status, term_signal);
        }
    }
};

} // namespace func
} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCPROCESS_HPP__

