/**
 * @file   TimeoutCancel.hpp
 * @brief  TimeoutCancel class prototype.
 * @author zer0
 * @date   2017-05-19
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVPP_EX_TIMEOUTCANCEL_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVPP_EX_TIMEOUTCANCEL_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/uvpp/Timer.hpp>
#include <libtbag/uvpp/Request.hpp>

#include <atomic>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {

// Forward declaration.
class Loop;

namespace ex   {

/**
 * TimeoutCancel class prototype.
 *
 * @author zer0
 * @date   2017-05-19
 */
class TBAG_API TimeoutCancel : public Timer
{
public:
    using AtomicBool = std::atomic_bool;

public:
    Request * _request;

    AtomicBool _cancel;
    AtomicBool _auto_close;

public:
    TimeoutCancel(Loop & loop, Request * request, bool auto_close = true);
    virtual ~TimeoutCancel();

public:
    inline bool isCancel() const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_cancel.load()))
    { return _cancel.load(); }
    inline void cancel(bool flag = true) TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_cancel.store(true)))
    { _cancel.store(flag); }

    inline bool isAutoClose() const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_auto_close.load()))
    { return _auto_close.load(); }
    inline void setAutoClose(bool flag = true) TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_auto_close.store(true)))
    { _auto_close.store(flag); }

public:
    /** Start the timer. timeout and repeat are in milliseconds. */
    Err start(uint64_t timeout);

public:
    virtual void onTimer() override;
    virtual void onClose() override;
};

} // namespace ex
} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_EX_TIMEOUTCANCEL_HPP__

