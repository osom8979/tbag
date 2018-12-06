/**
 * @file   UxPoll.hpp
 * @brief  UxPoll class prototype.
 * @author zer0
 * @date   2018-12-06
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXPOLL_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXPOLL_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/uvxx/UxHandle.hpp>
#include <libtbag/uvpp/func/FunctionalPoll.hpp>

#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvxx {

// Forward declaration.
class UxLoop;

/**
 * UxPoll class prototype.
 *
 * @author zer0
 * @date   2018-12-06
 */
class TBAG_API UxPoll : public libtbag::uvxx::UxHandle
{
public:
    using FuncPoll   = libtbag::uvpp::func::FuncPoll;
    using WeakPoll   = std::weak_ptr<FuncPoll>;
    using SharedPoll = std::shared_ptr<FuncPoll>;

public:
    using usock     = libtbag::uvpp::usock;
    using init_file = FuncPoll::init_file;
    using init_sock = FuncPoll::init_sock;
    using EventType = FuncPoll::EventType;

public:
    // @formatter:off
    TBAG_CONSTEXPR static EventType const EVENT_READABLE   = FuncPoll::EVENT_READABLE  ;
    TBAG_CONSTEXPR static EventType const EVENT_WRITABLE   = FuncPoll::EVENT_WRITABLE  ;
    TBAG_CONSTEXPR static EventType const EVENT_DISCONNECT = FuncPoll::EVENT_DISCONNECT;
    TBAG_CONSTEXPR static EventType const EVENT_RW         = FuncPoll::EVENT_RW        ;
    // @formatter:on

public:
    UxPoll();
    explicit UxPoll(UxLoop & loop, init_file fd);
    explicit UxPoll(UxLoop & loop, init_sock sock);
    UxPoll(UxPoll const & obj) TBAG_NOEXCEPT;
    UxPoll(UxPoll && obj) TBAG_NOEXCEPT;
    ~UxPoll();

public:
    UxPoll & operator =(UxPoll const & obj) TBAG_NOEXCEPT;
    UxPoll & operator =(UxPoll && obj) TBAG_NOEXCEPT;

public:
    void copy(UxPoll const & obj) TBAG_NOEXCEPT;
    void swap(UxPoll & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(UxPoll & lh, UxPoll & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }

public:
    SharedPoll lock() const TBAG_NOEXCEPT_SP_OP(_handle.lock())
    { return std::static_pointer_cast<FuncPoll>(_handle.lock()); }

public:
    // @formatter:off
    void setOnClose(FuncPoll::OnClose const & cb) { lock()->close_cb = cb; }
    void setOnWalk (FuncPoll::OnWalk  const & cb) { lock()->walk_cb  = cb; }
    void setOnPoll (FuncPoll::OnPoll  const & cb) { lock()->poll_cb  = cb; }
    // @formatter:on

public:
    Err initFile(UxLoop & loop, int fd);
    Err initSock(UxLoop & loop, usock sock);

public:
    Err start(EventType events = EVENT_RW);
    Err stop();

public:
    // @formatter:off
    static bool isEventReadable  (EventType event) TBAG_NOEXCEPT;
    static bool isEventWritable  (EventType event) TBAG_NOEXCEPT;
    static bool isEventDisconnect(EventType event) TBAG_NOEXCEPT;
    // @formatter:on
};

} // namespace uvxx

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXPOLL_HPP__

