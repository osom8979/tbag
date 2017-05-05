/**
 * @file   TimeoutToShutdown.hpp
 * @brief  TimeoutToShutdown class prototype.
 * @author zer0
 * @date   2017-05-05
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVPP_EX_TIMEOUTTOSHUTDOWN_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVPP_EX_TIMEOUTTOSHUTDOWN_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/uvpp/Timer.hpp>
#include <libtbag/uvpp/Stream.hpp>
#include <libtbag/uvpp/Request.hpp>

#include <atomic>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {
namespace ex   {

/**
 * TimeoutToShutdown class prototype.
 *
 * @author zer0
 * @date   2017-05-05
 */
class TBAG_API TimeoutToShutdown : public Timer
{
public:
    using AtomicBool = std::atomic_bool;

public:
    Stream * _stream;
    ShutdownRequest _request;

    AtomicBool _cancel;
    AtomicBool _auto_close;

public:
    TimeoutToShutdown(Loop & loop, Stream * stream, bool auto_close = true);
    virtual ~TimeoutToShutdown();

public:
    inline bool isCancel() const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_cancel.load()))
    { return _cancel.load(); }
    inline void cancel() TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_cancel.store(true)))
    { _cancel.store(true); }

    inline bool isAutoClose() const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_auto_close.load()))
    { return _auto_close.load(); }
    inline void setAutoClose(bool flag) TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_auto_close.store(true)))
    { _auto_close.store(flag); }

    inline ShutdownRequest & atRequest() TBAG_NOEXCEPT
    { return _request; }
    inline ShutdownRequest const & atRequest() const TBAG_NOEXCEPT
    { return _request; }

public:
    virtual void onTimer() override;
    virtual void onClose() override;
};

} // namespace ex
} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_EX_TIMEOUTTOSHUTDOWN_HPP__

