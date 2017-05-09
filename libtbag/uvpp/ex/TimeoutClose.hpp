/**
 * @file   TimeoutClose.hpp
 * @brief  TimeoutClose class prototype.
 * @author zer0
 * @date   2017-05-09
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVPP_EX_TIMEOUTCLOSE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVPP_EX_TIMEOUTCLOSE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/uvpp/Timer.hpp>

#include <atomic>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {

// Forward declaration.
class Loop;

namespace ex   {

/**
 * TimeoutClose class prototype.
 *
 * @author zer0
 * @date   2017-05-05
 */
class TBAG_API TimeoutClose : public Timer
{
public:
    using AtomicBool = std::atomic_bool;

private:
    Handle * _handle;

    AtomicBool _cancel;
    AtomicBool _auto_close;

public:
    TimeoutClose(Loop & loop, Handle * handle, bool auto_close = true);
    virtual ~TimeoutClose();

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
    uerr start(uint64_t timeout);

public:
    virtual void onTimer() override;
    virtual void onClose() override;
};

} // namespace ex
} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_EX_TIMEOUTCLOSE_HPP__

