/**
 * @file   UxFsEvent.hpp
 * @brief  UxFsEvent class prototype.
 * @author zer0
 * @date   2018-12-06
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXFSEVENT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXFSEVENT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/uvxx/UxHandle.hpp>
#include <libtbag/uvpp/func/FunctionalFsEvent.hpp>

#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvxx {

// Forward declaration.
class UxLoop;

/**
 * UxFsEvent class prototype.
 *
 * @author zer0
 * @date   2018-12-06
 */
class TBAG_API UxFsEvent : public libtbag::uvxx::UxHandle
{
public:
    using FuncFsEvent   = libtbag::uvpp::func::FuncFsEvent;
    using WeakFsEvent   = std::weak_ptr<FuncFsEvent>;
    using SharedFsEvent = std::shared_ptr<FuncFsEvent>;

public:
    using Event     = FuncFsEvent::Event;
    using EventFlag = FuncFsEvent::EventFlag;

public:
    // @formatter:off
    TBAG_CONSTEXPR static EventFlag const EVENT_FLAG_WATCH_ENTRY = FuncFsEvent::EVENT_FLAG_WATCH_ENTRY;
    TBAG_CONSTEXPR static EventFlag const EVENT_FLAG_STAT        = FuncFsEvent::EVENT_FLAG_STAT       ;
    TBAG_CONSTEXPR static EventFlag const EVENT_FLAG_RECURSIVE   = FuncFsEvent::EVENT_FLAG_RECURSIVE  ;
    TBAG_CONSTEXPR static EventFlag const EVENT_FLAG_DEFAULT     = FuncFsEvent::EVENT_FLAG_DEFAULT    ;
    // @formatter:on

public:
    UxFsEvent();
    UxFsEvent(UxLoop & loop);
    UxFsEvent(UxFsEvent const & obj) TBAG_NOEXCEPT;
    UxFsEvent(UxFsEvent && obj) TBAG_NOEXCEPT;
    ~UxFsEvent();

public:
    UxFsEvent & operator =(UxFsEvent const & obj) TBAG_NOEXCEPT;
    UxFsEvent & operator =(UxFsEvent && obj) TBAG_NOEXCEPT;

public:
    void copy(UxFsEvent const & obj) TBAG_NOEXCEPT;
    void swap(UxFsEvent & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(UxFsEvent & lh, UxFsEvent & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }

public:
    SharedFsEvent lock() const TBAG_NOEXCEPT_SP_OP(_handle.lock())
    { return std::static_pointer_cast<FuncFsEvent>(_handle.lock()); }

public:
    // @formatter:off
    void setOnClose  (FuncFsEvent::OnClose   const & cb) { lock()->close_cb    = cb; }
    void setOnWalk   (FuncFsEvent::OnWalk    const & cb) { lock()->walk_cb     = cb; }
    void setOnFsEvent(FuncFsEvent::OnFsEvent const & cb) { lock()->fs_event_cb = cb; }
    // @formatter:on

public:
    Err init(UxLoop & loop);

public:
    Err start(char const * path, EventFlag flags = EVENT_FLAG_DEFAULT);
    Err stop();

public:
    std::string getPath();

public:
    static Event getEvent(int native_events) TBAG_NOEXCEPT;
};

} // namespace uvxx

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXFSEVENT_HPP__

